#include "PackageInstaller.hpp"

#include <filesystem>
#include <iostream>
#include <string>

#include <boost/process.hpp>

#include "Package.hpp"

namespace bp = boost::process;
namespace fs = std::filesystem;

namespace ccpm {

namespace installer {

// Download, compile and install package
void Install(const Package& package) {
  Download(package);
  // todo compile and install
}

// Download package.
// Clones the package with git.
// Uses boost process to access git clone
void Download(const Package& package) {
  try {
    std::string cloneDir = package.name;
    if (fs::exists(cloneDir))
      fs::remove_all(cloneDir);

    std::string gitPath = bp::search_path("git").string();
    bp::child cloneProcess(gitPath, "clone", "--branch", package.gitTag,
                           "--depth", "1", package.repository, cloneDir);

    cloneProcess.wait();

    if (cloneProcess.exit_code() == 0) {
      std::cout << "Cloned successfully: " << package.repository << std::endl;
    } else {
      std::cerr << "Failed to clone: " << package.repository << std::endl;
    }

  } catch (const std::exception& e) {
    std::cerr << "Failed to clone: " << e.what() << std::endl;
  }
}

}  // namespace installer

}  // namespace ccpm