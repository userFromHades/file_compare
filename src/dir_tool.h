#ifndef DIR_TOOL_H
#define DIR_TOOL_H

#include <unordered_map>
#include <vector>
#include <utility>
#include <functional>

#include <experimental/filesystem>
namespace fs = std::experimental::filesystem::v1;


std::vector<fs::path> findAllFiles (const std::string &dirName);

std::unordered_multimap<uint32_t, fs::path>
          calcCrcMt (const std::vector<fs::path>& paths,
                     std::function<void(uint32_t)> &onCount,
                     const uint32_t &numOfThread = 1);

std::vector<std::pair<fs::path,fs::path>>
    findTheSameCrcPairs (const std::unordered_multimap<uint32_t, fs::path>& a,
                    const std::unordered_multimap<uint32_t, fs::path>& b);

bool compareFiles (const fs::path& a, const fs::path& b, const size_t maxBufferSize = 1024*1024);

#endif //DIR_TOOL_H
