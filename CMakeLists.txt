cmake_minimum_required(VERSION 3.20)
project(cpp20_market_data_pipeline LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(md_pipeline
        src/main.cpp
)

target_include_directories(md_pipeline PRIVATE include)

# std::format sometimes needs extra flags depending on libstdc++ version.
# If you hit issues, switch to fmtlib or use clang+libc++.
