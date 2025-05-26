#pragma once

#include <string>

namespace ccpm {

struct Package {
  std::string name;
  std::string repository;
  std::string gitTag;
  std::string cmakeFlags;
};

}  // namespace ccpm