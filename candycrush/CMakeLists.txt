# ===================================================================
# CMake Configuration for 'candycrush' SFML Project
# ===================================================================

# --- Basic Project Setup --- REQUIRED AT THE TOP ---
cmake_minimum_required(VERSION 3.15)
project(candycrush LANGUAGES CXX) # <-- Updated project name to 'candycrush'
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --- SFML Configuration ---
set(SFML_DIR "E:/SFML-2.6.1/lib/cmake/SFML") # <--- VERIFY THIS IS STILL CORRECT
set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML 2.6.1
        COMPONENTS graphics window system # Add audio, network if needed
        REQUIRED)

# --- Add Your Executable --- REQUIRED ---
add_executable(candycrush main.cpp) # <-- Defines the 'candycrush' target

# --- Configure SFML Linking ---
if(SFML_FOUND)
    message(STATUS "SFML Found! Version: ${SFML_VERSION}")
    target_link_libraries(candycrush PRIVATE sfml-graphics sfml-window sfml-system) # <-- Link to 'candycrush' target. Add sfml-audio etc. if needed

    if(SFML_STATIC_LIBRARIES)
        message(STATUS "Configuring for STATIC SFML linking.")
        target_compile_definitions(candycrush PRIVATE SFML_STATIC) # <-- Apply to 'candycrush' target
    else()
        message(STATUS "Configuring for DYNAMIC SFML linking.")
    endif()
else()
    message(FATAL_ERROR "SFML NOT FOUND! Check SFML_DIR path.")
endif()

# --- Optional: Copy Assets (e.g., images, fonts) to Build Directory ---
message(STATUS "Setting up asset copying...")

# Define the source asset directory (points to E:/candycrush/assets)
set(SOURCE_ASSET_DIR ${CMAKE_SOURCE_DIR}/assets)

# Check if the source directory exists before setting up the command
if(EXISTS ${SOURCE_ASSET_DIR})
    message(STATUS "Source asset directory found: ${SOURCE_ASSET_DIR}")
    message(STATUS "Will copy assets to an 'assets' folder next to the executable.")

    add_custom_command(TARGET candycrush POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
            ${SOURCE_ASSET_DIR}
            $<TARGET_FILE_DIR:candycrush>/assets
            COMMENT "Copying assets from ${SOURCE_ASSET_DIR} to $<TARGET_FILE_DIR:candycrush>/assets"
            VERBATIM
    )
else()
    message(WARNING "Asset source directory NOT FOUND: ${SOURCE_ASSET_DIR}. Asset copying command will NOT be added.")
endif()

# --- End of CMakeLists.txt ---
