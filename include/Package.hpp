#pragma once

#include <string>

#include "BuildType.hpp"

namespace ccpm {

struct Package {
  std::string name;
  std::string url;
  buildtype::BuildType buildType;
};

}  // namespace ccpm