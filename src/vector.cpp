#include "vector.h"

vector operator+(const vector& a, const vector& b) {
  return vector(a.element[0] + b.element[0], a.element[1] + b.element[1],
                a.element[2] + b.element[2]);
}

vector operator-(const vector& a, const vector& b) {
  return vector(a.element[0] - b.element[0], a.element[1] - b.element[1],
                a.element[2] - b.element[2]);
}

// Element by element, not a dot or cross product.
vector operator*(const vector& a, const vector& b) {
  return vector(a.element[0] * b.element[0], a.element[1] * b.element[1],
                a.element[2] * b.element[2]);
}

vector operator*(const vector& v, float scalar) {
  return vector(v.element[0] * scalar, v.element[1] * scalar,
                v.element[2] * scalar);
}

vector operator*(float scalar, const vector& v) { return v * scalar; }

vector operator/(const vector& v, float scalar) {
  return v * (1.0f / scalar);
}

// dot product: a · b = Sum(a_i * b_i)
float dot(const vector& a, const vector& b) {
  return (a[0] * b[0]) + (a[1] * b[1]) + (a[2] * b[2]);
}

// cross product, gives a vector orthogonal to the others.
vector cross(const vector& a, const vector& b) {
  return vector(a[1] * b[2] - a[2] * b[1], a[2] * b[0] - a[0] * b[2],
                a[0] * b[1] - a[1] * b[0]);
}

vector unit_vector(const vector& v) { return v / v.length(); }
