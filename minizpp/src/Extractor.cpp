#include "Minizpp/Extractor.hpp"

#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace minizpp {

Extractor::Extractor(const std::string _zipFilePath)
    : zipFilePath{std::move(_zipFilePath)}, zipArchive{} {
  if (!mz_zip_reader_init_file(&zipArchive, zipFilePath.c_str(), 0)) {
    throw std::runtime_error("Failed to open zip file: " + zipFilePath);
  }
}

Extractor::~Extractor() {
  mz_zip_reader_end(&zipArchive);
}

std::vector<std::string> Extractor::ListFiles() {
  std::vector<std::string> files{};
  int count = (int)mz_zip_reader_get_num_files(&zipArchive);
  for (int i = 0; i < count; ++i) {
    mz_zip_archive_file_stat stat;
    if (mz_zip_reader_file_stat(&zipArchive, i, &stat)) {
      files.emplace_back(stat.m_filename);
    }
  }
  return files;
}

void Extractor::ExtractTo(const std::string outPath) {
  namespace fs = std::filesystem;
  int count = (int)mz_zip_reader_get_num_files(&zipArchive);
  for (int i = 0; i < count; ++i) {
    mz_zip_archive_file_stat stat;
    if (!mz_zip_reader_file_stat(&zipArchive, i, &stat))
      continue;
    std::string filePath = outPath + "/" + stat.m_filename;
    if (mz_zip_reader_is_file_a_directory(&zipArchive, i)) {
      fs::create_directories(filePath);
    } else {
      fs::create_directories(fs::path(filePath).parent_path());
      if (!mz_zip_reader_extract_to_file(&zipArchive, i, filePath.c_str(), 0)) {
        throw std::runtime_error("Failed to extract: " + filePath);
      }
    }
  }
}

}  // namespace minizpp