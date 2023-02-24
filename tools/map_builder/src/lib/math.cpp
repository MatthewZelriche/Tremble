#include "math.hpp"

#include <glm/gtx/vector_angle.hpp>

float TR::DotProduct(const Vec3 &v1, const Vec3 &v2) { return glm::dot(v1, v2); }

TR::Vec3 TR::CrossProduct(const Vec3 &v1, const Vec3 &v2) { return glm::cross(v1, v2); }

float TR::Angle(const Vec3 &v1, const Vec3 &v2) {
   return glm::degrees(glm::angle(v1, v2));
}

TR::Vec3 TR::Normalize(const Vec3 &vector) { return glm::normalize(vector); }

float TR::SignedDistToPlane(const PlaneEq &plane, const Vec3 &point) {
   // Have to negate the result for some reason. Something to do with how
   // we define the plane equation?
   return -(glm::dot(plane.normal, point) + plane.dist);
}

TR::PlaneEq TR::CreatePlane(const Vec3 &p1, const Vec3 &p2, const Vec3 &p3) {
   TR::PlaneEq equation;
   equation.normal = glm::normalize(glm::cross(p3 - p2, p1 - p2));
   equation.point = p1;
   equation.dist = -glm::dot(equation.normal, p1);
   return equation;
}