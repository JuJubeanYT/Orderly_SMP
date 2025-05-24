cmake_minimum_required(VERSION 3.15)
project(MinecraftProfileCreator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_executable(MinecraftProfileCreator MACOSX_BUNDLE
    main.cpp
    downloader.cpp
    unzipper.cpp
    parser.cpp
)


target_include_directories(MinecraftProfileCreator PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

