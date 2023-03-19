#include <asgard/data/LogMessage.hxx>


namespace asgard{
namespace data{


std::string LogMessage::format() const{
	std::string level_string;
	switch(level){
	case log_level_e::DEBUG:
		level_string = "DEBUG";
		break;
	case log_level_e::INFO:
		level_string = "INFO";
		break;
	case log_level_e::WARN:
		level_string = "WARN";
		break;
	case log_level_e::ERROR:
		level_string = "ERROR";
		break;
	default:
		level_string = "???";
	}
	return "[" + unit + "] [" + level_string + "] " + message;
}


}
}

