#pragma once

#include <string>

#include "BuildType.hpp"

namespace ccpm {

struct Package {
  std::string name;
  std::string repository;
  std::string gitTag;
  buildtype::BuildType buildType;
};

}  // namespace ccpm