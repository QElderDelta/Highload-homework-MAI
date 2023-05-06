#include "kafka_config.h"
#include "user.h"
#include "user_base.h"

#include <cppkafka/cppkafka.h>

#include <csignal>
#include <iostream>

bool isRunning = true;

int main() {
    signal(SIGINT, [](int) { isRunning = false; });

    UserBase::initialize();

    try {
        cppkafka::Consumer consumer(KafkaConfig::get());

        consumer.subscribe({KafkaConfig::getTopic()});

        while (isRunning) {
            cppkafka::Message msg = consumer.poll();

            if (!msg) {
                continue;
            }

            if (msg.get_error()) {
                if (!msg.is_eof()) {
                    std::cerr << "Kafka consumer got error: " << msg.get_error() << std::endl;
                }

                continue;
            }

            const auto payload = std::string(msg.get_payload());

            consumer.commit(msg);

            UserBase::registerUser(User::fromJson(payload));
        }
    } catch (const std::exception& e) {
        std::cerr << "Kafka consumer caught exception: " << e.what() << std::endl;
    }
}
