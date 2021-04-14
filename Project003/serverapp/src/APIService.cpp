
#include "APIService.h"


APIService::APIService() {}

APIService::~APIService() {}

void APIService::addSupportToMethodsHTTP() {
	httpListener.support(web::http::methods::GET, std::bind(&APIService::handleGET, this, std::placeholders::_1));
	httpListener.support(web::http::methods::PUT, std::bind(&APIService::notHandleMethod, this, std::placeholders::_1, web::http::methods::PUT));
	httpListener.support(web::http::methods::POST, std::bind(&APIService::notHandleMethod, this, std::placeholders::_1, web::http::methods::POST));
	httpListener.support(web::http::methods::DEL, std::bind(&APIService::notHandleMethod, this, std::placeholders::_1, web::http::methods::DEL));
	httpListener.support(web::http::methods::PATCH, std::bind(&APIService::notHandleMethod, this, std::placeholders::_1, web::http::methods::PATCH));
	httpListener.support(web::http::methods::HEAD, std::bind(&APIService::notHandleMethod, this, std::placeholders::_1, web::http::methods::HEAD));
	httpListener.support(web::http::methods::OPTIONS, std::bind(&APIService::notHandleMethod, this, std::placeholders::_1, web::http::methods::OPTIONS));
	httpListener.support(web::http::methods::TRCE, std::bind(&APIService::notHandleMethod, this, std::placeholders::_1, web::http::methods::TRCE));
	httpListener.support(web::http::methods::CONNECT, std::bind(&APIService::notHandleMethod, this, std::placeholders::_1, web::http::methods::CONNECT));
}

void APIService::handleGET(web::http::http_request message) {
	std::string path = extractMessagePath(message);
	Logger::getLogger()->log("GET for path <" + path + ">");

	if (path == "/api") {
		message.reply(web::http::status_codes::OK, assemblyResponse(response_codes::code::SUCCESS, "API REST for AWS meta-data information"));
	}
	else if (path == "/api/ec2/instance-id") {
		web::http::client::http_client client("http://169.254.169.254/latest/meta-data");
		auto responseAWS = client.request(web::http::methods::GET, "/instance-id").get();
		if (responseAWS.status_code() == 200)
			message.reply(web::http::status_codes::OK, \
						  assemblyResponse(response_codes::code::SUCCESS, responseAWS.extract_string().get()));
		else
			message.reply(web::http::status_codes::OK, \
						  assemblyResponse(response_codes::code::ERROR, "AWS meta-data returned " + std::to_string(responseAWS.status_code())));
	}
	else
		message.reply(web::http::status_codes::NotFound, assemblyResponse(response_codes::code::ERROR, "path not found"));
}

void APIService::notHandleMethod(web::http::http_request message, web::http::method & method) {
	message.reply(web::http::status_codes::NotImplemented, assemblyResponse(response_codes::code::ERROR, method + " not implemented"));
}

web::json::value APIService::assemblyResponse(response_codes::code rStatus, std::string rMessageOrData) {
	web::json::value response = web::json::value::object();
	bool isStatusSuccess = (rStatus == response_codes::code::SUCCESS);
	
	//Designed for:
	//  response = { "status":"error", "message":"details about error", "data":null }
	//  response = { "status":"success", "message":null, "data":"requested info" }
	response["status"] = web::json::value::string( response_codes::to_string(rStatus) );
	response["message"] = isStatusSuccess ? web::json::value::null() : web::json::value::string(rMessageOrData);
	response["data"] = isStatusSuccess ? web::json::value::string(rMessageOrData) : web::json::value::null();
	
	return response;
}

std::string APIService::extractMessagePath(web::http::http_request message) {
	return message.request_uri().path();
}

std::string APIService::response_codes::to_string(code c) {
	switch (c) {
		case code::ERROR:		return "error"; break;
		case code::SUCCESS:		return "success";
	}
	return "success";
}