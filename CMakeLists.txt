cmake_minimum_required(VERSION 3.28)
project(Civ-LXVDXXXV LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

include_directories(
    modules/
    modules/imgui
    modules/imgui-sfml
    modules/implot
)



set(IMGUI_SRC
    modules/imgui/imgui.cpp
    modules/imgui/imgui_widgets.cpp
    modules/imgui/imgui_draw.cpp
    modules/imgui/imgui_tables.cpp

    modules/imgui/imgui_demo.cpp
)

set(IMGUI_SFML_SRC
    modules/imgui-sfml/imgui-SFML.cpp
)

set(IMPLOT_SRC
    modules/implot/implot.cpp
    modules/implot/implot_items.cpp
    modules/implot/implot_demo.cpp
)

add_executable(main
    ${IMGUI_SRC}
    ${IMGUI_SFML_SRC}
    ${IMPLOT_SRC}

    src/main.cpp
)

target_compile_features(main PRIVATE cxx_std_20)
target_link_libraries(main PRIVATE SFML::Graphics)
