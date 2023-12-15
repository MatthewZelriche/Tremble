#include "Tremble/core/engine.hpp"

#include "Tremble/io/log.hpp"
#include "Tremble/io/config.hpp"

using namespace TR;

Engine::Engine() {
   TR_INFO("Initializing TrembleCPP Engine and subsystems");
   Config config = Config("engine.toml");

   InitializeSubsystems(config.GetParsedSubsystems());
}

void Engine::InitializeSubsystems(const Subsystems &subsystemsConfig) {
   // Optional subsystems
   if (subsystemsConfig.window) { window = Window(); }
   window.map([](auto window) { window.Foo(); });
   if (window) { window.value().Foo(); }

   // Required subsystems
}

void Engine::Update(double delta) {}