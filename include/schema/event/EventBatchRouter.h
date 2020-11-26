/*
 * Copyright 2020-2020 Exactpro (Exactpro Systems Limited)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include "schema/message/impl/rabbitmq/AbstractRabbitMessageRouter.h"
#include "schema/event/EventBatchQueue.h"

namespace th2::common_cpp {

class EventBatchRouter : public AbstractRabbitMessageRouter<EventBatch> {
public:
    ~EventBatchRouter() = default;

protected:

    message_queue_ptr<EventBatch> create_queue(connection_manager_ptr connection_manager, queue_configuration_ptr queue_configuration) override {
        message_queue_ptr<EventBatch> event_batch_queue = std::make_shared<EventBatchQueue>();
        event_batch_queue->init(connection_manager, queue_configuration);
        return event_batch_queue;
    };

    typename AbstractRabbitMessageRouter<EventBatch>::alias_and_message_to_send_t find_by_filter(const queues_map_t& queues_map, const EventBatch& msg) override {
        typename AbstractRabbitMessageRouter<EventBatch>::alias_and_message_to_send_t result;

        for (const auto& [alias, configuration] : queues_map) {
            result.emplace(alias,msg);
        }
        return result;
    }

    queue_attributes_t required_subscribe_attributes() override {
        return REQUIRED_SUBSCRIBE_ATTRIBUTES;
    }

    queue_attributes_t required_send_attributes() override {
        return REQUIRED_SEND_ATTRIBUTES;
    }

private:

    queue_attributes_t REQUIRED_SUBSCRIBE_ATTRIBUTES {"event", "subscribe"};
    queue_attributes_t REQUIRED_SEND_ATTRIBUTES {"event", "publish"};
};

}


