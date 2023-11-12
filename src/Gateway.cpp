#include <asgard/com/Gateway.h>
#include <asgard/io/BufferedInput.h>

#include <algorithm>
#include <thread>


namespace asgard{
namespace com{


Gateway::Gateway(const std::string &name_):
	GatewayModule(name_)
{
}


Gateway::Gateway(const std::string &name_, std::unique_ptr<net::Endpoint> endpoint):
	GatewayModule(name_)
{
	init_endpoint(std::move(endpoint));
}


Gateway::Gateway(const std::string &name_, const std::string &address):
	GatewayModule(name_)
{
	init_endpoint(address);
}


void Gateway::init(){
	if(!m_endpoint){
		try{
			m_endpoint = net::Endpoint::from_address(m_address);
		}catch(const std::exception &err){
			throw std::runtime_error("Creating endpoint from address failed with: " + std::string(err.what()));
		}
	}
	if(!m_endpoint){
		throw std::logic_error("No endpoint given");
	}
	Super::init();
}


void Gateway::main(){
	std::thread read_thread(&Gateway::read_loop, this);

	Super::main();

	if(m_endpoint->is_connected()){
		try{
			m_endpoint->shutdown();
		}catch(const std::exception &err){
			log(WARN) << err.what();
		}
	}

	if(read_thread.joinable()){
		read_thread.join();
	}
	m_endpoint->close();
}


void Gateway::init_endpoint(const std::string &address){
	if(m_endpoint || !m_address.empty()){
		throw std::logic_error("Endpoint already given");
	}
	m_address = address;
}


void Gateway::init_endpoint(std::unique_ptr<net::Endpoint> endpoint){
	if(m_endpoint || !m_address.empty()){
		throw std::logic_error("Endpoint already given");
	}
	m_endpoint = std::move(endpoint);
}


void Gateway::set_output(std::unique_ptr<io::OutputSource> output){
	std::lock_guard<std::mutex> lock(mutex_output);
	m_output = std::move(output);
}


void Gateway::output_write(const void *data, size_t length){
	std::lock_guard<std::mutex> lock(mutex_output);
	if(!m_output){
		throw std::overflow_error("output closed");
	}
	m_output->write_all(data, length);
}


void Gateway::read_loop(){
	bool connect_error = false;
	while(node_should_run()){
		if(connect_error){
			error_wait();
			connect_error = false;
		}

		if(!m_endpoint->is_connected()){
			if(!m_endpoint->can_connect()){
				// Nothing to do anymore
				node_exit();
				break;
			}
			if(!m_endpoint->is_open()){
				try{
					m_endpoint->open();
				}catch(const std::exception &err){
					connect_error = true;
					log(WARN) << err.what();
				}
			}
			if(m_endpoint->is_open()){
				try{
					m_endpoint->connect();
					log(INFO) << "Connected";
				}catch(const std::exception &err){
					connect_error = true;
					log(WARN) << err.what();
				}
				if(!connect_error){
					set_output(m_endpoint->output_source());
					try{
						on_connect();
					}catch(const std::exception &err){
						log(WARN) << err.what();
					}
				}
			}
		}

		if(m_endpoint->is_connected()){
			try{
				keep_reading(m_endpoint->input_source());
			}catch(const std::underflow_error &err){
				log(INFO) << err.what();
			}catch(const std::exception &err){
				log(WARN) << err.what();
			}
			m_endpoint->close();
			set_output(nullptr);
			try{
				on_disconnect();
			}catch(const std::exception &err){
				log(WARN) << err.what();
			}
		}
	}
	try{
		on_hang_up();
	}catch(const std::exception &err){
		log(WARN) << err.what();
	}
}


void Gateway::keep_reading(std::unique_ptr<io::InputSource> input_source){
	// Dummy implementation, overwrite in child class
	io::BufferedInput input(std::move(input_source));
	while(true){
		input.read_next();
	}
}


void Gateway::error_wait() const{
	const time::duration micro_time = std::chrono::milliseconds(10);
	time::duration remaining = error_pause_time;
	while(node_should_run() && remaining > time::duration::zero()){
		const auto micro_wait = std::min(remaining, micro_time);
		std::this_thread::sleep_for(micro_wait);
		remaining -= micro_wait;
	}
}



}
}


