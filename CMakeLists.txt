cmake_minimum_required(VERSION 3.31)
project(Ikhlas_Donor)

set(CMAKE_CXX_STANDARD 14)
set(MYSQLCONNCPP_ROOT "C:/Users/aasus/Documents/Programming/MySQL/mysql-8.2.0-winx64")

include_directories(
        "${MYSQLCONNCPP_ROOT}/include"
        "${MYSQLCONNCPP_ROOT}/include/mysql"
)

link_directories("${MYSQLCONNCPP_ROOT}/lib")

add_executable(Ikhlas_Donor
        DBConnection.h
        main.cpp
        User.h
        Utilities.h)

target_link_libraries(Ikhlas_Donor
        mysql
)