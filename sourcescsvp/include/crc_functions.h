#pragma once
#include <unordered_map>
#include <string>
namespace crc_f {
    std::shared_ptr<unsigned long[]> create_crc32_table();
	unsigned long crc32(unsigned long crc, unsigned char* buffer, unsigned long long size, std::shared_ptr<unsigned long[]> crc_table);
	std::ifstream::pos_type determine_file_size(const std::string& filename);
	unsigned long calculate_checksum_crc32(const std::string& filename);	
	bool check_sum_crc32(const std::string& address, const std::string& expect_res);
	extern std::unordered_map<std::string, bool(*)(const std::string&, const std::string&)> algorithms_map;
}
