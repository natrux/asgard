cmake_minimum_required(VERSION 3.22)
project(asgard CXX)

set(CMAKE_CXX_STANDARD 11)
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	add_compile_options(
		-pedantic -Wall -Wextra
		-Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization
		-Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations
		-Wmissing-include-dirs -Wold-style-cast -Wredundant-decls -Wshadow
		-Wsign-conversion -Wsign-promo -Wstrict-null-sentinel
		-Wstrict-overflow -Wsuggest-override -Wswitch-default -Wundef

		-Werror
	)
else()
	message(WARNING "Unknown compiler - not turning on warnings")
endif()

find_package(Threads)

if(WIN32)
	list(APPEND PLATFORM_SOURCE_FILES
		src/win32/Terminal.cpp
	)
else()
	list(APPEND PLATFORM_SOURCE_FILES
		src/linux/UnixEndpoint.cpp
		src/linux/Terminal.cpp
	)
endif()

add_library(asgard SHARED
	${PLATFORM_SOURCE_FILES}
	src/ID.cpp
	src/crc_64_xz.cpp
	src/BufferedInput.cpp
	src/Endpoint.cpp
	src/FileInputSource.cpp
	src/FileOutputSource.cpp
	src/Gateway.cpp
	src/NamedEndpoint.cpp
	src/OutputSource.cpp
	src/Server.cpp
	src/SocketEndpoint.cpp
	src/SocketInputSource.cpp
	src/SocketOutputSource.cpp
	src/TcpEndpoint.cpp
	src/Messager.cpp
	src/Client.cpp
	src/Subscriber.cpp
	src/Module.cpp
	src/Pipe.cpp
	src/PipeIn.cpp
	src/Topic.cpp
	src/TopicPtr.cpp
	src/NetworkStream.cpp
	src/Terminal.cpp
	src/string.cpp
	src/LogMessage.cpp
	src/LogPublisher.cpp

	src/TerminalClient.cxx
)

target_include_directories(asgard PUBLIC
	include/
)

target_link_libraries(asgard
	Threads::Threads
)
if(WIN32)
	target_link_libraries(asgard
		ws2_32
	)
endif()

add_subdirectory(test)
