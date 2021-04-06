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

#include <boost/program_options.hpp>

#include "schema/factory/AbstractCommonFactory.h"

//LOGGING
#include <log4cxx/logger.h>

namespace th2::common_cpp {

class CommonFactory : public AbstractCommonFactory {
private:
    std::filesystem::path CONFIG_DEFAULT_PATH = "/var/th2/config/";

    const std::string_view RABBIT_MQ_FILE_NAME = "rabbitMQ.json";
    const std::string_view ROUTER_MQ_FILE_NAME = "mq.json";
    const std::string_view ROUTER_GRPC_FILE_NAME = "grpc.json";
    const std::string_view CRADLE_FILE_NAME = "cradle.json";
    const std::string_view PROMETHEUS_FILE_NAME = "prometheus.json";
    const std::string_view CUSTOM_FILE_NAME = "custom.json";

    std::filesystem::path rabbitMQ;
    std::filesystem::path routerMQ;
    std::filesystem::path routerGRPC;
    std::filesystem::path cradle;
    std::filesystem::path custom;
    std::filesystem::path prometheus;
    std::filesystem::path dictionariesDir;

public:
    CommonFactory() : AbstractCommonFactory() {
    	this->logger_common_factory = log4cxx::Logger::getLogger("CommonFactory");
	LOG4CXX_DEBUG (logger_common_factory, "Start constructor CommonFactory");  
        this->rabbitMQ = get_path(RABBIT_MQ_FILE_NAME);
        LOG4CXX_DEBUG (logger_common_factory, "init this->rabbitMQ = get_path(RABBIT_MQ_FILE_NAME)"); 
        this->routerMQ = get_path(ROUTER_MQ_FILE_NAME);
        LOG4CXX_DEBUG (logger_common_factory, "init this->routerMQ = get_path(ROUTER_MQ_FILE_NAME)"); 
    }

    CommonFactory(std::filesystem::path rabbitMQ,
                  std::filesystem::path routerMQ,
                  std::filesystem::path routerGRPC,
                  std::filesystem::path cradle,
                  std::filesystem::path custom,
                  std::filesystem::path prometheus,
                  std::filesystem::path dictionariesDir)
        : AbstractCommonFactory()
    {
    	this->logger_common_factory = log4cxx::Logger::getLogger("CommonFactory");
        this->rabbitMQ = rabbitMQ;
        this->routerMQ = routerMQ;
        this->routerGRPC = routerGRPC;
        this->cradle = cradle;
        this->custom = custom;
        this->prometheus = prometheus;
        this->dictionariesDir = dictionariesDir;
    }

    ~CommonFactory() = default;

    /**
    * Create {@link CommonFactory} from command line arguments
    *
    * @param args String array of command line arguments. Arguments:
    *             <p>
    *             --rabbitConfiguration - path to json file with RabbitMQ configuration
    *             <p>
    *             --messageRouterConfiguration - path to json file with configuration for {@link MessageRouter}
    *             <p>
    *             --grpcRouterConfiguration - path to json file with configuration for {@link GrpcRouter}
    *             <p>
    *             --cradleConfiguration - path to json file with configuration for cradle. ({@link CradleConfiguration})
    *             <p>
    *             --customConfiguration - path to json file with custom configuration
    *             <p>
    *             --dictionariesDir - path to directory which contains files with encoded dictionaries
    *             <p>
    *             --prometheusConfiguration - path to json file with configuration for prometheus metrics server
    *             <p>
    *             -c/--configs - folder with json files for schemas configurations with special names:
    *             <p>
    *             rabbitMq.json - configuration for RabbitMQ
    *             mq.json - configuration for {@link MessageRouter}
    *             grpc.json - configuration for {@link GrpcRouter}
    *             cradle.json - configuration for cradle
    *             custom.json - custom configuration
    * @return CommonFactory with set path
    * @throws ParseException - Can not parse command line arguments
    */
    static CommonFactory create_from_arguments(int argc, char* argv[]) {
        boost::program_options::options_description desc("Usage");
        desc.add_options()("rabbitConfiguration", boost::program_options::value<std::vector<std::string>>(),
                           "path to json file with RabbitMQ configuration")
                          ("messageRouterConfiguration", boost::program_options::value<std::vector<std::string>>(),
                           "path to json file with configuration for MessageRouter");

        boost::program_options::variables_map vm;
        boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
        boost::program_options::notify(vm);

        std::filesystem::path rabbitMQ;
        std::filesystem::path routerMQ;

        if (vm.count("rabbitConfiguration")) {
            auto val = vm["rabbitConfiguration"].as<std::vector<std::string>>();
            rabbitMQ = val[0];
        }

        if (vm.count("messageRouterConfiguration")) {
            auto val = vm["messageRouterConfiguration"].as<std::vector<std::string>>();
            routerMQ = val[0];
        }

        CommonFactory cf {rabbitMQ, routerMQ, {}, {}, {}, {}, {}};

        return cf;
    }

    [[nodiscard]]
    std::filesystem::path get_path_to_rmq_configuration() const override {
        return rabbitMQ;
    }

    [[nodiscard]]
    std::filesystem::path get_path_to_message_router_configuration() const override {
        return routerMQ;
    }

private:
    std::filesystem::path get_path(const std::string_view file) const;
    log4cxx::LoggerPtr logger_common_factory;
};

}
