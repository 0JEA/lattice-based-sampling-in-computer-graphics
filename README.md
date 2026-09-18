# Lattice-Based Sampling in Computer Graphics: Building a Renderer

A private collection of student work for a university directed reading.  
Yes I wrote this in third person, it wasn't AI ;)

|                 |                                                                                                                                                        |
| --------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------ |
| **Course**      | AUCSC 395, Directed Reading 1                                                                                                                          |
| **Institution** | University of Alberta, Augustana Campus                                                                                                                |
| **Professor**   | [Dr. Thibaud Lutellier](https://apps.ualberta.ca/directory/person/lutellie) ([Google Scholar](https://scholar.google.com/citations?user=bQECG60AAAAJ)) |
| **Student**     | Johnothan Andres                                                                                                                                       |
| **Term**        | Fall 2026                                                                                                                                              |

---

## PPM files (portable pixmaps)

I have begun by using these files as directed in
[_Ray Tracing in One Weekend_](https://raytracing.github.io/books/RayTracingInOneWeekend.html)
to play with getting colour and shapes to render fast.

As described in PPM's own man page, these are "egregiously inefficient",
highly redundant, and not recommended for much real work. That said, they are
very easy to write, read, and analyse, which funnily enough makes them a good
starting point for learning.

### The plain PPM format

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
