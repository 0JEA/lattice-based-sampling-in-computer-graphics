# Report 1:

|                 |                                                                                                                                                        |
| --------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------ |
| **Course**      | AUCSC 395, Directed Reading 1                                                                                                                          |
| **Institution** | University of Alberta, Augustana Campus                                                                                                                |
| **Professor**   | [Dr. Thibaud Lutellier](https://apps.ualberta.ca/directory/person/lutellie) ([Google Scholar](https://scholar.google.com/citations?user=bQECG60AAAAJ)) |
| **Student**     | Johnothan Andres                                                                                                                                       |
| **Term**        | Fall 2026                                                                                                                                              |
| **Date**        | September 18, 2026                                                                                                                                     |

**Project:** Lattice-Based Sampling in Computer Graphics: Building a Renderer

---

## Contents

1. [PPM files (portable pixmaps)](#1-ppm-files-portable-pixmaps)
2. [The `inline` keyword in the `vec3` class](#2-the-inline-keyword-in-the-vec3-class)
3. [References](#references)

---

## 1. PPM files (portable pixmaps)

I have begun by using these files as directed in
[_Ray Tracing in One Weekend_](https://raytracing.github.io/books/RayTracingInOneWeekend.html)
to play with getting colour and shapes to render fast.

As described in PPM's own man page, these are "egregiously inefficient",
highly redundant, and not recommended for much real work. That said, they are
very easy to write, read, and analyse, which funnily enough makes them a good
starting point for learning.

### 1.1 The plain PPM format

I am starting with the "plain PPM" format. This is `P3`, the magic number that
identifies the format in use. See `man 5 ppm` for the full specification.

A good example of `P3` in use is _Ray Tracing in One Weekend_, chapter 2.1,
figure 1. Each pixel is represented by a triplet of (red, green, blue):

```
P3
5 1
255
0 0 0
63 0 0
127 0 0
191 0 0
255 0 0
```

### 1.2 Where the example files live

| Path                                                | Contents                                        |
| --------------------------------------------------- | ----------------------------------------------- |
| [`ppm_exmaples/src/`](../ppm_exmaples/src/)         | `create_ppm.cpp`, the generator                 |
| [`ppm_exmaples/ppm/`](../ppm_exmaples/ppm/)         | the generated `.ppm` images, plus viewing notes |
| [`ppm_exmaples/outputs/`](../ppm_exmaples/outputs/) | PNG captures of how those images render         |

GitHub cannot display `.ppm` files, so every image below is a PNG screenshot of
them so you can easily see their output without having to mess around with feh.

### `4_pixels.ppm`

Manually made this:  
![2x2](../ppm_exmaples/outputs/4_pixels_output.png)

### `5x1.ppm` & `1x5.ppm`

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

### `30_pixels.ppm`

Made this by hand:  
![5x6](../ppm_exmaples/outputs/30_pixels_output.png)

### `100x100.ppm`

Made this with my `create_ppm.exe`
![100x100](../ppm_exmaples/outputs/100x100_output.png)

### 1.8 Index of every test image

![feh index](../ppm_exmaples/outputs/ppm_outputs.png)

This seems like a awesome way to show and compare many images so I'll likely be
using this along the project.

Produced with feh's full index mode:

```sh
feh -m -I -e NotoSans-Medium/14 -x -W 550 *.ppm -o ppm_outputs.png
```

### 1.9 Viewing these locally

See [`ppm_exmaples/ppm/how_to_view.txt`](../ppm_exmaples/ppm/how_to_view.txt).
In short: open the file with `feh`, press the up arrow to zoom in until
individual pixels are visible, and press `SHIFT+A` to toggle anti-aliasing off
if the pixels look like a smooth gradient instead of hard-edged blocks.

---

## 2. The `inline` keyword in the `vec3` class

- [cppreference: `inline` specifier](https://en.cppreference.com/cpp/language/inline)

While working through the examples given in chapter 3 of
_Ray Tracing in One Weekend_, I came across the `vec3` class and its use of
`inline`.

`inline` is new to me, and seeing all of the declarations sitting inside the
header file felt _very_ wrong at first. My instinct was that definitions belong
in a `.cpp` file, not a `.h` file.

After some reading, I found there is a strong reason they did this.

The idea is that functions like those on `vec3` get called extremely often in a
renderer. Declaring them `inline` inside the header encourages the compiler to
inline them at each call site, which can reduce runtime in a possibly
substantial way.

The utility functions in the example header look like this:

```cpp
inline vec3 operator+(const vec3 &u, const vec3 &v) {
  return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline double dot(const vec3 &u, const vec3 &v) {
  return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2];
}
```

I've likely overthought this, but I was almost convinced to keep using the `inline` keyword. Realistically tho I don't think I have the scale of a large project where this would make a noticeable impact I presume. So I wont be using it and later on if I have time after vulkan in implemented I can play with using inline and measure the gains.

Regardless here's a reputable quote talking about it.:

### 2.1 What the C++ Core Guidelines say

> **F.5: If a function is very small and time-critical, declare it `inline`**
>
> **Reason**
>
> Some optimizers are good at inlining without hints from the programmer, but
> don't rely on it. Measure! Over the last 40 years or so, we have been promised
> compilers that can inline better than humans without hints from humans. We are
> still waiting. Specifying `inline` (explicitly, or implicitly when writing
> member functions inside a class definition) encourages the compiler to do a
> better job.

Source: [C++ Core Guidelines, F.5](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#f5-if-a-function-is-very-small-and-time-critical-declare-it-inline)

---

## References

- [_Ray Tracing in One Weekend_](https://raytracing.github.io/books/RayTracingInOneWeekend.html), chapters 2.1 and 3
- [C++ Core Guidelines, F.5: If a function is very small and time-critical, declare it `inline`](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#f5-if-a-function-is-very-small-and-time-critical-declare-it-inline)
- [cppreference: `inline` specifier](https://en.cppreference.com/cpp/language/inline)
- `man 5 ppm`, the plain PPM (`P3`) specification
