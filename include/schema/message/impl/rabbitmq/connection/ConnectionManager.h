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

#include <chrono>
#include <memory>
#include <thread>

#include <boost/asio/io_service.hpp>
#include <boost/asio/strand.hpp>
#include <boost/asio/deadline_timer.hpp>

#include <amqpcpp.h>
#include <amqpcpp/linux_tcp.h>
#include <amqpcpp/libboostasio.h>

#include "schema/message/impl/rabbitmq/configuration/RabbitMQConfiguration.h"
#include "Util.h"

namespace th2::common_cpp {

class ConnectionManager {
public:
    using rmq_connection_ptr = std::unique_ptr<AMQP::TcpConnection>;
    using rmq_channel_ptr = std::unique_ptr<AMQP::TcpChannel>;

    ConnectionManager(rmq_configuration_ptr cfg) {
        rmq_configuration = cfg;
    }

    ~ConnectionManager() {
        if (service_thread.joinable()) {
            service_thread.join();
        }
    }

    void init() {
        if (rmq_configuration->get_subscriberName().empty()) {
            subscriber_name = rmq_configuration->get_subscriberName();
        } else {
            subscriber_name = "rabbit_mq_subscriber."; //TODO implement + System.currentTimeMillis();
        }

        handler = std::make_unique<AMQP::LibBoostAsioHandler>(service);

        std::string addr_str = "amqp://";
        addr_str.append(rmq_configuration->get_username());
        addr_str.append(":");
        addr_str.append(rmq_configuration->get_password());
        addr_str.append("@");
        addr_str.append(rmq_configuration->get_host());
        addr_str.append(":");
        addr_str.append(std::to_string(rmq_configuration->get_port()));
        addr_str.append(rmq_configuration->get_vHost());

        std::cout << addr_str << std::endl;

        connection = std::make_unique<AMQP::TcpConnection>(handler.get(), AMQP::Address(addr_str));

        channel = std::make_unique<AMQP::TcpChannel>(connection.get());
        channel->setQos(rmq_configuration->get_prefetchCount());

        service_thread = std::thread([this](){
            service.run();
        });
    }

    void basic_publish(const std::string& exchange, const std::string& routing_key, const ByteVector& message) {
//        channel->declareQueue(routing_key, args)
//        .onSuccess([](const std::string &name, uint32_t messagecount, uint32_t consumercount) {
//            std::cout << "onSuccess [" << name << "]" << std::endl;
//        })
//        .onError([](const char *message) {
//            std::cout << "onError [" << message << "]" << std::endl;
//        });

//        channel->declareExchange(rmq_configuration->get_exchangeName())
//        .onSuccess([](){
//            std::cout << "declareExchange onSuccess" << std::endl;
//        })
//        .onFinalize([](){
//            std::cout << "declareExchange onFinalize" << std::endl;
//        })
//        .onError([](const char* message){
//            std::cout << "declareExchange onError [" << message << "]" << std::endl;
//        });

//        channel->bindQueue(rmq_configuration->get_exchangeName(), routing_key, routing_key)
//        .onSuccess([](){
//            std::cout << "bindQueue onSuccess" << std::endl;
//        })
//        .onFinalize([](){
//            std::cout << "bindQueue onFinalize" << std::endl;
//        })
//        .onError([](const char* message){
//            std::cout << "bindQueue onError [" << message << "]" << std::endl;
//        });

        //channel->startTransaction();

        bool b = channel->publish(exchange, routing_key, reinterpret_cast<const char*>(message.data()), message.size());

        //std::cout << b << std::endl;

        //channel->commitTransaction();
    }

private:
    rmq_configuration_ptr rmq_configuration;
    rmq_connection_ptr connection;
    rmq_channel_ptr channel;

    std::unique_ptr<AMQP::LibBoostAsioHandler> handler;

    std::string subscriber_name;

    boost::asio::io_service service{4};
    std::thread service_thread;
};

using connection_manager_ptr = std::shared_ptr<ConnectionManager>;

}
