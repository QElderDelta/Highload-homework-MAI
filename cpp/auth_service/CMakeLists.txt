add_executable(auth_service
        include/authentication_common.h
        include/kafka_config.h
        include/request_handler_factory.h
        include/service_initializer.h
        include/user.h
        include/user_base.h
        include/user_cache.h
        include/user_validator.h
        src/authentication_common.cpp
        src/kafka_config.cpp
        src/request_handler_factory.cpp
        src/service_initializer.cpp
        src/user.cpp
        src/user_base.cpp
        src/user_cache.cpp
        src/user_validator.cpp
        main.cpp)

target_link_libraries(auth_service PRIVATE utils redis++::redis++ CppKafka::CppKafka)
target_include_directories(auth_service PRIVATE include)

add_executable(kafka_consumer
        include/kafka_config.h
        include/user.h
        include/user_base.h
        src/kafka_config.cpp
        src/user.cpp
        src/user_base.cpp
        kafka_consumer.cpp)
target_link_libraries(kafka_consumer PRIVATE utils CppKafka::CppKafka)
target_include_directories(kafka_consumer PRIVATE include)