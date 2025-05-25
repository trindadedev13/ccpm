#include "PackageSearcher.hpp"

#include <iostream>
#include <optional>
#include <vector>

#include "Package.hpp"

namespace ccpm {

namespace searcher {

std::optional<Package> Search(const std::vector<Package>& packages,
                              const std::string& packageName) {
  for (const auto& p : packages) {
    if (p.name.contains(packageName))
      return p;
  }
  return std::nullopt;
}

// i used char** bcz of argv
void SearchAll(const std::vector<Package>& packages, char** packagesNames) {
  for (int i = 2; packagesNames[i] != nullptr; ++i) {
    bool foundAny = false;
    for (const auto& pkg : packages) {
      if (pkg.name.contains(packagesNames[i])) {
        std::cout << "Package Found: " << pkg.name << std::endl;
        foundAny = true;
      }
    }
    if (!foundAny) {
      std::cout << "Package Not found: " << packagesNames[i] << std::endl;
    }
  }
}

}  // namespace searcher

}  // namespace ccpm