This repo is a private collection of student work for a university
directed reading.

Univerity of Alberta, Augustana Campus
AUCSC 395 - Directed Reading 1
Lattice-Based Sampling in Computer Graphics: Building a Renderer

Professor: Dr. Thibaud Luttiere
Student: Johnothan Andres
Fall 2026

---

PPM Files (Portable Pixel Maps)

I have begun by using these files as directed in the "Ray tracing
in a weekend" to play with getting color and shaped to render fast.

As described in PPM's own man files, these are "egregiously inefficient",
highly redundant and not recommended for much real work. That said they're
very easy to write, read and analyse. Thus are funnily a good starting point
to learn.

I'll be using the "Plain PPM" format to start, this is P3, a magic number that identifies the format
being used. Read more: man 5 ppm

A good example of P3 being used is in https://raytracing.github.io/books/RayTracingInOneWeekend.html,
chaper 2) 2.1, fig 1.

Each pixel is represented by a triplet of (red, green, blue).
