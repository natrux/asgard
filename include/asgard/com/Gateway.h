#pragma once

#include <asgard/mod/Module.h>
#include <asgard/net/Endpoint.h>
#include <asgard/io/InputSource.h>

#include <set>
#include <thread>
#include <memory>


namespace asgard{
namespace com{


class Gateway : public asgard::mod::Module{
	using Super = asgard::mod::Module;
public:
	Gateway(const std::string &name_, std::unique_ptr<net::Endpoint> endpoint);
	Gateway(const std::string &name_, const std::string &address);

protected:
	void main() override;

	void set_output(std::unique_ptr<io::OutputSource> output);
	void output_write(const char *data, size_t length);

private:
	unsigned int error_pause_time_ms = 1000;
	std::unique_ptr<net::Endpoint> m_endpoint;
	std::mutex mutex_output;
	std::unique_ptr<io::OutputSource> m_output;

	void read_loop();
	virtual void keep_reading(std::unique_ptr<io::InputSource> input_source);
	virtual void on_connect(){ /* empty default */ }
	virtual void on_disconnect(){ /* empty default */ }
	virtual void on_hang_up(){ /* empty default */ }

	void error_wait() const;
};


}
}
