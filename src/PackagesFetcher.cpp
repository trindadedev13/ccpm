#include "PackagesFetcher.hpp"

#include <iostream>
#include <memory>
#include <sstream>
#include <vector>

#include "Package.hpp"
#include "Urls.hpp"

#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/cURLpp.hpp>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace ccpm {

namespace fetcher {

std::string SafeGetString(const nlohmann::json& obj, const std::string& key) {
  if (!obj.contains(key) || !obj[key].is_string()) {
    return "";
  }
  return obj[key].get<std::string>();
}

std::vector<Package> Fetch() {
  try {
    curlpp::Cleanup cleanup{};
    curlpp::Easy request{};

    auto response = std::make_unique<std::ostringstream>();

    request.setOpt<curlpp::options::Url>(PACKAGES_URL);
    request.setOpt<curlpp::options::UserAgent>(
        "Curl/8.13.0 GNU/Linux Ubuntu x86_64");
    request.setOpt<curlpp::options::WriteStream>(response.get());

    request.perform();

    json data = json::parse(response->str());

    std::vector<Package> packages = {};

    for (const auto& item : data) {
      std::string name = SafeGetString(item, "name");
      std::string repository = SafeGetString(item, "repository");
      std::string gitTag = SafeGetString(item, "git_tag");
      std::string cmakeFlags = SafeGetString(item, "cmake_flags");

      if (name.empty() || repository.empty() || gitTag.empty() ||
          cmakeFlags.empty()) {
        std::cerr << "Skipping invalid entry\n";
        continue;
      }
      packages.push_back({name, repository, gitTag, cmakeFlags});
    }

    return packages;
  } catch (const curlpp::RuntimeError& e) {
    std::cerr << "Runtime error: " << e.what() << std::endl;
  } catch (const curlpp::LogicError& e) {
    std::cerr << "Logic error: " << e.what() << std::endl;
  }
  return {};
}

}  // namespace fetcher

}  // namespace ccpm