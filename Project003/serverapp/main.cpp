
#include <iostream>
#include "APIService.h"
#include "include/DateTimeUtils.h"

#include "include/Logger.h"
#define LOGGER Logger::getLogger()


//execution = ./bin/restapi.app <port_number>
int main(int argc, const char * argv[])
{
    APIService service;
	std::string port = ((argc > 1) ? argv[1] : "8080");
    service.setEndpoint("http://host_IPV4:" + port + "/api");	//host_IPV4 is internally replaced by actual IP of host!

	LOGGER->log(">> Endpoint configured: " + service.getEndpoint());
	std::cout << ">> Endpoint configured: " << service.getEndpoint() << '\n';

    try {
		LOGGER->log(">> Opening...", Logger::message_types::type::INFO);
		std::cout << ">> Opening...\n";
		
        service.open().wait();

		LOGGER->log(">> Service listening for requests!", Logger::message_types::type::INFO);
        std::cout << ">> Service listening for requests!\n\n";

		while (true);
		// boost::asio::io_context handlerContext;
		// boost::asio::signal_set signals(handlerContext, SIGINT);
		// signals.async_wait( interrupt_handler );
		// handlerContext.run();
  
        service.close().wait();
    }
    catch(std::exception & e) {
		LOGGER->log(">> " + (std::string) e.what(), Logger::message_types::type::ERROR);
        std::cout << ">> ERROR! <" <<  e.what() << ">\n";
    }

    return 0;
}
