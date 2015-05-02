cmake_minimum_required(VERSION 2.6)
include(ExternalProject)

project(Arkesia)
INCLUDE_DIRECTORIES(BEFORE ${CMAKE_BINARY_DIR}/include)
SET(ENV{PATH} ${CMAKE_BINARY_DIR}/bin:$ENV{PATH})

EXTERNALPROJECT_ADD(
	regal
	GIT_REPOSITORY git@github.com:p3/regal.git
	GIT_TAG master
	CONFIGURE_COMMAND ""
	BUILD_COMMAND PATH=$ENV{PATH} make -j16
	INSTALL_COMMAND ""
	BUILD_IN_SOURCE 1
)
INCLUDE_DIRECTORIES( SYSTEM
	${CMAKE_BINARY_DIR}/include
	${CMAKE_BINARY_DIR}/regal-prefix/src/regal/include
)
LINK_DIRECTORIES(
	${CMAKE_BINARY_DIR}/regal-prefix/src/regal/lib/darwin
)

add_subdirectory( plugins )

# Use our modified FindSDL2* modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${Arkesia_SOURCE_DIR}/cmake")
# Set an output directory for our binaries

# Bump up warning levels appropriately for clang, gcc & msvc
# Also set debug/optimization flags depending on the build type. IDE users choose this when
# selecting the build mode in their IDE
if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -std=c++11")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG} -g")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE} -O2")
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
	if (CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
		string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
	else()
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
	endif()
endif()

# Look up SDL2 and add the include directory to our include path
find_package(SDL2 REQUIRED)
include_directories(
	${SDL2_INCLUDE_DIR}
)
message("${PROJECT_BINARY_DIR}/bin")
file(GLOB_RECURSE sources
	"src/*.cpp"
)

add_executable(arkesia ${sources})
add_dependencies(arkesia regal)
target_link_libraries(arkesia Regallib ${SDL2_LIBRARY})
install(TARGETS arkesia RUNTIME DESTINATION ${PROJECT_BINARY_DIR}/bin)

# Look in the src subdirectory to find its CMakeLists.txt so we can build the executable
add_subdirectory(src)