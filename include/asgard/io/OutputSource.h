#pragma once

#include <string>
#include <cstddef>


namespace asgard{
namespace io{



class OutputSource{
public:
	virtual ~OutputSource() = default;
	/**
	 * Writes up to length bytes from the given buffer, possibly blocking.
	 * Returns the number of written bytes.
	 */
	virtual size_t write(const char *data, size_t length) = 0;
	/**
	 * Writes length bytes from the given buffer, possibly looping and blocking.
	 * Throws if part of the data is refused.
	 */
	void write_all(const char *data, size_t length);
	void write_all(const std::string &data);
};



}
}

