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
};


}
}
