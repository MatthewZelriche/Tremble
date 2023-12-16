#pragma once

#include <stdexcept>

#include "Tremble/c_api/error/tremble_error.hpp"

namespace TR {

class tremble_exception : public std::runtime_error {
  public:
   tremble_exception(const char *what, TrembleError code) :
       std::runtime_error(what), code(code) {}
   inline TrembleError GetErrorCode() { return code; }

  private:
   TrembleError code;
};

class third_party_init_error : public tremble_exception {
  public:
   third_party_init_error(const char *what) :
       tremble_exception(what, THIRD_PARTY_INIT_ERROR) {}
};

} // namespace TR