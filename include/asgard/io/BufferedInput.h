#pragma once

#include <asgard/io/InputSource.h>

#include <vector>
#include <memory>


namespace asgard{
namespace io{


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
	void read(void *data, size_t length);

	/**
	 * Reads length bytes and returns them as a vector.
	 * Reading behaves in the same way as in read(void*, size_t).
	 */
	std::vector<uint8_t> read(size_t length);

	/**
	 * Reads sizeof(T) bytes in the same way that read(void*, size_t) does.
	 */
	template<class T>
	T read(){
		T result;
		read(&result, sizeof(T));
		return result;
	}

private:
	static constexpr size_t BUFFER_SIZE = 65536;
	static constexpr size_t BUFFER_THRESHOLD = 8192;
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


