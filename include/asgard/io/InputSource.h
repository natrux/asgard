#pragma once

#include <cstddef>


namespace asgard{
namespace io{


class InputSource{
public:
	virtual ~InputSource() = default;
	/**
	 * Reads up to length bytes into the given buffer.
	 * If no data is available, the call can block until there is.
	 * 
	 * Returns the number of read bytes, zero if the source has been closed.
	 * Throws if there was an error.
	 */
	virtual size_t read(void *data, size_t length) = 0;

	/**
	 * Reads length bytes into the given buffer, possibly blocking.
	 * Throws if the input source gets closed before the requested
	 * amount of data is retrieved.
	*/
	void read_all(void *data, size_t length);
};


}
}
