#pragma once

#include <glm/glm.hpp>
#include <vector>

enum class MoveDir { BACK = -1, FOREWARD = 1 };

enum class StrafeDir { LEFT = -1, RIGHT = 1 };

class Camera {
  public:
   Camera(float fov, float aspectRatio);
   Camera(glm::vec3 startPos, float fov, float aspectRatio);
   Camera(glm::vec3 startPos, float pitch, float yaw, float fov, float aspectRatio);

   void Tick(float deltaTime);
   void SetNewAspectRatio(int screenWidth, int screenHeight);
   void Move(MoveDir dir);
   void Strafe(StrafeDir dir);
   void Look(float deltaYaw, float deltaPitch);
   inline glm::mat4 ProjectionMatrix() const { return m_Projection; }
   inline glm::mat4 ViewProjectionMatrix() const { return m_Projection * m_View; }
   inline glm::mat4 ViewMatrix() const { return m_View; }
   inline glm::vec3 GetWorldPos() { return m_cameraWorldPos; }

  private:
   void CalcViewMatrix();
   void CalcAxes();

   float m_Fov = 50.0f;
   float m_LookSpeed = 8.0f;
   float m_MoveSpeed = 8.0f;
   float m_Pitch = 0.0f;
   float m_Yaw = 0.0f;

   glm::vec3 moveAmount = {0.0f, 0.0f, 0.0f};
   glm::vec3 m_zeroVec = {0.0f, 0.0f, 0.0f};

   glm::vec3 m_Front = {0.0f, 0.0f, 0.0f};
   glm::vec3 m_Right = {0.0f, 0.0f, 0.0f};
   glm::vec3 m_cameraWorldPos = {0.0f, 0.0f, 0.0f};

   glm::mat4 m_Projection = glm::mat4(1.0f);
   glm::mat4 m_View = glm::mat4(1.0f);
   glm::mat4 m_RotationMatrix = glm::mat4(1.0f);
};