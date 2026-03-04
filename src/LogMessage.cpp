#include <asgard/data/LogMessage.hxx>


namespace asgard{
namespace data{


std::string LogMessage::get_level() const{
	switch(level){
	case log_level_e::DEBUG:
		return "DEBUG";
	case log_level_e::INFO:
		return "INFO";
	case log_level_e::WARN:
		return "WARN";
	case log_level_e::ERROR:
		return "ERROR";
	default:
		return "???";
	}
}


std::string LogMessage::format() const{
	return "[" + unit + "] [" + get_level() + "] " + message;
}


}
}

