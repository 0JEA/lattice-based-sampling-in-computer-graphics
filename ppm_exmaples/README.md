## Where the example files live

| Path                                                | Contents                                        |
| --------------------------------------------------- | ----------------------------------------------- |
| [`ppm_exmaples/src/`](../ppm_exmaples/src/)         | `create_ppm.cpp`, the generator                 |
| [`ppm_exmaples/ppm/`](../ppm_exmaples/ppm/)         | the generated `.ppm` images, plus viewing notes |
| [`ppm_exmaples/outputs/`](../ppm_exmaples/outputs/) | PNG captures of how those images render         |

GitHub cannot display `.ppm` files, so every image below is a PNG screenshot of
them so you can easily see their output without having to mess around with feh.

## `4_pixels.ppm`

Manually made this:  
![2x2](../ppm_exmaples/outputs/4_pixels_output.png)

## `5x1.ppm` & `1x5.ppm`

Made these with my `create_ppm.exe`  
![5x1](../ppm_exmaples/outputs/5x1_output.png)
![1x5](../ppm_exmaples/outputs/1x5_output.png)

These two made me notice a bug in the [_Ray Tracing in One Weekend_](https://raytracing.github.io/books/RayTracingInOneWeekend.html) code section
2.3 where if the row or column is 1 or less there is a division by zero.

Obviously they're just giving quick examples, but it's fun to be running into bugs
so early.

Quick fix:

```cpp
double red = width > 1 ? double(column) / (width - 1) : 0;
double green = height > 1 ? double(row) / (height - 1) : 0;
```

## `30_pixels.ppm`

Made this by hand:  
![5x6](../ppm_exmaples/outputs/30_pixels_output.png)

## `100x100.ppm`

Made this with my `create_ppm.exe`  
![100x100](../ppm_exmaples/outputs/100x100_output.png)

## Index of every test image

![feh index](../ppm_exmaples/outputs/ppm_outputs.png)

This seems like a awesome way to show and compare many images so I'll likely be
using this along the project.

Produced with feh's full index mode:

```sh
feh -m -I -e NotoSans-Medium/14 -x -W 550 *.ppm -o ppm_outputs.png
```
