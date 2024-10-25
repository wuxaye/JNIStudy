# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.18.1)

# Declares and names the project.

project("myjni")

# 创建的第一个so
add_library( # Sets the name of the library.
        myjni

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        native-lib.cpp)

# 第二个库
add_library(mysecondlib SHARED second-lib.cpp)

# 生产者消费者模型库
add_library(pclib SHARED producer_consumer.cpp)

#串口
add_library(serialportlib SHARED serial_port.cpp)

find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log)

target_link_libraries( # Specifies the target library.
        myjni

        # Links the target library to the log library
        # included in the NDK.
        ${log-lib})

# 第二个库
target_link_libraries(mysecondlib ${log-lib})

# 生产者消费者模型库
target_link_libraries(pclib ${log-lib})

#串口
target_link_libraries(serialportlib ${log-lib})