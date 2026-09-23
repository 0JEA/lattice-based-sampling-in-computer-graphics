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

vector operator+(const vector& vec1, const vector& vec2);
vector operator-(const vector& vec1, const vector& vec2);
vector operator*(const vector& vec1, const vector& vec2);
vector operator*(const vector& vec, float scalar);
vector operator*(float scalar, const vector& vec);
vector operator/(const vector& vec, float scalar);

// dot product: a · b = Sum(a_i * b_i)
float dot(const vector& a, const vector& b);

// cross product, gives a vector orthogonal to the others.
vector cross(const vector& a, const vector& b);

vector unit_vector(const vector& v);

#endif
