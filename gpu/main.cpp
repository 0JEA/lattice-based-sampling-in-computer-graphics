// Vulkan setup adapted from NVIDIA's vk_mini_path_tracer, chapter 7 (Apache-2.0).
// Copyright 2020-2024 NVIDIA Corporation
// SPDX-License-Identifier: Apache-2.0
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include <nvh/fileoperations.hpp>
#include <nvvk/context_vk.hpp>
#include <nvvk/descriptorsets_vk.hpp>
#include <nvvk/error_vk.hpp>
#include <nvvk/resourceallocator_vk.hpp>
#include <nvvk/shaders_vk.hpp>

#include <algorithm>
#include <cstdint>
#include <string>
#include <vector>

static const uint32_t render_width     = 1000;
static const uint32_t render_height    = 1000;
static const uint32_t workgroup_width  = 16;
static const uint32_t workgroup_height = 8;

struct Settings
{
  uint32_t width;
  uint32_t height;
  int32_t  aa_grid;
  int32_t  shadow_samples_per_side;
};

int main(int argc, const char** argv)
{
  Settings settings{.width = render_width, .height = render_height, .aa_grid = 8, .shadow_samples_per_side = 8};

  nvvk::ContextCreateInfo deviceInfo;
  deviceInfo.apiMajor = 1;
  deviceInfo.apiMinor = 2;

  nvvk::Context context;
  context.init(deviceInfo);

  nvvk::ResourceAllocatorDedicated allocator;
  allocator.init(context, context.m_physicalDevice);

  VkDeviceSize       bufferSizeBytes = VkDeviceSize(render_width) * render_height * 3 * sizeof(float);
  VkBufferCreateInfo bufferCreateInfo{.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
                                      .size  = bufferSizeBytes,
                                      .usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT};
  nvvk::Buffer buffer = allocator.createBuffer(bufferCreateInfo, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
                                                                     | VK_MEMORY_PROPERTY_HOST_CACHED_BIT
                                                                     | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

  const std::string        exePath(argv[0], std::string(argv[0]).find_last_of("/\\") + 1);
  std::vector<std::string> searchPaths = {exePath + PROJECT_RELDIRECTORY, exePath + PROJECT_RELDIRECTORY "..",
                                          exePath + PROJECT_RELDIRECTORY "../..", exePath + PROJECT_NAME};

  VkCommandPoolCreateInfo cmdPoolInfo{.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO, .queueFamilyIndex = context.m_queueGCT};
  VkCommandPool           cmdPool;
  NVVK_CHECK(vkCreateCommandPool(context, &cmdPoolInfo, nullptr, &cmdPool));

  nvvk::DescriptorSetContainer descriptorSetContainer(context);
  descriptorSetContainer.addBinding(0, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, VK_SHADER_STAGE_COMPUTE_BIT);
  descriptorSetContainer.initLayout();
  descriptorSetContainer.initPool(1);

  VkPushConstantRange pushConstantRange{.stageFlags = VK_SHADER_STAGE_COMPUTE_BIT, .offset = 0, .size = sizeof(Settings)};
  descriptorSetContainer.initPipeLayout(1, &pushConstantRange);

  VkDescriptorBufferInfo descriptorBufferInfo{.buffer = buffer.buffer, .range = bufferSizeBytes};
  VkWriteDescriptorSet   writeDescriptor = descriptorSetContainer.makeWrite(0, 0, &descriptorBufferInfo);
  vkUpdateDescriptorSets(context, 1, &writeDescriptor, 0, nullptr);

  VkShaderModule rayTraceModule =
      nvvk::createShaderModule(context, nvh::loadFile("shaders/raytrace.comp.glsl.spv", true, searchPaths));

  VkPipelineShaderStageCreateInfo shaderStageCreateInfo{.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
                                                        .stage  = VK_SHADER_STAGE_COMPUTE_BIT,
                                                        .module = rayTraceModule,
                                                        .pName  = "main"};

  VkComputePipelineCreateInfo pipelineCreateInfo{.sType  = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO,
                                                 .stage  = shaderStageCreateInfo,
                                                 .layout = descriptorSetContainer.getPipeLayout()};
  VkPipeline computePipeline;
  NVVK_CHECK(vkCreateComputePipelines(context, VK_NULL_HANDLE, 1, &pipelineCreateInfo, nullptr, &computePipeline));

  VkCommandBufferAllocateInfo cmdAllocInfo{.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
                                           .commandPool        = cmdPool,
                                           .level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
                                           .commandBufferCount = 1};
  VkCommandBuffer             cmdBuffer;
  NVVK_CHECK(vkAllocateCommandBuffers(context, &cmdAllocInfo, &cmdBuffer));

  VkCommandBufferBeginInfo beginInfo{.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
                                     .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT};
  NVVK_CHECK(vkBeginCommandBuffer(cmdBuffer, &beginInfo));

  vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, computePipeline);
  VkDescriptorSet descriptorSet = descriptorSetContainer.getSet(0);
  vkCmdBindDescriptorSets(cmdBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, descriptorSetContainer.getPipeLayout(), 0, 1,
                          &descriptorSet, 0, nullptr);
  vkCmdPushConstants(cmdBuffer, descriptorSetContainer.getPipeLayout(), VK_SHADER_STAGE_COMPUTE_BIT, 0,
                     sizeof(Settings), &settings);

  vkCmdDispatch(cmdBuffer, (render_width + workgroup_width - 1) / workgroup_width,
                (render_height + workgroup_height - 1) / workgroup_height, 1);

  VkMemoryBarrier memoryBarrier{.sType         = VK_STRUCTURE_TYPE_MEMORY_BARRIER,
                                .srcAccessMask = VK_ACCESS_SHADER_WRITE_BIT,
                                .dstAccessMask = VK_ACCESS_HOST_READ_BIT};
  vkCmdPipelineBarrier(cmdBuffer, VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT, VK_PIPELINE_STAGE_HOST_BIT, 0, 1,
                       &memoryBarrier, 0, nullptr, 0, nullptr);

  NVVK_CHECK(vkEndCommandBuffer(cmdBuffer));

  VkSubmitInfo submitInfo{.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO, .commandBufferCount = 1, .pCommandBuffers = &cmdBuffer};

  NVVK_CHECK(vkQueueSubmit(context.m_queueGCT, 1, &submitInfo, VK_NULL_HANDLE));
  NVVK_CHECK(vkQueueWaitIdle(context.m_queueGCT));

  const float*         linear = reinterpret_cast<const float*>(allocator.map(buffer));
  std::vector<uint8_t> bytes(size_t(render_width) * render_height * 3);
  for(size_t i = 0; i < bytes.size(); ++i)
  {
    bytes[i] = uint8_t(255.999f * std::clamp(linear[i], 0.0f, 1.0f));
  }
  allocator.unmap(buffer);
  stbi_write_png("out.png", render_width, render_height, 3, bytes.data(), render_width * 3);

  vkDestroyPipeline(context, computePipeline, nullptr);
  vkDestroyShaderModule(context, rayTraceModule, nullptr);
  descriptorSetContainer.deinit();
  vkFreeCommandBuffers(context, cmdPool, 1, &cmdBuffer);
  vkDestroyCommandPool(context, cmdPool, nullptr);
  allocator.destroy(buffer);
  allocator.deinit();
  context.deinit();
}
