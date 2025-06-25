cmake_minimum_required(VERSION 3.10)
project(fjgk)

find_package(PkgConfig REQUIRED)
pkg_check_modules(CURL REQUIRED libcurl)
find_package(OpenSSL REQUIRED)

add_executable(fjgk main.cpp)
target_link_libraries(fjgk ${CURL_LIBRARIES} OpenSSL::Crypto)
target_include_directories(fjgk PRIVATE ${CURL_INCLUDE_DIRS})