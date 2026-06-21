cmake_minimum_required(VERSION 3.10)
project(asgard CXX)

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)
option(BUILD_CODEGEN "Build code generator" ON)

find_package(Threads REQUIRED)


if(BUILD_CODEGEN)
	find_package(antlr4-runtime REQUIRED)

	add_executable(asgard_codegen
		grammar/asgard/code/AsgardLexer.cpp
		grammar/asgard/code/AsgardParser.cpp
		src/codegen.cpp
	)

	target_include_directories(asgard_codegen PUBLIC
		${ANTLR4_INCLUDE_DIR}
		grammar
	)

	target_link_libraries(asgard_codegen
		antlr4-runtime
	)
endif()


file(GLOB GENERATED_CXX
	package/asgard/com/*.cxx
	package/asgard/data/*.cxx
	package/asgard/run/*.cxx
)

add_library(asgard
	${GENERATED_CXX}
	src/ID.cpp
	src/Signature.cpp
	src/EnumMap.cpp
	src/TypeRegistry.cpp
	src/Value.cpp
	src/Enum.cpp
	src/Bin.cpp
	src/InputSource.cpp
	src/BufferedInput.cpp
	src/TypeReader.cpp
	src/Endpoint.cpp
	src/File.cpp
	src/Exception.cpp
	src/FileInputSource.cpp
	src/FileOutputSource.cpp
	src/PosixInputSource.cpp
	src/PosixOutputSource.cpp
	src/Fifo.cpp
	src/FifoInputSource.cpp
	src/FifoOutputSource.cpp
	src/VectorInputSource.cpp
	src/VectorOutputSource.cpp
	src/Gateway.cpp
	src/NamedEndpoint.cpp
	src/OutputSource.cpp
	src/BufferedOutput.cpp
	src/TypeWriter.cpp
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
	src/RPC.cpp
	src/random.cpp
	src/strtime.cpp
	src/Topic.cpp
	src/TopicPtr.cpp
	src/NetworkStream.cpp
	src/Terminal.cpp
	src/string.cpp
	src/LogMessage.cpp
	src/LogPublisher.cpp
	src/crc_64_xz.cpp
)
if(WIN32)
	target_sources(asgard PRIVATE
		src/win32/Terminal.cpp
		src/win32/ls.cpp
	)
else()
	target_sources(asgard PRIVATE
		src/linux/UnixEndpoint.cpp
		src/linux/Terminal.cpp
		src/linux/ls.cpp
	)
endif()

target_compile_features(asgard PUBLIC cxx_std_17)
target_compile_options(asgard PRIVATE
	-Wall -Wextra -Wpedantic
	-Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization
	-Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations
	-Wmissing-include-dirs -Wold-style-cast -Wredundant-decls -Wshadow
	-Wsign-conversion -Wsign-promo -Wstrict-null-sentinel
	-Wstrict-overflow -Wsuggest-override -Wswitch-default -Wundef

	#-Werror
)

target_include_directories(asgard PUBLIC
	include
	package
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
