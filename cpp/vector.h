#ifndef VECTOR2_H
#define VECTOR2_H

#include <cmath>

// Standard vecor type, locked into 3 dimensions.
struct vector {
  // Going with a float becuase I'll never need the extra precision.
  float element[3];

  // Args or not, it gets created wither with zeros or the args.
  vector() : element{0, 0, 0} {}
  vector(float element1, float element2, float element3)
      : element{element1, element2, element3} {}

  // No need for getters, dont see a need to make this a class and have anything
  // private.

  // Make is so we can do -vec and it negeates.
  vector operator-() const {
    return vector(-element[0], -element[1], -element[2]);
  }

  float operator[](int i) const { return element[i]; }
  float& operator[](int i) { return element[i]; }

  // Theres many of the operations I see in the reference online, but I'm not
  // sure if its better to just copy it all, or not imeplement it until its
  // needed. Is it lazy to not do now, futureproofing? Overworking overloads?
  // Not sure, due to my lack of experiance. Thoughts Thibaud?

  // sqrt(a_1^2 + a_2^2 + ... + a_i^2)
  float length() const {
    return std::sqrt(element[0] * element[0] + element[1] * element[1] +
                     element[2] * element[2]);
  }
};

// I see the use of inline now, this is so we dont need to compile with a .cpp
// linked and this kind of behaviour still belongs here from what I've read.
inline vector operator+(const vector& vec1, const vector& vec2) {
  return vector(vec1.element[0] + vec2.element[0],
                vec1.element[1] + vec2.element[1],
                vec1.element[2] + vec2.element[2]);
}

inline vector operator-(const vector& vec1, const vector& vec2) {
  return vector(vec1.element[0] - vec2.element[0],
                vec1.element[1] - vec2.element[1],
                vec1.element[2] - vec2.element[2]);
}

inline vector operator*(const vector& vec1, const vector& vec2) {
  return vector(vec1.element[0] * vec2.element[0],
                vec1.element[1] * vec2.element[1],
                vec1.element[2] * vec2.element[2]);
}

inline vector operator*(const vector& vec, float scalar) {
  return vector(vec.element[0] * scalar, vec.element[1] * scalar,
                vec.element[2] * scalar);
}

inline vector operator*(float scalar, const vector& vec) {
  return vec * scalar;
}

inline vector operator/(const vector& vec, float scalar) {
  return vec * (1.0f / scalar);
}

// dot product: a · b = Sum(a_i * b_i)
inline float dot(const vector& a, const vector& b) {
  return (a[0] * b[0]) + (a[1] * b[1]) + (a[2] * b[2]);
}

// cross product, gives a vector orthogonal to the others.
inline vector cross(const vector& a, const vector& b) {
  return vector(a[1] * b[2] - a[2] * b[1], a[2] * b[0] - a[0] * b[2],
                a[0] * b[1] - a[1] * b[0]);
}

inline vector unit_vector(const vector& v) { return v / v.length(); }

#endif
