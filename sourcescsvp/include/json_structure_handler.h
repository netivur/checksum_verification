#pragma once
#include "crc_functions.h"
#include "json.hpp"

namespace jsh {
    class Json_struct_handler {
    public:    
        Json_struct_handler();    
        Json_struct_handler(const std::string& function_name);
        void set_new_algo(const std::string& function_name);
        bool use_method(const std::string& address, const std::string& checksum);

        Json_struct_handler(const Json_struct_handler& ah) = delete;
	    const Json_struct_handler& operator=(const Json_struct_handler ah) = delete;
        
    private:
	    bool (*ptr_to_funct)(const std::string&, const std::string&) = nullptr;
	    std::string algo_name = "";
    };

    struct Sum_n_path {
        std::string path_;
        std::string sum_;
    };
    using json = nlohmann::json;

    void from_json(const json& j, Sum_n_path& p);
    void from_json(const json& j, std::vector<Sum_n_path>& fv);
    
    bool the_file_exists(const char* filename);
    
    void set_algorithm(Json_struct_handler& ah, const int argc, char** argv);

    bool correct_input(int argc, char** argv);
}
