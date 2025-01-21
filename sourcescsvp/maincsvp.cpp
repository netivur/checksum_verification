#include <iostream>
#include <vector>
#include <memory>
#include <fstream>
#include "json_structure_handler.h"

int main(int argc, char* argv[]) {   
    if (jsh::correct_input(argc, argv)) {
        try {
            jsh::json jsonarr;
            std::ifstream inputstream;            
            inputstream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            inputstream.open(argv[1]);
            inputstream >> jsonarr;
            inputstream.close();

            std::unique_ptr< std::vector<jsh::Sum_n_path>> uptr_to_vp = std::make_unique<std::vector<jsh::Sum_n_path>>();
            *uptr_to_vp = jsonarr;

            jsh::Json_struct_handler ah("crc32");
            jsh::set_algorithm(ah, argc, argv);
            
            for (auto it = uptr_to_vp->begin(); it != uptr_to_vp->end(); ++it) {
                try {
                    if (jsh::the_file_exists((it->path_).c_str())) {
                        bool result = ah.use_method(it->path_, it->sum_);
                        std::cout << "File: " << it->path_ << "\n";
                        if (result) {
                            std::cout << "Test results: No changes in file" << std::endl;
                        }
                        else {
                            std::cout << "Test results: There are changes in file" << std::endl;
                        }
                        std::cout << std::endl;
                    }
                }
                catch (std::ifstream::failure& e) {
                    std::cerr<<e.what()<< '\n';
                }
            }            
        }
        catch (std::length_error& le) {
            std::cerr << le.what() << '\n';
            return EXIT_FAILURE;
        }   
        catch (std::bad_alloc& bae) {
            std::cerr << bae.what() << '\n';
            return EXIT_FAILURE;
        }
        catch (std::ifstream::failure& e) {
            std::cerr << e.what() << '\n';
        }
    }
    else {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
