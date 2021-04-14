
#pragma once

#include <cpprest/http_msg.h>


class APIMethods {
	private:
		//only GET is enough for the demo!
		virtual void handleGET		(web::http::http_request message) = 0;
		// virtual void handlePUT    (web::http::http_request message) = 0;
		// virtual void handlePOST   (web::http::http_request message) = 0;
		// virtual void handleDEL    (web::http::http_request message) = 0;
		// virtual void handlePATCH  (web::http::http_request message) = 0;
		// virtual void handleHEAD   (web::http::http_request message) = 0;
		// virtual void handleOPTIONS(web::http::http_request message) = 0;
		// virtual void handleTRCE   (web::http::http_request message) = 0;
		// virtual void handleCONNCET(web::http::http_request message) = 0;

		virtual void notHandleMethod(web::http::http_request message, web::http::method & method) = 0;
};