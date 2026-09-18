# PPM examples

| Path                   | Contents                                        |
| ---------------------- | ----------------------------------------------- |
| [`src/`](src/)         | `create_ppm.cpp`, the generator                 |
| [`ppm/`](ppm/)         | the generated `.ppm` images, plus viewing notes |
| [`outputs/`](outputs/) | PNG captures of how those images render         |

GitHub cannot display `.ppm` files, so every image below is a PNG capture of
the render sitting beside the plain-text source that produced it.

## 4_pixels.ppm

![2x2 render beside its P3 source](outputs/4_pixels_output.png)

## 12_pixels.ppm

![5x6 render beside its P3 source](outputs/30_pixels_output.png)

## Index of every test image

![feh index showing all five test images with name, size and dimensions](outputs/ppm_outputs.png)

Produced with feh's full index mode:

```sh
feh -m -I -e NotoSans-Medium/14 -x -W 550 *.ppm -o ppm_outputs.png
```

## Viewing these locally

See [`ppm/how_to_view.txt`](ppm/how_to_view.txt). In short: open the file with
`feh`, press the up arrow to zoom in until individual pixels are visible, and
press `A` to toggle anti-aliasing off if the pixels look like a smooth
gradient instead of hard-edged blocks.
