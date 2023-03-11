#pragma once

#include <asgard/io/InputSource.h>
#include <asgard/io/OutputSource.h>
#include <string>
#include <memory>


namespace asgard{
namespace net{


class Endpoint{
public:
	static std::unique_ptr<Endpoint> from_address(const std::string &address, bool allow_named=true);
	virtual ~Endpoint() = default;
	virtual std::string get_name() const = 0;

	virtual bool is_open() const = 0;
	virtual bool is_connected() const = 0;
	virtual bool can_connect() const = 0;
	virtual bool can_expect() const = 0;

	virtual void open() = 0;
	virtual void expect() const = 0;
	virtual std::unique_ptr<Endpoint> accept() const = 0;
	virtual void connect() = 0;
	virtual void shutdown() const = 0;
	virtual void close() = 0;

	virtual std::unique_ptr<io::InputSource> input_source() const = 0;
	virtual std::unique_ptr<io::OutputSource> output_source() const = 0;
};


}
}
