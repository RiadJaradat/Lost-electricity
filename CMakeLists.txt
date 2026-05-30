cmake_minimum_required(VERSION 3.15)
project(LostElectricity VERSION 1.0.0 LANGUAGES CXX)

# Force C++23 or higher (SFML works great with standard modern C++)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Find the SFML packages
# You can add or remove components here (e.g., audio, network) depending on your needs
find_package(SFML 2.5 REQUIRED COMPONENTS graphics window system)

# 2. Define your executable and point it to your source file
add_executable(${PROJECT_NAME} src/main.cpp)

# 3. include include
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")

# 4. Include SFML headers and link the library binaries to your executable
target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics sfml-window sfml-system)

target_compile_options(${PROJECT_NAME} PRIVATE 
    -O3
    -Ofast 
    -ffast-math 
    -march=native # best run in Intel (tm) core i7 gen 8, its compiled source.
)