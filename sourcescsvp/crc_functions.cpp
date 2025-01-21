#include <fstream>
#include <iostream>
#include "crc_functions.h"

namespace crc_f {
	unsigned long CRC32_function(unsigned char* buf, unsigned long len) {
		try {
			std::unique_ptr<unsigned long[]>crc_table(new unsigned long[256]);

			unsigned long crc;
			for (int i = 0; i < 256; ++i) {
				crc = i;
				for (int j = 0; j < 8; ++j)
					crc = crc & 1 ? (crc >> 1) ^ 0xEDB88320UL : crc >> 1;
				crc_table[i] = crc;
			};
			crc = 0xFFFFFFFFUL;
			while (len--)
				crc = crc_table[(crc ^ *buf++) & 0xFF] ^ (crc >> 8);
			return crc ^ 0xFFFFFFFFUL;
		}
		catch (const std::bad_alloc& ex) {
			std::cerr << ex.what() << '\n';
			throw;
		}
	}
	unsigned long CRC32_count(const std::string& filename) {
		std::ifstream check_file_size;
		check_file_size.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try {
			check_file_size.open(filename, std::ifstream::ate | std::ifstream::binary);
		}
		catch (std::ifstream::failure& e) {
			std::cerr << "Failing to open " << filename << '\n';
			throw e;
		}
		std::ifstream::pos_type file_size;
		file_size = check_file_size.tellg();
		check_file_size.close();

		try {
			std::unique_ptr<char[]> pbuf(new char[file_size]);
			std::ifstream current_file;
			try {
				current_file.open(filename, std::ios::binary);
				current_file.read(pbuf.get(), file_size);
				unsigned long result = CRC32_function((unsigned char*)(pbuf.get()), static_cast<unsigned long>(current_file.gcount()));
				current_file.close();

				return result;
			}
			catch (std::ifstream::failure& e) {
				std::cerr << "Failing to open " << filename << '\n';
				throw e;
			}
		}
		catch (std::bad_alloc& err) {
			std::cerr << "In crc32_count " << err.what() << '\n';
			throw err;
		}
	}
	bool check_sum_crc32(const std::string& address, const std::string& expected_result) {
		try {
			unsigned long verified_result = std::stoul(expected_result, nullptr, 16);
			unsigned long calculated_result = CRC32_count(address);
			return verified_result == calculated_result;
		}
		catch (std::invalid_argument& arg_err) {
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
