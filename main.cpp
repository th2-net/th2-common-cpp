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

#include <iostream>

#include <schema/factory/CommonFactory.h>

#include <common.pb.h>

#include <log4cxx/logger.h>
#include <log4cxx/xml/domconfigurator.h>
#include <log4cxx/propertyconfigurator.h>
#include <log4cxx/helpers/properties.h>

int main(int argc, char* argv[]) {

    using namespace th2;
    using namespace th2::common_cpp;
    
    //Define logger variable
    log4cxx::LoggerPtr loggerMain(log4cxx::Logger::getLogger("main"));
    setenv("main", "log4cxx.log", true);
    log4cxx::PropertyConfigurator::configure("logging-configmap.yaml");
    
    LOG4CXX_INFO (loggerMain, "---start main()---");
    
    auto factory = argc > 1 ? std::make_unique<CommonFactory>(CommonFactory::create_from_arguments(argc, argv))
                            : std::make_unique<CommonFactory>();
                            
    LOG4CXX_INFO (loggerMain, "auto factory is initialized. argc = " << argc);

    factory->init();
    
    LOG4CXX_INFO (loggerMain, "factory->init() successful");


    std::cout << "Sending raw messages" << std::endl;

    {
        auto raw_router = factory->get_message_router_raw_batch();
        LOG4CXX_DEBUG (loggerMain, "auto raw_router = factory->get_message_router_raw_batch()");

        constexpr size_t batch_total = 10;
        for (size_t i = 0; i < batch_total; ++i) {
            RawMessageBatch batch;

            auto raw_msg_batch = batch.add_messages();
            auto meta = raw_msg_batch->mutable_metadata();

            auto timestamp = meta->mutable_timestamp();
            timestamp->set_seconds(1234);
            timestamp->set_nanos(5678);

            auto properties = meta->mutable_properties();

            (*properties)["requestId"] = std::to_string(0);
            (*properties)["requestRef"] = std::to_string(1);

            //std::cout << "index " << msg.raw_msg->msg_batch_index << " index ref " << msg.raw_msg->msg_batch_index_ref << std::endl;

            auto message_id = meta->mutable_id();

            bool from_client = true;
            if (from_client) {
                message_id->set_direction(Direction::SECOND);
            } else {
                message_id->set_direction(Direction::FIRST);
            }

            message_id->set_sequence(0);

            auto connection_id = message_id->mutable_connection_id();
            connection_id->set_session_alias("session_alias");

            raw_msg_batch->set_body(std::string("Message body"));

            queue_attributes_t attr;

            attr.emplace_back("publish");
            attr.emplace_back("raw");
            attr.emplace_back("first");
            attr.emplace_back("store");

//            raw_router->sendAll(batch, attr);
            raw_router->send(batch);
            LOG4CXX_DEBUG (loggerMain, "raw_router->send(batch)");
        }
    }

    std::cout << "Sending events" << std::endl;

    {
        auto event_router = factory->get_event_batch_router();
        LOG4CXX_DEBUG (loggerMain, "auto event_router = factory->get_event_batch_router()");
        constexpr size_t batch_total = 10;
        for (size_t i = 0; i < batch_total; ++i) {
            EventBatch event_batch;

            auto event = event_batch.add_events();

            auto start_timestamp = event->mutable_start_timestamp();
            auto end_timestamp = event->mutable_end_timestamp();

            start_timestamp->set_seconds(1234);
            start_timestamp->set_nanos(5678);
            end_timestamp->set_seconds(2345);
            end_timestamp->set_nanos(6789);

            event->set_status(SUCCESS);
            event->set_name(std::string("Demo_event"));
            event->set_type(std::string("Example"));
            event->set_body(std::string("Event body"));

            auto event_id = event->id();
            event_id.set_id("ex_event");

            event_router->send(event_batch);
            LOG4CXX_DEBUG (loggerMain, "event_router->send(event_batch)");
        }
    }

    std::cout << "Sending group messages" << std::endl;

    {
        auto group_router = factory->get_message_router_message_group_batch();
        LOG4CXX_DEBUG (loggerMain, "auto group_router = factory->get_message_router_message_group_batch()");
        constexpr size_t batch_total = 10;
        for (size_t i = 0; i < batch_total; ++i) {
            MessageGroupBatch batch;

            auto group_msg_batch = batch.add_groups();
            auto msg = (group_msg_batch->add_messages())->mutable_raw_message();
            auto meta = msg->mutable_metadata();

            auto timestamp = meta->mutable_timestamp();
            timestamp->set_seconds(1234);
            timestamp->set_nanos(5678);

            auto properties = meta->mutable_properties();

            (*properties)["requestId"] = std::to_string(0);
            (*properties)["requestRef"] = std::to_string(1);

            auto message_id = meta->mutable_id();

            bool from_client = true;
            if (from_client) {
                message_id->set_direction(Direction::SECOND);
            } else {
                message_id->set_direction(Direction::FIRST);
            }

            message_id->set_sequence(0);

            auto connection_id = message_id->mutable_connection_id();
            connection_id->set_session_alias("session_alias");

            msg->set_body(std::string("Group message body"));

            queue_attributes_t attr;

            attr.emplace_back("publish");
            attr.emplace_back("raw");
            attr.emplace_back("first");
            attr.emplace_back("store");

            group_router->send(batch);
            LOG4CXX_DEBUG (loggerMain, "group_router->send(batch)");
        }
    }

    std::cout << "Finished" << std::endl;
    LOG4CXX_INFO (loggerMain, "---end main()---");
    return 0;
}
