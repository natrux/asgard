cmake_minimum_required(VERSION 3.22)



add_executable(server
	main_server.cpp
)

target_link_libraries(server
	asgard
)


add_executable(client
	main_client.cpp
	src/StdIn.cpp
)

target_include_directories(client PUBLIC
	include
)

target_link_libraries(client
	asgard
)


