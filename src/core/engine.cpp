#include "engine.hpp"

#include <loguru.hpp>

using namespace TR;

Engine::Engine() {
   // Initialize logging
   loguru::set_thread_name("MAIN");
   // TODO: Log rotating
   loguru::add_file("error_log.txt", loguru::Truncate, loguru::Verbosity_ERROR);
   loguru::add_file("verbose_log.txt", loguru::Truncate, loguru::Verbosity_MAX);
   DLOG_F(INFO, "Successfully initialized logging module.");

   // Create a new window
   mWindow = std::make_unique<Window>();
}

void Engine::Run() {
   while (!mWindow->RequestedClose()) { mWindow->PollEvents(); }

   DLOG_F(INFO, "Performing an orderly shutdown");
}