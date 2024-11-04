# CMakeLists.txt : CMake project for chess, include source and define
# project specific logic here.
#
cmake_minimum_required(VERSION 3.8)

# Add header and source directories
include_directories(${CMAKE_SOURCE_DIR}/include) # adds headers of 
include_directories(${CMAKE_SOURCE_DIR}/Chess/src)     # New includes
include_directories(${CMAKE_SOURCE_DIR}/Chess/headers)

# Gather all source files in the src directory
file(GLOB_RECURSE CHESS_SRC_FILES "src/*.cpp")
list(REMOVE_ITEM CHESS_SRC_FILES ${CMAKE_SOURCE_DIR}/Chess/src/TestEngine.cpp) # Exclude TestEngine.cpp from chess

# Add the main chess executable (uses main.cpp)
add_executable(chess ${CHESS_SRC_FILES})

# Link SFML libraries
if(CMAKE_BUILD_TYPE MATCHES Debug)
    target_link_libraries(chess 
        ${CMAKE_SOURCE_DIR}/lib/sfml-graphics-d.lib
        ${CMAKE_SOURCE_DIR}/lib/sfml-audio-d.lib
        ${CMAKE_SOURCE_DIR}/lib/sfml-network-d.lib
        ${CMAKE_SOURCE_DIR}/lib/sfml-system-d.lib
        ${CMAKE_SOURCE_DIR}/lib/sfml-window-d.lib
    )
else()
    target_link_libraries(chess 
        ${CMAKE_SOURCE_DIR}/lib/sfml-graphics.lib
        ${CMAKE_SOURCE_DIR}/lib/sfml-audio.lib
        ${CMAKE_SOURCE_DIR}/lib/sfml-network.lib
        ${CMAKE_SOURCE_DIR}/lib/sfml-system.lib
        ${CMAKE_SOURCE_DIR}/lib/sfml-window.lib
    )
endif()

# Set C++ standard
set_property(TARGET chess PROPERTY CXX_STANDARD 20)

#copy image files into build folder after building
add_custom_command(TARGET chess POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
    $<TARGET_FILE_DIR:chess>/Images
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/Chess/Images $<TARGET_FILE_DIR:chess>/Images
)
#copy dll files into build folder after building
set(SFML_DLL_DIR ${CMAKE_SOURCE_DIR}/bin)


# Copy DLL files from bin folder to the output directory after building
add_custom_command(TARGET chess POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${SFML_DLL_DIR}
    $<TARGET_FILE_DIR:chess>)



# Define source files for TestEngine executable
file(GLOB_RECURSE TESTENGINE_SRC_FILES "src/*.cpp")
list(REMOVE_ITEM TESTENGINE_SRC_FILES ${CMAKE_SOURCE_DIR}/Chess/src/main.cpp) # Exclude main.cpp from TestEngine
# Add the TestEngine executable (uses TestEngine.cpp)
add_executable(TestEngine ${TESTENGINE_SRC_FILES})

# Link SFML libraries for TestEngine executable
if(CMAKE_BUILD_TYPE MATCHES Debug)
    target_link_libraries(TestEngine 
        ${CMAKE_SOURCE_DIR}/lib/sfml-graphics-d.lib
        ${CMAKE_SOURCE_DIR}/lib/sfml-audio-d.lib
        ${CMAKE_SOURCE_DIR}/lib/sfml-network-d.lib
        ${CMAKE_SOURCE_DIR}/lib/sfml-system-d.lib
        ${CMAKE_SOURCE_DIR}/lib/sfml-window-d.lib
    )
else()
    target_link_libraries(TestEngine 
        ${CMAKE_SOURCE_DIR}/lib/sfml-graphics.lib
        ${CMAKE_SOURCE_DIR}/lib/sfml-audio.lib
        ${CMAKE_SOURCE_DIR}/lib/sfml-network.lib
        ${CMAKE_SOURCE_DIR}/lib/sfml-system.lib
        ${CMAKE_SOURCE_DIR}/lib/sfml-window.lib
    )
endif()

# Set C++ standard for TestEngine executable
set_property(TARGET TestEngine PROPERTY CXX_STANDARD 20)

# Optionally copy resources for TestEngine, similar to chess
add_custom_command(TARGET TestEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
    $<TARGET_FILE_DIR:TestEngine>/Images
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/Chess/Images $<TARGET_FILE_DIR:TestEngine>/Images
)
add_custom_command(TARGET TestEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${SFML_DLL_DIR}
    $<TARGET_FILE_DIR:TestEngine>
)