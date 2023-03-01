#include "camera.hpp"

#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

Camera::Camera(float fov, float aspectRatio) :
    Camera({0.0f, 0.0f, 0.0f}, 0.0f, 0.0f, fov, aspectRatio) {}
Camera::Camera(glm::vec3 startPos, float fov, float aspectRatio) :
    Camera(startPos, 0.0f, 0.0f, fov, aspectRatio) {}

Camera::Camera(glm::vec3 startPos, float pitch, float yaw, float fov, float aspectRatio) {
   m_cameraWorldPos = startPos;
   m_Fov = fov;
   m_Pitch = pitch;
   m_Yaw = yaw;

   m_RotationMatrix = glm::eulerAngleXY(std::clamp(pitch, -89.9f, 89.9f), yaw);
   m_Projection = glm::perspective(glm::radians(fov), aspectRatio, 0.1f, 10000.f);

   CalcViewMatrix();
   CalcAxes();
}

void Camera::Tick(float deltaTime) {
   if (moveAmount != m_zeroVec) { moveAmount = glm::normalize(moveAmount); }
   m_cameraWorldPos += moveAmount * deltaTime * m_MoveSpeed;
   m_RotationMatrix = glm::eulerAngleXY(glm::radians(m_Pitch), glm::radians(m_Yaw));

   CalcViewMatrix();
   CalcAxes();

   moveAmount = {0.0f, 0.0f, 0.0f};
}

void Camera::SetNewAspectRatio(int screenWidth, int screenHeight) {
   m_Projection = glm::perspective(glm::radians(m_Fov), (float)screenWidth / screenHeight,
                                   0.1f, 1000.f);
}

void Camera::Move(MoveDir dir) { moveAmount -= m_Front * (float)dir; }

void Camera::Strafe(StrafeDir dir) { moveAmount += m_Right * (float)dir; }

void Camera::Look(float deltaYaw, float deltaPitch) {
   m_Pitch += deltaPitch * m_LookSpeed;
   m_Pitch = std::clamp(m_Pitch, -89.9f, 89.9f);

   m_Yaw += deltaYaw * m_LookSpeed;
}

void Camera::CalcViewMatrix() {
   m_View = glm::translate(m_RotationMatrix, -m_cameraWorldPos);
}

void Camera::CalcAxes() {
   auto inv = glm::inverse(m_View);
   m_Front = inv[2];
   m_Right = inv[0];
}