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
namespace sver {
    struct Sum_n_path {
        std::string path_;
        std::string sum_;
    };
    using json = nlohmann::json;

    void from_json(const json& j, Sum_n_path& p) {
        j.at("filename").get_to(p.path_);
        j.at("sum").get_to(p.sum_);
    }
    void from_json(const json& j, std::vector<Sum_n_path>& fv) {
        const json& value = j.at("Files");
        fv.resize(value.size());
        std::copy(value.begin(), value.end(), fv.begin());
    }
    
    bool the_file_exists(const char* filename) {
        std::ifstream inputstream(filename);
        if (!inputstream.is_open()) {
            std::cout << "File " << filename << " does not exist." << std::endl;
            return false;
        }
        inputstream.close();
        return true;
    }
    
    void set_algorithm(Algo_handler& ah, const int argc, char** argv) {
        if (argc == 3) {
            std::string name_algo = argv[2];
            std::transform(name_algo.begin(), name_algo.end(), name_algo.begin(),
                [](unsigned char c) { return std::tolower(c); });
            ah.set_new_algo(name_algo);
        }
    }

    bool correct_input(int argc, char** argv) {
        bool result{ true };
        if ((argc < 2) || (argc > 3)) {
            std::cout << "Usage: " << argv[0] << " database_file checksum_algorithm(CRC32 by default)" << std::endl;
            result = false;
        }
        if (result && (argc == 3)) {
            std::string name_algo = argv[2];
            std::transform(name_algo.begin(), name_algo.end(), name_algo.begin(),
                [](unsigned char c) { return std::tolower(c); });
            if (!crc_f::algo_map.contains(name_algo)) {
                std::cout << name_algo << " not Supported.\n" << "Supported algorythms: \n";
                for (const auto& p : crc_f::algo_map) {
                    std::cout << p.first << " ";
                }
                std::cout << std::endl;
                result = false;
            }
        }

        std::filesystem::path path_to_file = argv[1];
        if (result && ((path_to_file.extension() != ".json") && (path_to_file.extension() != ".JSON"))) {
            std::cout << path_to_file.filename() << " is an invalid type." << std::endl;
            result = false;
        }

        if (result) {
            result = the_file_exists(argv[1]);
            std::ifstream inputstream(argv[1]);

            if (result) {
                result = (inputstream.peek() == std::ifstream::traits_type::eof()) ? false : true;
                if (!result) {
                    std::cout << "File is empty." << std::endl;
                    result = false;
                }
            }
            inputstream.close();
        }

        return result;
    }
}