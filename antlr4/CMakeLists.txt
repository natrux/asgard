cmake_minimum_required(VERSION 3.10)


find_package(antlr4-runtime REQUIRED)

file(GLOB GENERATED_CXX generated/*.cpp)

add_executable(main
	${GENERATED_CXX}
	src/main.cpp
)

target_include_directories(main PUBLIC
	${ANTLR4_INCLUDE_DIR}
	generated
)

target_link_libraries(main
	antlr4-runtime
)
