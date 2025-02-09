cmake_minimum_required(VERSION 3.28)
project(Civ-LXVDXXXV LANGUAGES CXX)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


add_executable(termstyle_sample
    TermStyle.cpp
    TermStyle_sample.cpp
)

target_compile_features(termstyle_sample PRIVATE cxx_std_20)


add_executable(civlog_sample
    TermStyle.cpp
    ScopeFlags.cpp
    civlog.cpp
    civlog_sample.cpp
)

target_compile_features(civlog_sample PRIVATE cxx_std_20)

add_executable(getopt_sample
    TermStyle.cpp
    ScopeFlags.cpp
    civlog.cpp
    A_GetOpt.cpp
    A_GetOpt_sample.cpp
)

target_compile_features(getopt_sample PRIVATE cxx_std_20)
