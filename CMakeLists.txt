cmake_minimum_required(VERSION 3.21)

project(NordicBodyHairSliders VERSION 0.1.0 LANGUAGES CXX)

find_package(CommonLibSSE CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

add_commonlibsse_plugin(${PROJECT_NAME}
    SOURCES
        src/PCH.h
        src/main.cpp
        src/Settings.h
        src/Settings.cpp
        src/HairColor.h
        src/HairColor.cpp
        src/OverlayManager.h
        src/OverlayManager.cpp
        src/RaceMenuIntegration.h
        src/RaceMenuIntegration.cpp
)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)
target_precompile_headers(${PROJECT_NAME} PRIVATE src/PCH.h)
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    NOMINMAX
    WIN32_LEAN_AND_MEAN
)

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /utf-8 /permissive- /Zc:preprocessor)
endif()

set(PACKAGE_ROOT "${CMAKE_SOURCE_DIR}/package")
set(PLUGIN_OUT "${PACKAGE_ROOT}/SKSE/Plugins")

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${PLUGIN_OUT}"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE:${PROJECT_NAME}>"
        "${PLUGIN_OUT}/$<TARGET_FILE_NAME:${PROJECT_NAME}>"
    VERBATIM
)
