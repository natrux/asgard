cmake_minimum_required(VERSION 3.10)
project(shut-up-cmake)


find_package(antlr4-runtime REQUIRED)

add_executable(main
	generated/AsgardLexer.cpp
	generated/AsgardParser.cpp
	src/main.cpp
)

target_include_directories(main PUBLIC
	${ANTLR4_INCLUDE_DIR}
	generated
)

target_link_libraries(main
	antlr4-runtime
)
