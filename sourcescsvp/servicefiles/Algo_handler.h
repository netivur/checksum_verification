#pragma once
#include "Ciclyc_red.h"
#include "json.hpp"
#include <filesystem>

class Algo_handler {
public:
    Algo_handler()
    {
        algo_name = "crc32";
        ptr_to_funct = crc_f::algo_map[algo_name];
    }

    Algo_handler(const std::string& function_name)
    {
        algo_name = function_name;
        ptr_to_funct = crc_f::algo_map[algo_name];
    }
    void set_new_algo(const std::string& function_name)
    {
        algo_name = function_name;
        ptr_to_funct = crc_f::algo_map[algo_name];
    }

    bool use_method(const std::string& address, const std::string& checksum)
    {
        bool res = ptr_to_funct(address, checksum);
        return res;
    }

	Algo_handler(const Algo_handler& ah) = delete;
	const Algo_handler& operator=(const Algo_handler ah) = delete;

private:
	bool (*ptr_to_funct)(const std::string&, const std::string&) = nullptr;
	std::string algo_name = "";
};
