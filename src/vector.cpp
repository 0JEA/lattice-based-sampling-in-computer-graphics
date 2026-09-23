#include "vector.h"

vector operator+(const vector& vec1, const vector& vec2) {
  return vector(vec1.element[0] + vec2.element[0],
                vec1.element[1] + vec2.element[1],
                vec1.element[2] + vec2.element[2]);
}

vector operator-(const vector& vec1, const vector& vec2) {
  return vector(vec1.element[0] - vec2.element[0],
                vec1.element[1] - vec2.element[1],
                vec1.element[2] - vec2.element[2]);
}

vector operator*(const vector& vec1, const vector& vec2) {
  return vector(vec1.element[0] * vec2.element[0],
                vec1.element[1] * vec2.element[1],
                vec1.element[2] * vec2.element[2]);
}

vector operator*(const vector& vec, float scalar) {
  return vector(vec.element[0] * scalar, vec.element[1] * scalar,
                vec.element[2] * scalar);
}

vector operator*(float scalar, const vector& vec) {
  return vec * scalar;
}

vector operator/(const vector& vec, float scalar) {
  return vec * (1.0f / scalar);
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
