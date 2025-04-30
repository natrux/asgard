#pragma once

#include <asgard/io/OutputSource.h>

#include <vector>
#include <memory>


namespace asgard{
namespace io{


class BufferedOutput{
public:
	BufferedOutput(std::shared_ptr<OutputSource> source);

	virtual ~BufferedOutput();

	/**
	 * Writes length bytes from the given buffer, possibly looping and blocking.
	 * Throws if part of the data is refused.
	 * 
	 * To speed up low level writing, the method may buffer (part of)
	 * the data internally and write it on a subsequent call.
	 * Make sure to call flush() when data needs to be actually sent.
	 */
	void write(const void *data, size_t length);

	/**
	 * Writes the bytes in data.
	 * Writing behaves in the same way as in write(void*, size_t).
	 */
	void write(const std::vector<uint8_t> &data);

	/**
	 * Writes one byte in the same way as write(void*, size_t).
	 */
	void write(const uint8_t &data);

	/**
	 * Writes all internally buffered data to the output.
	 * Throws if part of the data is refused.
	 */
	void flush();

private:
	static constexpr size_t BUFFER_SIZE = 65536;
	static constexpr size_t BUFFER_THRESHOLD = 8192;
	std::shared_ptr<OutputSource> m_source;
	// buffer is filled from 0 until before end
	uint8_t m_buffer[BUFFER_SIZE];
	size_t end = 0;
};


}
}
