cmake_minimum_required(VERSION 3.10)
project(asgard CXX)

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

find_package(Threads)


if(WIN32)
	list(APPEND PLATFORM_SOURCE_FILES
		src/win32/Terminal.cpp
		src/win32/ls.cpp
	)
	list(APPEND PLATFORM_CODEGEN_SOURCE_FILES
		src/win32/ls.cpp
	)
else()
	list(APPEND PLATFORM_SOURCE_FILES
		src/linux/UnixEndpoint.cpp
		src/linux/Terminal.cpp
		src/linux/ls.cpp
	)
	list(APPEND PLATFORM_CODEGEN_SOURCE_FILES
		src/linux/ls.cpp
	)
endif()


file(GLOB GENERATED_CXX
	package/asgard/com/*.cxx
	package/asgard/data/*.cxx
	package/asgard/run/*.cxx
)

add_library(asgard
	${GENERATED_CXX}
	${PLATFORM_SOURCE_FILES}
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
	src/Exception.cpp
	src/FileInputSource.cpp
	src/FileOutputSource.cpp
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


add_executable(asgard_codegen
	${PLATFORM_CODEGEN_SOURCE_FILES}
	src/ClassType.cpp
	src/DataType.cpp
	src/Declaration.cpp
	src/Method.cpp
	src/ModuleType.cpp
	src/Namespace.cpp
	src/packages.cpp
	src/Token.cpp
	src/Type.cpp
	src/codegen.cpp
)

target_include_directories(asgard_codegen PUBLIC
	include
)

add_subdirectory(test)
