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

#include "schema/message/impl/rabbitmq/AbstractRabbitSender.h"

#include "common.pb.h"

namespace th2::common_cpp {
/*
    var batchBuilder = MessageGroupBatch.newBuilder();

    for (var rawMessage : value.getMessagesList()) {
        var anyMessage = AnyMessage.newBuilder().setRawMessage(rawMessage).build();
        var group = MessageGroup.newBuilder().addMessages(anyMessage).build();
        batchBuilder.addGroups(group);
    }

    return batchBuilder.build().toByteArray();
	*/

class RabbitRawBatchSender : public AbstractRabbitSender<RawMessageBatch> {
protected:

    ByteVector value_to_bytes(const RawMessageBatch& batch) override {
    	MessageGroupBatch group_batch;

    	for (auto&& raw_msg: batch.messages()) {
    		AnyMessage any_msg;

    		any_msg.mutable_raw_message()->CopyFrom(raw_msg);

    		MessageGroup group_msg;

    		group_msg.mutable_messages()->Add()->CopyFrom(any_msg);

    		group_batch.mutable_groups()->Add()->CopyFrom(group_msg);
    	}

        ByteVector bv;

        auto size = group_batch.ByteSizeLong();
        bv.resize(size);

        group_batch.SerializePartialToArray(reinterpret_cast<void*>(bv.data()), size);

        return bv;
    }
};

}
