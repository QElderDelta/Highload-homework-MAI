#pragma once

#include <cppkafka/cppkafka.h>

class KafkaConfig {
public:
    static const cppkafka::Configuration& get();

    static const std::string& getTopic();

private:
    KafkaConfig();

    const cppkafka::Configuration config;
};
