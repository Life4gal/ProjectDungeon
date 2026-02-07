cmake_minimum_required(VERSION 3.25)

# set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
set(VCPKG_ROOT "C:/workspace/vcpkg")
set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")

set(
		PD_MAJOR_VERSION
		1
		CACHE STRING
		"ProjectDungeon major version"
)
set(
		PD_MINOR_VERSION
		0
		CACHE STRING
		"ProjectDungeon minor version"
)
set(
		PD_PATCH_VERSION
		0
		CACHE STRING
		"ProjectDungeon patch version"
)
set(
		PD_VERSION
		"${PD_MAJOR_VERSION}.${PD_MINOR_VERSION}.${PD_PATCH_VERSION}"
		CACHE STRING
		"ProjectDungeon version"
)

project(
		ProjectDungeon
		VERSION ${PD_VERSION}
		DESCRIPTION "ProjectDungeon"
		HOMEPAGE_URL "https://github.com/Life4gal/ProjectDungeon"
		LANGUAGES CXX
)

# ===================================================================================================
# OPTION

# ===================================================================================================
# DEFINES

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/defines.cmake)

# ===================================================================================================
# DEPENDENCIES

#include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/find_cpm.cmake)

# SFML
find_package(SFML 3 CONFIG REQUIRED COMPONENTS Graphics Audio)
find_package(OpenGL REQUIRED)
set(PD_SFML_LIBRARIES "SFML::Graphics;SFML::Audio;OpenGL::GL")

# IMGUI
find_package(imgui CONFIG REQUIRED)

# EnTT
find_package(EnTT CONFIG REQUIRED)

# box2d
find_package(box2d CONFIG REQUIRED)

# nlohmann::json
find_package(nlohmann_json CONFIG REQUIRED)

# spdlog
find_package(spdlog CONFIG REQUIRED)

# ===================================================================================================
# PACK

set(PD_VERSION_HEADER_FILE ${CMAKE_CURRENT_BINARY_DIR}/include/pd/version.hpp)

configure_file(
		${CMAKE_CURRENT_SOURCE_DIR}/cmake/version.hpp.in
		${PD_VERSION_HEADER_FILE}
		@ONLY
)

# ===================================================================================================
# EXECUTABLE

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
#set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(${PROJECT_NAME})

target_include_directories(
		${PROJECT_NAME}

		PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		# version header file
		$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
		$<INSTALL_INTERFACE:include>
)

target_compile_options(
		${PROJECT_NAME}
		PUBLIC

		${PD_COMPILE_FLAGS}
)

#target_compile_definitions(
#	${PROJECT_NAME}
#	PUBLIC
#
#)

target_compile_features(
		${PROJECT_NAME}
		PRIVATE
		cxx_std_23
)

target_link_libraries(
		${PROJECT_NAME}
		PRIVATE

		${PD_SFML_LIBRARIES}
		imgui::imgui
		EnTT::EnTT
		box2d::box2d
		spdlog::spdlog
)

set(
		PD_PUBLIC_FILES

		${CMAKE_CURRENT_SOURCE_DIR}/include/pd/defines.hpp

		# ============================
		# EXTERNAL
		# ============================

		${CMAKE_CURRENT_SOURCE_DIR}/include/pd/external/imgui-SFML.hpp
)

set(
		PD_PRIVATE_FILES

		# ============================
		# EXTERNAL
		# ============================

		${CMAKE_CURRENT_SOURCE_DIR}/src/external/imgui-SFML.cpp

		# ============================
		# MAIN
		# ============================

		${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
)

set_source_files_properties(
		${PD_PUBLIC_FILES}
		${PD_PRIVATE_FILES}
		PROPERTIES
		LANGUAGE CXX
)

target_sources(
		${PROJECT_NAME}
		PUBLIC
		FILE_SET public_header_files
		TYPE HEADERS
		BASE_DIRS
		${CMAKE_CURRENT_SOURCE_DIR}/include
		# version header file
		${CMAKE_CURRENT_BINARY_DIR}/include
		FILES
		${PD_PUBLIC_FILES}
		${PD_VERSION_HEADER_FILE}
)

target_sources(
		${PROJECT_NAME}
		PRIVATE
		${PD_PRIVATE_FILES}
)
