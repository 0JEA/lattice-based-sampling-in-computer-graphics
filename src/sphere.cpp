/* INFO: A sphere centered at the origin of radius r is:
              x^2 + y^2 + z^2 = r^2
         Thus x^2 + y^2 + z^2 < r^2 is inside the sphere.
         Thus x^2 + y^2 + z^2 > r^2 is outside the sphere.

   INFO: To center the sphere at any point (C_x,C_y,C_z):
         (C_x - x)^2 + (C_y - y)^2 + (C_z - z)^2 = r^2 is on the sphere surface.
    Thus (C_x - x)^2 + (C_y - y)^2 + (C_z - z)^2 < r^2 is inside the sphere.
    Thus (C_x - x)^2 + (C_y - y)^2 + (C_z - z)^2 > r^2 is outside the sphere.

   INFO: Let           Center = C(C_x,C_y,C_z)
               Selected Point = P(x,y,z)
         Then Movement Vector = (P-C) = from center to point.
         Let            (P-C) = M The movement vector.
         Now          M dot M = r^2 is on the sphere's surface
         And          M dot M < r^2 is inside the sphere at some point P
         And          M dot M > r^2 is outside the sphere at some point P
   INFO:
    Now we introduce a Ray, some Ray of light that travels in a straight linear
    line, a vector.

    We can find any point along that line of light with a simple
    function: P(t) = R + td;

    Where R is out ray, d is a distance vector.
    Then t is some point along that Ray.
    Now we can substitube a ray of light into the sphere equation to see
    if the light will hit the sphere.

 P(t) = R + td
    M = P(t) - C
      = (R+td) - C
 Then M dot M =

*/
