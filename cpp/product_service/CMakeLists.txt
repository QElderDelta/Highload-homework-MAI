add_executable(product_service
        include/product.h
        include/product_base.h
        include/product_validator.h
        include/request_handler_factory.h
        include/service_initializer.h
        src/product.cpp
        src/product_base.cpp
        src/product_validator.cpp
        src/request_handler_factory.cpp
        src/service_initializer.cpp
        main.cpp)

target_link_libraries(product_service PRIVATE utils)
target_include_directories(product_service PRIVATE include)