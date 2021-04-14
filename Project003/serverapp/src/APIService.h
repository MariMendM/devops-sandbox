
#pragma once

#include "APIController.h"
#include "APIMethods.h"
#include "include/Logger.h"

#include <cpprest/http_client.h>


class APIService : public APIController, APIMethods {
	private:
		std::string extractMessagePath(web::http::http_request message);
		
		class response_codes {
			public:
				enum class code { ERROR, SUCCESS };
				static std::string to_string(code t);
		};

		web::json::value assemblyResponse(response_codes::code rStatus, std::string rMessageOrData);

		//From APIMethods (whose methods are or are not handled by service)
		void handleGET(web::http::http_request message) override;		//Only method implemented in this demo
		void handleHEAD(web::http::http_request message) override;		//NOT Implemented (handler: notHandleMethod)
		void handlePUT(web::http::http_request message) override;		//NOT Implemented (handler: notHandleMethod)
		void handlePOST(web::http::http_request message) override;		//NOT Implemented (handler: notHandleMethod)
		void handleDELETE(web::http::http_request message) override;	//NOT Implemented (handler: notHandleMethod)
		void handlePATCH(web::http::http_request message) override;		//NOT Implemented (handler: notHandleMethod)
		void handleOPTIONS(web::http::http_request message) override;	//NOT Implemented (handler: notHandleMethod)
		void handleTRACE(web::http::http_request message) override;		//NOT Implemented (handler: notHandleMethod)
		void handleCONNECT(web::http::http_request message) override;	//NOT Implemented (handler: notHandleMethod)
		void notHandleMethod(web::http::http_request message, web::http::method & method) override;

		//From APIController (connection of methods' handlers to listener)
		void addSupportToMethodsHTTP() override;

	public:
		APIService();
		~APIService();
};
