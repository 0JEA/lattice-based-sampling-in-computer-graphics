# Report 1 — `inline` and the `vec3` class

|                 |                                                                                                                                                        |
| --------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------ |
| **Course**      | AUCSC 395, Directed Reading 1                                                                                                                          |
| **Institution** | University of Alberta, Augustana Campus                                                                                                                |
| **Professor**   | [Dr. Thibaud Lutellier](https://apps.ualberta.ca/directory/person/lutellie) ([Google Scholar](https://scholar.google.com/citations?user=bQECG60AAAAJ)) |
| **Student**     | Johnothan Andres                                                                                                                                       |
| **Term**        | Fall 2026                                                                                                                                              |
| **Date**        | September 18, 2028                                                                                                                                     |

---

## 1. The use of `inline` in the `vec3` class

While working through the examples given in chapter 3 of
[_Ray Tracing in One Weekend_](https://raytracing.github.io/books/RayTracingInOneWeekend.html),
I came across the `vec3` class and its use of `inline`.

`inline` is new to me, and seeing every declaration sitting inside the header
file felt _very_ wrong at first — my instinct was that definitions belong in a
`.cpp` file, not a `.h` file.

After some reading, I found there is a strong reason they did this.

The idea is that functions like those on `vec3` get called extremely often in a
renderer. Declaring them `inline` inside the header encourages the compiler to
inline them at each call site, which can reduce runtime in a possibly
substantial way.

The utility functions in the book's header look like this:

```cpp
inline vec3 operator+(const vec3 &u, const vec3 &v) {
  return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline double dot(const vec3 &u, const vec3 &v) {
  return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2];
}
```

The member functions defined inside the class body — `x()`, `length()`,
`operator+=`, and so on — are implicitly `inline` for the same reason, without
needing the keyword written out.

### What the C++ Core Guidelines say

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

— [C++ Core Guidelines, F.5](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#f5-if-a-function-is-very-small-and-time-critical-declare-it-inline)

---

## References

- [C++ Core Guidelines — F.5: If a function is very small and time-critical, declare it `inline`](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#f5-if-a-function-is-very-small-and-time-critical-declare-it-inline)
- [cppreference — `inline` specifier](https://en.cppreference.com/cpp/language/inline)
- [_Ray Tracing in One Weekend_, chapter 3](https://raytracing.github.io/books/RayTracingInOneWeekend.html)
