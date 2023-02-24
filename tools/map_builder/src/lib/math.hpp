#pragma once

#include <optional>

#include <glm/glm.hpp>

namespace TR {

// Need a substantially higher Epsilon, presumably because
// we are doing everything in single-precision.
// Hopefully this shouldn't be a problem, as its still well
// below the minimum trenchbrom grid size of 0.125
// TODO: May need to be tweaked
constexpr float BIG_EPS = 0.0003f;

using Vec3 = glm::vec3;
using Vec4 = glm::vec4;
using Vec2 = glm::vec2;
using IVec2 = glm::ivec2;

struct PlaneEq {
   Vec3 normal;
   Vec3 point;
   float dist;
};

/**
 * @brief Computes the signed angle, in degrees, between two normalized 3D vectors.
 * 
 * @param v1 A normalized 3D vector.
 * @param v2 A normalized 3D vector.
 * @return The signed angle, in degrees, between v1 and v2
 */
float Angle(const Vec3 &v1, const Vec3 &v2);
/**
 * @brief Computes the signed distance of a point to a plane.
 * 
 * The result is signed, such that a positive value indicates the point lies in front of 
 * the plane, and a negative value indicates the point lies behind the plane. The plane's 
 * normal direction is considered its "front". A value sufficiently close to zero should 
 * be interpreted as being directly on the plane.
 * 
 * @param plane The 3D plane to test against
 * @param point The 3D point to test
 * @return The signed distance from the point to the plane.
 */
float SignedDistToPlane(const PlaneEq &plane, const Vec3 &point);
/** @brief Computes the cross product of two vectors. */
Vec3 CrossProduct(const Vec3 &v1, const Vec3 &v2);
/** @brief Computes the dot product of two vectors. */
float DotProduct(const Vec3 &v1, const Vec3 &v2);
/** @brief Normalizes a vector. */
Vec3 Normalize(const Vec3 &vector);
/** @brief Constructs a plane from a 3-point plane equation. */
PlaneEq CreatePlane(const Vec3 &p1, const Vec3 &p2, const Vec3 &p3);
} // namespace TR
