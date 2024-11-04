cmake_minimum_required(VERSION 3.8)

# Find and include GoogleTest
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/release-1.12.1.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)  # For Windows users
FetchContent_MakeAvailable(googletest)


# Add include directories
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/Chess/src)
include_directories(${CMAKE_SOURCE_DIR}/Chess/headers)

# Gather all test source files in the TestChess directory
file(GLOB_RECURSE TEST_SRC_FILES "*.cpp")

# Create the test executable
add_executable(TestChess ${TEST_SRC_FILES})
file(GLOB_RECURSE ENGINE_SRC_FILES ${CMAKE_SOURCE_DIR}/Chess/src/*.cpp)
add_library(Engine STATIC ${ENGINE_SRC_FILES})
target_link_libraries(TestChess gtest gtest_main Engine)

# Link the test executable with GoogleTest and the SFML libraries
if(CMAKE_BUILD_TYPE MATCHES Debug)
    target_link_libraries(TestChess gtest_main 
        ${CMAKE_SOURCE_DIR}/lib/sfml-graphics-d.lib
        ${CMAKE_SOURCE_DIR}/lib/sfml-audio-d.lib
        ${CMAKE_SOURCE_DIR}/lib/sfml-network-d.lib
        ${CMAKE_SOURCE_DIR}/lib/sfml-system-d.lib
        ${CMAKE_SOURCE_DIR}/lib/sfml-window-d.lib
        Engine 
    )
else()
    target_link_libraries(TestChess gtest_main 
        ${CMAKE_SOURCE_DIR}/lib/sfml-graphics.lib
        ${CMAKE_SOURCE_DIR}/lib/sfml-audio.lib
        ${CMAKE_SOURCE_DIR}/lib/sfml-network.lib
        ${CMAKE_SOURCE_DIR}/lib/sfml-system.lib
        ${CMAKE_SOURCE_DIR}/lib/sfml-window.lib
        Engine 
    )
endif()

# Register tests with CTest
include(GoogleTest)
gtest_discover_tests(TestChess)


# Define the output directory for copying resources
set(OUTPUT_DIR $<TARGET_FILE_DIR:TestChess>)

# Optionally copy resources for TestChess if needed
add_custom_command(TARGET TestChess POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${OUTPUT_DIR}/Images
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Chess/Images ${OUTPUT_DIR}/Images
    COMMAND ${CMAKE_COMMAND} -E echo "Finished copying Images."
)

# Set the SFML DLL directory
set(SFML_DLL_DIR ${CMAKE_SOURCE_DIR}/bin)
message(STATUS "SFML DLL Directory: ${SFML_DLL_DIR}")

# Copy SFML DLLs to the output directory
add_custom_command(TARGET TestChess POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "Copying SFML DLLs..."
    COMMAND ${CMAKE_COMMAND} -E make_directory ${OUTPUT_DIR}  # This is where the TestChess executable is located
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${SFML_DLL_DIR} ${OUTPUT_DIR}  # Destination for DLLs
    COMMAND ${CMAKE_COMMAND} -E echo "Finished copying SFML DLLs."
)
