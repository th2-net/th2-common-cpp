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

#include "common.pb.h"
#include "schema/message/impl/rabbitmq/router/AbstractRabbitBatchMessageRouter.h"
#include "schema/message/impl/rabbitmq/group/RabbitMessageGroupBatchQueue.h"

namespace th2::common_cpp {

class RabbitMessageGroupBatchRouter : public AbstractRabbitBatchMessageRouter<MessageGroup, MessageGroupBatch> {
public:

protected:
    message_queue_ptr<MessageGroupBatch> create_queue(connection_manager_ptr connection_manager, queue_configuration_ptr queue_configuration) override {
        message_queue_ptr<MessageGroupBatch> queue = std::make_shared<RabbitMessageGroupBatchQueue>();
        queue->init(connection_manager, queue_configuration);
        return queue;
    }

    [[nodiscard]] queue_attributes_t required_subscribe_attributes() noexcept override {
        return REQUIRED_SUBSCRIBE_ATTRIBUTES;
    }

    [[nodiscard]] queue_attributes_t required_send_attributes() noexcept override {
        return REQUIRED_SEND_ATTRIBUTES;
    }

    [[nodiscard]] MessageGroup* add_messages(MessageGroupBatch& batch) const override {
        return batch.add_groups();
    }

    const google::protobuf::RepeatedPtrField<MessageGroup>& get_messages(const MessageGroupBatch& batch) const override {
        return batch.groups();
    }

private:

    const queue_attributes_t REQUIRED_SUBSCRIBE_ATTRIBUTES {"raw", "subscribe"};
    const queue_attributes_t REQUIRED_SEND_ATTRIBUTES {"raw", "publish"};
};

}

