cmake_minimum_required(VERSION 3.8)

add_executable(Json "include/Json.h" "include/Json.inl" "app/main.cpp")
target_include_directories(Json PUBLIC "include")