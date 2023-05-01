add_library(utils
        include/auth_data.h
        include/authentication_request.h
        include/db_config.h
        include/db_session_manager.h
        include/default_request_handler.h
        include/healthcheck_handler.h
        include/http_request_common.h
        include/http_web_server.h
        include/i_service_initializer.h
        src/auth_data.cpp
        src/authentication_request.cpp
        src/db_config.cpp
        src/db_session_manager.cpp
        src/default_request_handler.cpp
        src/healthcheck_handler.cpp
        src/http_request_common.cpp)

target_include_directories(utils PUBLIC include)
target_link_libraries(utils PUBLIC Poco::Poco)