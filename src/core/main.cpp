#include <loguru.hpp>
#include <cassert>

int main(int argc, char **argv) {
   loguru::set_thread_name("MAIN");
   // TODO: Log rotating
   loguru::add_file("error_log.txt", loguru::Truncate, loguru::Verbosity_ERROR);
   loguru::add_file("verbose_log.txt", loguru::Truncate, loguru::Verbosity_MAX);

   DLOG_F(INFO, "Hello, world!");
   return 0;
}