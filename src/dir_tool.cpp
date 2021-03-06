#include "dir_tool.h"

#include <queue>
#include <mutex>
#include <atomic>
#include <thread>
#include <fstream>
#include <memory>

#include "crc32.h"

uint32_t calcCrc32ForFile (const fs::path &path, const size_t &maxBufferSize = 1024*1024)
{
	auto size = fs::file_size(path);

	auto buffer = std::unique_ptr<uint8_t[]>{new uint8_t[maxBufferSize]};

	//path.generic_u8string()
	auto f = std::ifstream {path.u8string (), std::fstream::binary};

	//if (f.is_open())
	//	throw 0;

	uint32_t crc = 0;
	while (size != 0){

		size_t curretSize = size <= maxBufferSize ? size : maxBufferSize;

		f.read (reinterpret_cast<char*>(buffer.get()), curretSize);
		crc = crc32_bitwise(buffer.get(), curretSize, crc);

		size -= curretSize;
	}

	return crc;

}

std::vector<fs::path> findAllFiles (const std::string &dirName){

	auto output = std::vector<fs::path>{};

	std::queue<fs::path> dirs;
	//if (fs::is_symlink(fs::path{dirName}))
	dirs.push(fs::path{dirName});

	while (not dirs.empty()){

		const auto p = dirs.front();

		for (auto& x : fs::directory_iterator(p)){
			if (fs::is_regular_file(x))
				output.push_back(x.path());
			else if (fs::is_directory(x))
				dirs.push(x);
		}

		dirs.pop();
	}

	return output;
}

std::unordered_multimap<uint32_t, fs::path>
        calcCrcMt (const std::vector<fs::path>& paths,
                   std::function<void(uint32_t)>& onCount,
                   const uint32_t &numOfThread)
{
	int32_t namesPerThread = paths.size() / numOfThread;
	if (namesPerThread == 0)
		namesPerThread = 1;

	std::mutex mutex;
	std::unordered_multimap<uint32_t, fs::path> result;
	uint32_t counter = 0;
	std::vector<std::thread> threadPool;

	for (int i = 0; i < numOfThread and i < paths.size(); i++){

		auto begin = std::begin(paths) + i       * namesPerThread;
		auto end   = std::begin(paths) + (i + 1) * namesPerThread;//Todo

		threadPool.emplace_back(std::thread([&, begin, end](){

			for (auto it = begin; it < end; it++){
				const auto crc = calcCrc32ForFile (*it);

				{
					auto lock = std::lock_guard{mutex};
					result.insert(std::make_pair(crc, *it));
					counter++;
					onCount(counter);
				}


			}

		}));
	}

	for (auto &l : threadPool)
		l.join();

	return result;
}

std::vector<std::pair<fs::path,fs::path>>
    findTheSameCrcPairs (const std::unordered_multimap<uint32_t, fs::path>& a,
                    const std::unordered_multimap<uint32_t, fs::path>& b)
{
	if (a.size() > b.size())
		return findTheSameCrcPairs(b, a);

	std::vector<std::pair<fs::path,fs::path>> output;

	for (auto& [key, value] : a){
		auto range = b.equal_range(key);
		for (auto it = range.first; it != range.second; ++it)
		{
			const auto c = std::make_pair (value, it->second);
			output.push_back(c);
		}
	}

	return output;
}

bool compareFiles (const fs::path& a, const fs::path& b, const size_t maxBufferSize){
	if (fs::file_size(a) != fs::file_size(b))
		return false;

	auto size = fs::file_size(a);

	auto buffer_a = std::unique_ptr<uint8_t[]>{new uint8_t[maxBufferSize]};

	auto buffer_b = std::unique_ptr<uint8_t[]>{new uint8_t[maxBufferSize]};

	auto f_a = std::ifstream {a.u8string (), std::fstream::binary};
	if (not f_a.is_open())
		return false;

	auto f_b = std::ifstream {b.u8string (), std::fstream::binary};
	if (not f_b.is_open())
		return false;

	while (size != 0){

		size_t curretSize = size <= maxBufferSize ? size : maxBufferSize;

		f_a.read (reinterpret_cast<char*>(buffer_a.get()), curretSize);
		f_b.read (reinterpret_cast<char*>(buffer_b.get()), curretSize);

		for (int i = 0; i < curretSize; i++)
			if (buffer_a[i] != buffer_b[i])
				return false;

		size -= curretSize;
	}

	return true;

}

void compareFilesMt(const std::vector<std::pair<fs::path,fs::path>> pairs,
                    std::function<void(bool, uint32_t count ,std::pair<fs::path,fs::path>)>& onCompare,
                    const uint32_t &numOfThread)
{
	int32_t namesPerThread = pairs.size() / numOfThread;
	if (namesPerThread == 0)
		namesPerThread = 1;

	std::mutex mutex;
	uint32_t counter = 0;
	std::vector<std::thread> threadPool;

	for (int i = 0; i < numOfThread and i < pairs.size(); i++){

		auto begin = std::begin(pairs) + i       * namesPerThread;
		auto end   = std::begin(pairs) + (i + 1) * namesPerThread;//Todo

		threadPool.emplace_back(std::thread([&, begin, end](){

			for (auto it = begin; it < end; it++){
				const auto equal = compareFiles(it->first,it->second);

				{
					auto lock = std::lock_guard{mutex};
					counter++;
					onCompare(equal, counter, *it);
				}

			}

		}));
	}

	for (auto &l : threadPool)
		l.join();

}
