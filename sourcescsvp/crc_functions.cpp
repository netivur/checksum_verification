#include <fstream>
#include <iostream>
#include "crc_functions.h"

namespace crc_f {
    constexpr unsigned long long buffer_size = 81920;

	std::shared_ptr<unsigned long[]> create_crc32_table() {
		std::shared_ptr<unsigned long[]>crc_table(new unsigned long[256]);
		unsigned long crc;
		for (int i = 0; i < 256; ++i) {
			crc = i;
			for (int j = 0; j < 8; ++j) {
				crc = crc & 1 ? (crc >> 1) ^ 0xEDB88320UL : crc >> 1;
			}				
			crc_table[i] = crc;
		}
		return crc_table;
	}
	unsigned long crc32(unsigned long crc, unsigned char* buffer, unsigned long long size, std::shared_ptr<unsigned long[]> crc_table) {
		while (size--) {
			crc = crc_table[(crc ^ *buffer++) & 0xFF] ^ (crc >> 8);
		}
		return crc;
	}
	std::ifstream::pos_type determine_file_size(const std::string& filename) {
		std::ifstream check_file_size;
		check_file_size.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try {
			check_file_size.open(filename, std::ifstream::ate | std::ifstream::binary);
		}
		catch (const std::ifstream::failure& e) {
			std::cerr << "Failing to open " << filename << '\n';
			throw e;
		}
		std::ifstream::pos_type file_size;
		try {
			file_size = check_file_size.tellg();
		}
		catch (const std::ifstream::failure& e) {
			std::cerr << "In determne_file_size " << e.what() << '\n';
		}
		check_file_size.close();
		return file_size;
	}
	unsigned long calculate_checksum_crc32(const std::string& filename) {
		try {
			unsigned long long file_size = determine_file_size(filename);
			std::shared_ptr<char[]> pbuf(new char[buffer_size]);
			std::shared_ptr<unsigned long[]> crc_table = create_crc32_table();
			unsigned long crc = 0xFFFFFFFFUL;
			std::ifstream current_filesream(filename, std::ios::binary);
			while (file_size >= buffer_size) {
				current_filesream.read(pbuf.get(), buffer_size);
				crc = crc32(crc, (unsigned char*)(pbuf.get()), buffer_size, crc_table);
				file_size -= buffer_size;
			}
			if (file_size > 0) {
				current_filesream.read(pbuf.get(), file_size);
				crc = crc32(crc, (unsigned char*)(pbuf.get()), file_size, crc_table);
				file_size = 0;
			}
			return crc ^ 0xFFFFFFFFUL;
		}
		catch (const std::bad_alloc& alloc_err) {
			std::cerr << "In calculate_checksum_crc32 with file " << filename << " \nerror: " <<
				alloc_err.what() << '\n';
			throw;
		}		
		catch (const std::ifstream::failure& e) {
			std::cerr << "Exception opening/reading file " << filename << '\n' <<  e.what();
			throw;
		}
	}
	bool check_sum_crc32(const std::string& address, const std::string& expected_result) {
		try {
			unsigned long verified_result = std::stoul(expected_result, nullptr, 16);
			unsigned long calculated_result = calculate_checksum_crc32(address);
			return verified_result == calculated_result;
		}
		catch(std::invalid_argument& arg_err){
			std::cerr << "In check_sum_crc32 " << address << " " << arg_err.what() << '\n';
			return false;
		}
		catch (std::out_of_range& range_err) {
			std::cerr << "In check_sum_crc32 " << address << " " << range_err.what() << '\n';
			return false;
		}		
	}
	std::unordered_map<std::string, bool(*)(const std::string&, const std::string&)> algorithms_map = {
		{"crc32", &check_sum_crc32}
	};
}
