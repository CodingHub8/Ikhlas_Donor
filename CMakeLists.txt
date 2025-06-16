cmake_minimum_required(VERSION 3.31)
project(Ikhlas_Donor)

set(CMAKE_CXX_STANDARD 23)

# Add MySQL Connector
set(MYSQLCONNCPP_ROOT "C:/Users/aasus/Documents/Programming/MySQL/mysql-8.2.0-winx64")

# Add Faker-cxx
set(FAKER_ROOT "C:/Users/aasus/Documents/Programming/C++/Faker/faker-cxx")
set(BUILD_TESTING OFF CACHE BOOL "Disable all tests" FORCE)
set(FAKER_ENABLE_TESTING OFF CACHE BOOL "Disable Faker-cxx testing" FORCE)
add_subdirectory(${FAKER_ROOT} "${CMAKE_BINARY_DIR}/faker-cxx")

include_directories(
        "${MYSQLCONNCPP_ROOT}/include"
        "${MYSQLCONNCPP_ROOT}/include/mysql"
        "${FAKER_ROOT}/include"
)

link_directories("${MYSQLCONNCPP_ROOT}/lib")

add_executable(Ikhlas_Donor
        DBConnection.h
        main.cpp
        User.h
        Utilities.h
        Admin.h
        Item.h)

target_link_libraries(Ikhlas_Donor
        mysql
        faker-cxx
)