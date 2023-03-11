#pragma once

#include <asgard/io/InputSource.h>
#include <memory>


namespace asgard{
namespace io{


#define BUFFER_SIZE 65536
#define BUFFER_THRESHOLD 8192


class BufferedInput{
public:
	BufferedInput(std::unique_ptr<InputSource> source);
	/**
	 * Reads length bytes into the given buffer, possibly blocking.
	 * Throws if the input source gets closed before the requested
	 * amount of data is retrieved.
	 * 
	 * To speed up future reading, the function might also read more
	 * than the requested number of bytes and buffer them internally.
	 */
	void read(char *data, size_t length);

	/**
	 * Reads one byte in the same way that read() does.
	 */
	char read_next();

private:
	std::unique_ptr<InputSource> m_source;
	// buffer is filled from start until before end
	char m_buffer[BUFFER_SIZE];
	size_t start = 0;
	size_t end = 0;
	
	void read_from_buffer(char *data, size_t length);
	size_t fill_buffer();
	void reset_buffer();
};


}
}


