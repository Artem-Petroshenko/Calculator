cmake_minimum_required (VERSION 3.8)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project ("CalculatorCMake")

add_executable (CalculatorCMake "Main.cpp" "Calc.cpp" "Operators.cpp" "PluginLoader.cpp" "Calc.h" "Operators.h" "PluginLoader.h" "Token.h")

add_subdirectory("dll")
