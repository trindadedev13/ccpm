#pragma once

#include <string>
#include <vector>

#include <miniz/miniz.h>

namespace minizpp {

class Extractor {
 public:
  Extractor(const std::string zipFilePath);
  ~Extractor();

  std::vector<std::string> ListFiles();
  void ExtractTo(const std::string outDir);

 private:
  std::string zipFilePath;
  mz_zip_archive zipArchive;
};

}  // namespace minizpp