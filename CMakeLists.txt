cmake_minimum_required(VERSION 3.30)
project(SDL_1)

#Set cool standard on
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

#SDL2
find_package(SDL2 REQUIRED COMPONENTS SDL2)
include_directories(${SDL2_INCLUDE_DIRS})

#local files
set(SOURCE_FILES src/main.c)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC src)
target_link_libraries(${PROJECT_NAME} PRIVATE -std=c23 ${SDL2_LIBRARIES})




