#pragma once

#include <asgard/data/Data.hxx>
#include <asgard/time/time.h>
#include <asgard/data/log_level_e.hxx>

#include <string>


namespace asgard{
namespace data{


class LogMessage : public Data{
	using Super = Data;
public:
	static code::Signature static_signature();
	static std::shared_ptr<LogMessage> create();

	LogMessage();
	void read_member(io::TypeReader &reader, const std::string &name, const code::Typecode &type) override;
	void write_member(io::TypeWriter &writer, const std::string &name) const override;

	time::time time;
	log_level_e level;
	std::string unit;
	std::string message;

	std::string get_level() const;
	std::string format() const;
};


}
}

