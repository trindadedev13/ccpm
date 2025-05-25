#include "PackageInstaller.hpp"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <Minizpp/Extractor.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/cURLpp.hpp>

#include "Package.hpp"

namespace ccpm {

namespace installer {

// Download extract and install package
void Install(const Package& package) {
  Download(package);
  std::string zipFile = package.name + ".zip";
  if (std::filesystem::exists(zipFile)) {
    minizpp::Extractor extractor{zipFile};
    std::vector<std::string> files = extractor.ListFiles();
    std::cout << "Files inside: " << zipFile << std::endl;
    for (int i = 0; i < files.size(); ++i) {
      std::cout << files[i] << std::endl;
    }
    extractor.ExtractTo(package.name);
  }
}

// Download package from URL.
void Download(const Package& package) {
  try {
    curlpp::Cleanup cleanup{};

    std::ofstream outFile{package.name + ".zip", std::ios::binary};

    curlpp::Easy request{};
    request.setOpt<curlpp::options::Url>(package.url);
    request.setOpt<curlpp::options::WriteStream>(&outFile);
    request.setOpt<curlpp::options::FollowLocation>(true);
    request.perform();

  } catch (const curlpp::RuntimeError& e) {
    std::cerr << "Runtime error: " << e.what() << std::endl;
  } catch (const curlpp::LogicError& e) {
    std::cerr << "Logic error: " << e.what() << std::endl;
  }
}

}  // namespace installer

}  // namespace ccpm