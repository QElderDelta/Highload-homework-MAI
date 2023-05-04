add_executable(cache_service
        include/request_handler_factory.h
        include/service_initializer.h
        include/user_cache.h
        src/request_handler_factory.cpp
        src/service_initializer.cpp
        src/user_cache.cpp
        ../auth_service/include/user.h
        ../auth_service/src/user.cpp
        main.cpp)

target_link_libraries(cache_service PRIVATE utils redis++::redis++)
target_include_directories(cache_service PRIVATE include ../auth_service/include)