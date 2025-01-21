#include <filesystem>
#include <fstream>
#include <iostream>
#include "json_structure_handler.h"

namespace jsh {
	Json_struct_handler::Json_struct_handler() {
        algorithm_name = "crc32";
        ptr_to_funct = crc_f::algorithms_map[algorithm_name];
    }
    Json_struct_handler::Json_struct_handler(const std::string& function_name) {
        algorithm_name = function_name;
        ptr_to_funct = crc_f::algorithms_map[algorithm_name];
    }
    void Json_struct_handler::set_new_algorithm(const std::string& function_name) {
        algorithm_name = function_name;
        ptr_to_funct = crc_f::algorithms_map[algorithm_name];
    }
    bool Json_struct_handler::use_method(const std::string& address, const std::string& checksum) {
        bool res = ptr_to_funct(address, checksum);
        return res;
    }

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

    void set_algorithm(Json_struct_handler& ah, const int argc, char** argv) {
        if (argc == 3) {
            std::string algorithm_name = argv[2];
            std::transform(algorithm_name.begin(), algorithm_name.end(), algorithm_name.begin(),
                [](unsigned char c) { return std::tolower(c); });
            ah.set_new_algorithm(algorithm_name);
        }
    }

    bool correct_input(int argc, char** argv) {
        bool result{ true };
        if ((argc < 2) || (argc > 3)) {
            std::cout << "Usage: " << argv[0] << " database_file checksum_algorithm(CRC32 by default)" << std::endl;
            result = false;
            return result;
        }
        if (result && (argc == 3)) {
            std::string algorithm_name = argv[2];
            std::transform(algorithm_name.begin(), algorithm_name.end(), algorithm_name.begin(),
                [](unsigned char c) { return std::tolower(c); });
            if (!crc_f::algorithms_map.contains(algorithm_name)) {
                std::cout << algorithm_name << " not Supported.\n" << "Supported algorythms: \n";
                for (const auto& p : crc_f::algorithms_map) {
                    std::cout << p.first << " ";
                }
                std::cout << std::endl;
                result = false;
                return result;
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
