cmake_minimum_required(VERSION 3.10)


file(GLOB GENERATED_CXX package/*.cxx)

add_executable(server
	${GENERATED_CXX}
	src/main_server.cpp
)

target_include_directories(server PUBLIC
	include
	package
)

target_link_libraries(server
	asgard
)


add_executable(client
	${GENERATED_CXX}
	src/main_client.cpp
	src/StdIn.cpp
)

target_include_directories(client PUBLIC
	include
	package
)

target_link_libraries(client
	asgard
)


