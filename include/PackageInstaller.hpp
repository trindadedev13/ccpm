#pragma once

#include "Package.hpp"

namespace ccpm {

namespace installer {

void Install(const Package&);

void Download(const Package&);

}  // namespace installer

}  // namespace ccpm