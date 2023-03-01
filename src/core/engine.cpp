#include "engine.hpp"

#include "../io/input_manager.hpp"

#include <loguru.hpp>
#include <lib_types.hpp>
#include <serialization.hpp>

#include "../rendering/camera.hpp"

#include "PxPhysicsAPI.h"
#include "extensions/PxDefaultAllocator.h"

using namespace physx;

static PxDefaultAllocator gAllocator;

class UserErrorCallback : public PxErrorCallback {
  public:
   virtual void reportError(PxErrorCode::Enum code, const char *message, const char *file,
                            int line) {
      // error processing implementation
   }
};

static UserErrorCallback gCallback;

using namespace TR;

// Testing
Camera *camera;
// TODO: Move to input manager
double lastX = 0;
double lastY = 0;
void cursorCallback(GLFWwindow *window, double xpos, double ypos) {
   float deltaX = xpos - lastX;
   float deltaY = ypos - lastY;

   camera->Look(deltaX * 0.01f, deltaY * 0.01f);

   lastX = xpos;
   lastY = ypos;
}

void ProcKeys() {
   if (InputManager::IsKeyDown(KeyCode::KeyW)) { camera->Move(MoveDir::FOREWARD); }
   if (InputManager::IsKeyDown(KeyCode::KeyS)) { camera->Move(MoveDir::BACK); }
   if (InputManager::IsKeyDown(KeyCode::KeyA)) { camera->Strafe(StrafeDir::LEFT); }
   if (InputManager::IsKeyDown(KeyCode::KeyD)) { camera->Strafe(StrafeDir::RIGHT); }
}

Engine::Engine() {
   // Initialize logging
   loguru::set_thread_name("MAIN");
   // TODO: Log rotating
   loguru::add_file("error_log.txt", loguru::Truncate, loguru::Verbosity_ERROR);
   loguru::add_file("verbose_log.txt", loguru::Truncate, loguru::Verbosity_MAX);
   DLOG_F(INFO, "Successfully initialized logging module");

   InputManager::Initialize();
   mWindow = std::make_unique<Window>();
   glfwSetCursorPosCallback(mWindow->GetHandle(), cursorCallback);
   glfwSetInputMode(mWindow->GetHandle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
   // Currently only one context, so have it always current
   mWindow->MakeContextCurrent();
   mRenderer = std::make_unique<NaiveRenderer>();

   camera = new Camera({0.0f, 0.0f, 3.0f}, 65.0f, 800.0f / 600.0f);

   // Testbed for map loading
   // Successfully loaded binary data into engine.
   SerializableData data = SerializeFromDisk("output.trmap");
   for (auto &entity : data) {
      auto brushes = entity.geo.value_or(std::vector<std::vector<Face>>());
      for (auto &brush : brushes) {
         for (auto &face : brush) { mRenderer->AddMapFace(face); }
      }
   }

   PxFoundation *mFoundation =
       PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gCallback);
}

void Engine::Run() {
   camera = new Camera({0.0f, 0.0f, 3.0f}, 65.0f, 800.0f / 600.0f);
   while (!mWindow->RequestedClose()) {
      glfwSwapBuffers(mWindow->GetHandle());
      mWindow->PollEvents();
      ProcKeys();

      mRenderer->Draw(*camera);
      camera->Tick(0.3666f);
   }

   DLOG_F(INFO, "Performing an orderly shutdown");
}