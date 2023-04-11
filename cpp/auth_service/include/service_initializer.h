#pragma once

#include <i_service_initializer.h>

class ServiceInitializer : public IServiceInitializer {
public:
    void initialize() override;

private:
    static void createUserTable();
};
