#pragma once

#include "Package.hpp"

namespace ccpm {

namespace installer {

void Download(const Package&, const std::string& cloneDir);

void Compile(const Package& package);

void Install(const Package&);

}  // namespace installer

}  // namespace ccpm