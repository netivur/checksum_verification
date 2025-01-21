#pragma once
#include <unordered_map>
#include <string>
namespace crc_f {
	unsigned long CRC32_function(unsigned char* buf, unsigned long len);
	unsigned long CRC32_count(const std::string& filename);
	bool check_sum_crc32(const std::string& address, const std::string& expect_res);
	
	extern std::unordered_map<std::string, bool(*)(const std::string&, const std::string&)> algorithms_map;
}
