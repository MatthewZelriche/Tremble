#include "Tremble/core/engine.hpp"

#include "Tremble/io/log.hpp"
#include "Tremble/io/config.hpp"

using namespace TR;

Engine::Engine() {
   TR_INFO("Initializing TrembleCPP Engine and subsystems");
   Config config = Config("engine.toml");

   InitializeSubsystems(config);
}

void Engine::InitializeSubsystems(const Config &config) {
   const Subsystems &subsystems = config.GetParsedSubsystems();
   // Optional subsystems
   if (subsystems.window) { window = Window(config.GetParsedWindowProperties()); }

   // Required subsystems
}

bool Engine::RequestedShutdown() {
   return window.map_or([](auto &window) { return window.WindowRequestedClose(); },
                        false);
}

void Engine::Update(double delta) {
   window.map([](Window &window) { window.Draw(); });
}