add_executable(auth_service
        include/authentication_common.h
        include/request_handler_factory.h
        include/service_initializer.h
        include/user.h
        include/user_base.h
        include/user_validator.h
        src/authentication_common.cpp
        src/request_handler_factory.cpp
        src/service_initializer.cpp
        src/user.cpp
        src/user_base.cpp
        src/user_validator.cpp
        main.cpp)

target_link_libraries(auth_service PRIVATE utils)
target_include_directories(auth_service PRIVATE include)