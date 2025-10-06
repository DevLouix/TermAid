cmake_minimum_required(VERSION 3.15)
project(TermAid VERSION 2.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Include FetchContent for downloading dependencies
include(FetchContent)

# Download nlohmann/json library
FetchContent_Declare(
    json
    URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)
FetchContent_MakeAvailable(json)

# Add executable
add_executable(termaid src/termaid.cpp)

# Link libraries
target_link_libraries(termaid PRIVATE nlohmann_json::nlohmann_json)

# Platform-specific settings
if(WIN32)
    # Windows-specific settings
    target_compile_definitions(termaid PRIVATE _WIN32_WINNT=0x0601)
elseif(APPLE)
    # macOS-specific settings
    target_compile_options(termaid PRIVATE -Wall -Wextra)
elseif(UNIX)
    # Linux-specific settings
    target_compile_options(termaid PRIVATE -Wall -Wextra)
endif()

# Installation
install(TARGETS termaid DESTINATION bin)

# Copy commands directory to build directory
file(COPY ${CMAKE_SOURCE_DIR}/commands DESTINATION ${CMAKE_BINARY_DIR})

# Print build information
message(STATUS "TermAid v${PROJECT_VERSION}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "System: ${CMAKE_SYSTEM_NAME}")
