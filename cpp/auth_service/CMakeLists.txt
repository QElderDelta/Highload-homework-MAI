add_executable(auth_service
        main.cpp
        ../utils/include/db_config.h
        ../utils/src/db_config.cpp
        ../utils/include/db_session_manager.h
        ../utils/src/db_session_manager.cpp
        include/user.h
        ../utils/include/http_web_server.h
        ../utils/include/i_service_initializer.h
        src/service_initializer.cpp
        include/service_initializer.h
        src/request_handler_factory.cpp
        include/request_handler_factory.h
        src/user_base.cpp include/user_base.h
        src/authentication_common.cpp
        include/authentication_common.h src/user_validator.cpp include/user_validator.h src/user.cpp)

target_link_libraries(auth_service PRIVATE Poco::Poco)
target_include_directories(auth_service PRIVATE ../utils/include include)