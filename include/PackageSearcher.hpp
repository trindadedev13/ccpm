#pragma once

#include <optional>
#include <vector>

#include "Package.hpp"

namespace ccpm {

namespace searcher {

std::optional<Package> Search(const std::vector<Package>& packages,
                              const std::string& packageName);

void SearchAll(const std::vector<Package>& packages, char** packagesNames);

}  // namespace searcher

}  // namespace ccpm