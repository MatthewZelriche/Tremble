#include "math_util.hpp"

TR::PlaneEq TR::ComputePlane(const glm::vec3 &p1, const glm::vec3 &p2,
                             const glm::vec3 &p3) {
   TR::PlaneEq equation;
   equation.normal = glm::normalize(glm::cross(p3 - p2, p2 - p1));
   equation.point = p1;
   equation.dist = -glm::dot(equation.normal, p1);
   return equation;

   /*
   TR::PlaneEq equation;
   equation.normal = glm::normalize(glm::cross(p3 - p2, p2 - p1));
   equation.point = p1;
   equation.dist = (equation.normal.x * -p1.x) + (equation.normal.y * -p1.y) +
                   (equation.normal.z * -p1.z);

   return equation;
   */
}