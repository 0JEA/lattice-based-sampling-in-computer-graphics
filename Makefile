CXX      = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -MMD -MP

BUILD  = build
SHARED = $(BUILD)/ppm.o $(BUILD)/color.o $(BUILD)/vector.o

all: $(BUILD)/create_ppm.exe $(BUILD)/camera_testing.exe

$(BUILD)/create_ppm.exe: $(BUILD)/create_ppm.o $(SHARED)
	$(CXX) $^ -o $@

$(BUILD)/camera_testing.exe: $(BUILD)/camera_testing.o $(SHARED)
	$(CXX) $^ -o $@

$(BUILD)/%.o: src/%.cpp | $(BUILD)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD):
	mkdir -p $(BUILD)

clean:
	rm -rf $(BUILD)

-include $(wildcard $(BUILD)/*.d)

.PHONY: all clean
