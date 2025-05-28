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

static std::string gitBin = bp::search_path("git").string();
static std::string cmakeBin = bp::search_path("cmake").string();
static std::string makeBin = bp::search_path("make").string();

// Download package.
// Clones the package with git.
// Uses boost process to access git clone
void Download(const Package& package, const std::string& cloneDir) {
  try {
    bp::child cloneProcess(gitBin, "clone", "--branch", package.gitTag,
                           "--depth", "1", package.repository, cloneDir);
    cloneProcess.wait();

    if (cloneProcess.exit_code() == 0) {
      std::cout << "Cloned successfully: " << package.repository << std::endl;
    } else {
      std::cerr << "Failed to clone: " << package.repository << std::endl;
    }

  } catch (const std::exception& e) {
    std::cerr << "Failed to clone: " << e.what() << std::endl;
    std::exit(EXIT_FAILURE);
  }
}

// Compiles the package with CMake & Makefile.
// Uses boost process to access cmake & make.
void Compile(const Package& package) {
  try {
    // gets the env to install lib
    bp::environment env = boost::this_process::environment();
    std::string homeEnv = env["HOME"].to_string();

    std::string packagePath = fs::current_path().string() + "/" + package.name;
    std::string packageBuildPath = packagePath + "/build";

    // Run CMake
    std::vector<std::string> args = {
        "..", "-DCMAKE_INSTALL_PREFIX=" + homeEnv + "/../usr"};
    std::istringstream iss(package.cmakeFlags);
    std::string flag;
    while (iss >> flag) {
      args.push_back(flag);
    }
    bp::child cmakeProcess(cmakeBin, bp::args = args,
                           bp::start_dir = packageBuildPath);
    cmakeProcess.wait();
    if (cmakeProcess.exit_code() == EXIT_SUCCESS) {
      std::cout << "Cmake build configuration successful." << std::endl;
    } else {
      std::cerr << "Failed to create cmake config files with cmake process "
                   "for package: "
                << package.name << std::endl;
      std::exit(EXIT_FAILURE);
    }

    // Run make (finally build)
    bp::child makeProcess(makeBin, bp::start_dir = packageBuildPath);
    makeProcess.wait();
    if (makeProcess.exit_code() == EXIT_SUCCESS) {
      std::cout << "Make build successfully!" << std::endl;
    } else {
      std::cerr << "Failed to build " << package.name << " with Makefile."
                << std::endl;
      std::exit(EXIT_FAILURE);
    }
  } catch (const std::exception& e) {
    std::cerr << "Failed to build " << package.name
              << "with cmake: " << e.what() << std::endl;
  }
}

// Download, compile and install package
void Install(const Package& package) {
  try {
    std::string packagePath = fs::current_path().string() + "/" + package.name;
    std::string packageBuildPath = packagePath + "/build";

    // delete clone to avoid cache errors or something like it
    std::string cloneDir = package.name;
    if (fs::exists(cloneDir))
      fs::remove_all(cloneDir);

    // create clone dir and build path for compile
    if (!fs::exists(packageBuildPath))
      fs::create_directories(packageBuildPath);

    Download(package, cloneDir);
    Compile(package);

    // Run CMake Install (finally step)
    bp::child installProcess(cmakeBin, "--install", ".",
                             bp::start_dir = packageBuildPath);
    installProcess.wait();
    if (installProcess.exit_code() == EXIT_SUCCESS) {
      std::cout << "Installed successfully!!!" << std::endl;
    } else {
      std::cerr << "Failed to install package " << package.name << std::endl;
      std::exit(EXIT_FAILURE);
    }
  } catch (const std::exception& e) {
    std::cerr << "Failed to install " << package.name << ": " << e.what()
              << std::endl;
  }
}

}  // namespace installer

}  // namespace ccpm