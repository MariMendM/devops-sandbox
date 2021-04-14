
#include "APIController.h"


APIController::APIController() {}

APIController::~APIController() {}

void APIController::setEndpoint(const std::string &URI) {
	web::uri endpointURI(URI);
	
	//URI example = https://127.0.0.1:123/forum/questions
	web::uri_builder endpointBuilder;
	endpointBuilder.set_scheme(endpointURI.scheme());	// https
	endpointBuilder.set_host(getHostIP());				// 127.0.0.1
	endpointBuilder.set_port(endpointURI.port());		// 123
	endpointBuilder.set_path(endpointURI.path());		// /forum/questions

	this->httpListener = web::http::experimental::listener::http_listener(endpointBuilder.to_uri());
}

std::string APIController::getEndpoint() const {
	return this->httpListener.uri().to_string();
}

std::string APIController::getHostIP() {
	boost::asio::io_service ioService;
	boost::asio::ip::tcp::resolver resolver(ioService);
	return resolver.resolve(boost::asio::ip::host_name(), "")->endpoint().address().to_string();
}

pplx::task<void> APIController::open() {
	addSupportToMethodsHTTP();
	return this->httpListener.open();
}

pplx::task<void> APIController::close() {
	return this->httpListener.close();
}

void APIController::addSupportToMethodsHTTP() {}
