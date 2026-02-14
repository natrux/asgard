cmake_minimum_required(VERSION 3.10)


file(GLOB GENERATED_CXX package/*.cxx)

add_executable(main
	${GENERATED_CXX}
	src/Calculator.cpp
	src/Math.cpp
	src/main.cpp
)

target_include_directories(main PUBLIC
	include
	package
)

target_link_libraries(main
	asgard
)


