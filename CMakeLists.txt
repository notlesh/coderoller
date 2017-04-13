# cmake file for coderoller
cmake_minimum_required(VERSION 3.0.0)
project(coderoller_lib)

# set(CMAKE_CXX_STANDARD 11) # requires cmake >= 3.1)
add_compile_options(-std=c++11)

include_directories("include/" "tinyxml2/")

set(SOURCES ./src/base/stream_dispatcher.cpp
	./src/base/stream_roller.cpp
	./src/cr_util.cpp
	./src/cr_globals.cpp
	./src/core/exception.cpp
	./src/core/file.cpp
	./src/core/log.cpp
	./src/languages/cpp
	./src/languages/cpp/cpp_util.cpp
	./src/languages/cpp/cpp_class_generator.cpp
	./src/languages/cpp/cpp_generator.cpp
	./src/generate.cpp
	./src/parse/parse.cpp
	./tinyxml2/tinyxml2.cpp
)

# Get the current working branch
execute_process(
	COMMAND git rev-parse --abbrev-ref HEAD
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
	OUTPUT_VARIABLE GIT_BRANCH
	OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Get the latest commit hash of the working branch
execute_process(
	COMMAND git rev-parse HEAD
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
	OUTPUT_VARIABLE GIT_HASH
	OUTPUT_STRIP_TRAILING_WHITESPACE
)

add_definitions("-DGIT_HASH=\"${GIT_HASH}\"")
add_definitions("-DGIT_BRANCH=\"${GIT_BRANCH}\"")

add_library(cr STATIC ${SOURCES})

add_executable(coderoller "app/linux_main.cpp")
target_link_libraries(coderoller cr)

