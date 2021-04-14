
#pragma once

#include <string>
#include <cpprest/http_listener.h>
#include <pplx/pplxtasks.h>
#include <boost/asio.hpp>


class APIController {
	protected:
		web::http::experimental::listener::http_listener httpListener;
	
	private:
		std::string getHostIP();
		virtual void addSupportToMethodsHTTP();

	public:
		APIController();
		virtual ~APIController();
		
		void setEndpoint(const std::string & URI);
		std::string getEndpoint() const;

		pplx::task<void> open();
		pplx::task<void> close();
};