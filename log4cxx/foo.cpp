#include <stdlib.h> 
#include <iostream>
#include <schema/factory/CommonFactory.h>
#include <log4cxx/logger.h>
#include <log4cxx/xml/domconfigurator.h>
#include <log4cxx/propertyconfigurator.h>
#include <log4cxx/helpers/properties.h>
using namespace log4cxx;
using namespace log4cxx::xml;
using namespace log4cxx::helpers;
using namespace th2::common_cpp;

// Define static logger variable
LoggerPtr loggerMyMain(Logger::getLogger( "main"));

int main()
{
    // Load XML configuration file using DOMConfigurator
    DOMConfigurator::configure("foo.xml");
    setenv("name", "NAME_OF_THE_CURRENT_LOG_FILE", true);
    PropertyConfigurator::configure("foo.properties");

    LOG4CXX_INFO (loggerMyMain, "[main]---start main()---");
    
    auto factory = std::make_unique<th2::common_cpp::CommonFactory>();
    LOG4CXX_DEBUG (loggerMyMain, "[main]is created " << &factory);
    factory->init();
    LOG4CXX_INFO (loggerMyMain, "[main]factory->init() successful");
 	 
    LOG4CXX_INFO (loggerMyMain, "[main]---end main()---");
    return 0;
}
