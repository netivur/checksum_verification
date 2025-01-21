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
		std::ifstream check_file_length;
		check_file_length.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try {
			check_file_length.open(filename, std::ifstream::ate | std::ifstream::binary);
		}
		catch (std::ifstream::failure& e) {
			std::cerr << "Failing to open " << filename << '\n';
			throw e;
		}
		std::ifstream::pos_type file_size;

		file_size = check_file_length.tellg();
		check_file_length.close();

		char* pbuf = new char[file_size];
		try {
			std::ifstream f;
			try {
				f.open(filename, std::ios::binary);
				f.read(pbuf, file_size);
				unsigned long result = CRC32_function((unsigned char*)pbuf, static_cast<unsigned long>(f.gcount()));
				f.close();
				delete[] pbuf;
				return result;
			}
			catch (std::ifstream::failure& e) {
				std::cerr << "Failing to open " << filename << '\n';
				throw e;
			}
		}
		catch (...) {
			delete[] pbuf;
			throw;
		}
	}
	bool check_sum_crc32(const std::string& address, const std::string& expect_res) {
		unsigned long result_for_ver = std::stoul(expect_res, nullptr, 16);
		unsigned long check_result = CRC32_count(address);

		return result_for_ver == check_result;
	}
	std::unordered_map<std::string, bool(*)(const std::string&, const std::string&)> algorithms_map = {
		{"crc32", &check_sum_crc32}
	};
}
