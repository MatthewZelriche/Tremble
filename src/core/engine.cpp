#include "Tremble/core/engine.hpp"

#include "Tremble/io/log.hpp"
#include "Tremble/io/config.hpp"

using namespace TR;

Engine::Engine() {
   TR_INFO("Initializing TrembleCPP Engine");
   Config config = Config("engine.toml");
}

void Engine::Update(double delta) {}