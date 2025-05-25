#include <iostream>
#include <vector>

#include "Inline.h"
#include "Package.hpp"
#include "PackageInstaller.hpp"
#include "PackageSearcher.hpp"
#include "PackagesFetcher.hpp"

namespace ccpm {

CCPM_INLINE static void PrintHelp() {
  std::cout << "Usage:\n"
            << "  install | -i     Install packages\n"
            << "  uninstall | -u   Uninstall packages\n"
            << "  search | -s      Search packages" << std::endl;
}

}  // namespace ccpm

int main(int argc, char** argv) {
  if (argc < 2) {
    ccpm::PrintHelp();
    return EXIT_FAILURE;
  }

  std::vector<ccpm::Package> packages{ccpm::fetcher::Fetch()};

  std::string command = argv[1];

  if (command == "install" || command == "-i") {
    for (int i = 2; argv[i] != nullptr; ++i) {
      if (auto pkg = ccpm::searcher::Search(packages, argv[i]);
          pkg.has_value()) {
        std::cout << "Package found! Downloading..." << std::endl;
        ccpm::installer::Install(pkg.value());
      } else {
        std::cout << "Package not found: " << argv[i] << std::endl;
      }
    }
  } else if (command == "uninstall" || command == "-u") {
    // todo
  } else if (command == "search" || command == "-s") {
    std::cout << "Searching...." << std::endl;
    ccpm::searcher::SearchAll(packages, argv);
  } else {
    ccpm::PrintHelp();
  }
  return EXIT_SUCCESS;
}