#include "kafka_config.h"

const cppkafka::Configuration& KafkaConfig::get() {
    static KafkaConfig config;
    return config.config;
}

const std::string& KafkaConfig::getTopic() {
    const static std::string topic = std::getenv("KAFKA_TOPIC");
    return topic;
}

KafkaConfig::KafkaConfig() : config({{"metadata.broker.list", std::getenv("KAFKA_CONNECTION_STR")},
                                     {"enable.auto.commit",   false},
                                     {"group.id",             "0"}}) {}
