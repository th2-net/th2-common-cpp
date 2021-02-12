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

namespace th2::common_cpp {

template <class MessageType, class MessageBatchType>
class AbstractRabbitBatchMessageRouter : public AbstractRabbitMessageRouter<MessageBatchType> {
public:

protected:
    typename AbstractRabbitMessageRouter<MessageBatchType>::alias_and_message_to_send_t find_by_filter(const queues_map_t& queues_map, const MessageBatchType& batch) override {
        typename AbstractRabbitMessageRouter<MessageBatchType>::alias_and_message_to_send_t result;

        for (auto&& msg : get_messages(batch)) {
            for (auto&& queue_alias : filter(queues_map, msg)) {
                auto it = result.find(queue_alias);
                if (it == std::cend(result)) {
                    auto pair = result.emplace(queue_alias, MessageBatchType{});
                    it = pair.first;
                }

                auto new_msg = add_messages(it->second);
                *new_msg = msg;
            }
        }
        return result;
    }

    std::unordered_set<std::string> filter(const queues_map_t& queues, const MessageType& message) {
        std::unordered_set<std::string> aliases;

        for (const auto& [alias, queue] : queues) {

            auto filters = queue->get_filters();
            //TODO implement filterStrategy
            //if (filters.isEmpty() || filterStrategy.get().verify(message, filters)) {
            if (filters.empty()) {
                aliases.emplace(alias);
            }
        }

        return aliases;
    }

    virtual MessageType* add_messages(MessageBatchType&) const = 0;

    virtual const google::protobuf::RepeatedPtrField<MessageType>& get_messages(const MessageBatchType& batch) const = 0;
};

}
