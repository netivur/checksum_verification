cmake_minimum_required(VERSION 3.15)

project(csvp VERSION 0.1.1.1)
add_library(csvp_compiler_flags INTERFACE)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
target_compile_features(csvp_compiler_flags INTERFACE cxx_std_20)

set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")
target_compile_options(csvp_compiler_flags INTERFACE
    "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused>>"
    "$<${msvc_cxx}:$<BUILD_INTERFACE:-W3>>")

add_executable(csvp maincsvp.cpp)

target_include_directories(csvp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)


target_include_directories(csvp PUBLIC "${PROJECT_BINARY_DIR}")#"${PROJECT_SOURSE_DIR}/servicefiles")
install(TARGETS csvp DESTINATION bin)

#Tests
enable_testing()
#Проверка входа для меньшего или большего числа аргументов
add_test(NAME Usage1 COMMAND csvp)
set_tests_properties(Usage1 PROPERTIES PASS_REGULAR_EXPRESSION "Usage: .* .* .*") #number of arguments does not match

#Пустой файл
add_test(NAME Empfile COMMAND csvp D:\\Gitrep\\test_task\\filesfortests\\globalinput0.json )
set_tests_properties(Empfile PROPERTIES PASS_REGULAR_EXPRESSION "File is empty.")#empty file in input

#Файл, содержащий корректный путь и контрольную сумму
add_test(NAME Correctfile COMMAND csvp D:\\Gitrep\\test_task\\filesfortests\\globalinput1.json )
set_tests_properties(Correctfile PROPERTIES PASS_REGULAR_EXPRESSION ".* No changes in file.*")#descrepancy undetected

#Файл, содержащий путь к пустому файлу
add_test(NAME CorrectEmpFile COMMAND csvp D:\\Gitrep\\test_task\\filesfortests\\globalinput2.json )
set_tests_properties(CorrectEmpFile PROPERTIES PASS_REGULAR_EXPRESSION ".* No changes in file.*")#descrepency undetected

#Файл, содержащий путь к файлу, которого не существует
add_test(NAME NotExistFile COMMAND csvp D:\\Gitrep\\test_task\\filesfortests\\globalinput3.json )
set_tests_properties(NotExistFile PROPERTIES PASS_REGULAR_EXPRESSION "File .* does not exist.")#descrepency undetected

#Файл, содержащий файл с неправильной контрольной суммой
add_test(NAME FileErrorInSum COMMAND csvp D:\\Gitrep\\test_task\\filesfortests\\globalinput4.json )
set_tests_properties(FileErrorInSum PROPERTIES PASS_REGULAR_EXPRESSION ".* There are changes in file")#descrepency undetected
