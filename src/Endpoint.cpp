#include <asgard/net/Endpoint.h>
#include <asgard/net/TcpEndpoint.h>
#ifndef _WIN32
#include <asgard/net/UnixEndpoint.h>
#endif
#include <asgard/net/NamedEndpoint.h>
#include <asgard/make_unique.h>

#include <stdexcept>


namespace asgard{
namespace net{



std::unique_ptr<Endpoint> Endpoint::from_address(const std::string &address, bool allow_named){
	const auto colon = address.rfind(":");
	if(colon != std::string::npos){
		// IPv4 or IPv6
		std::string host = address.substr(0, colon);
		std::string str_port = address.substr(colon+1);
		unsigned long port = std::stoul(str_port);

		if(host == "localhost" || host.find(".") != std::string::npos){
			return std::make_unique<Tcp4Endpoint>(host, port);
		}else{
			return std::make_unique<Tcp6Endpoint>(host, port);
		}
	}

#ifndef _WIN32
	if(address.length() >= 5 && address.find(".sock", address.length()-5) != std::string::npos){
		// Unix Socket
		return std::make_unique<UnixEndpoint>(address);
	}
#endif

	// if nothing else helps
	if(allow_named){
		return std::make_unique<NamedEndpoint>(address);
	}

	throw std::logic_error("No suitable Endpoint found for '" + address + "'");
}


}
}
