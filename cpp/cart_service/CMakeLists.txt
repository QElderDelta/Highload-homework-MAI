add_executable(cart_service
        include/request_handler_factory.h
        include/service_initializer.h
        src/request_handler_factory.cpp
        src/service_initializer.cpp
        main.cpp src/cart_base.cpp include/cart_base.h src/cart.cpp include/cart.h)

target_link_libraries(cart_service PRIVATE utils)
target_include_directories(cart_service PRIVATE include)