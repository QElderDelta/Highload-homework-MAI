cmake_minimum_required(VERSION 3.20)

project(HighloadHW CXX)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})

message(${CMAKE_MODULE_PATH})

include(cmake/conan.cmake)
set(CMAKE_CXX_STANDARD 17)

if (MSVC)
    add_compile_options(/W4 /WX)
else ()
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif ()

conan_cmake_configure(REQUIRES
        poco/1.12.4 redis-plus-plus/1.3.7
        cppkafka/0.4.0
        GENERATORS cmake_find_package)

conan_cmake_install(PATH_OR_REFERENCE .
        BUILD missing
        REMOTE conancenter)

find_package(Poco)
find_package(redis++)
find_package(CppKafka)

add_subdirectory(utils)

add_subdirectory(auth_service)
add_subdirectory(product_service)
add_subdirectory(cart_service)
