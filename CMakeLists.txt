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
# DEFINES

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/defines.cmake)

# ===================================================================================================
# DEPENDENCIES

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/find_cpm.cmake)

# Core
CPMAddPackage(
		NAME PrometheusCore
		GIT_TAG main
		GITHUB_REPOSITORY "Life4gal/core"
		OPTIONS 
		"PROMETHEUS_CORE_TEST OFF"
		"PROMETHEUS_CORE_INSTALL OFF"
)

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

set(PD_VERSION_HEADER_FILE ${CMAKE_CURRENT_BINARY_DIR}/game/version.hpp)

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

add_executable(
	${PROJECT_NAME}
		
	# ============================
	# EXTERNAL
	# ============================

	${CMAKE_CURRENT_SOURCE_DIR}/src/external/imgui-SFML.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/external/imgui-SFML.cpp
					
	# ============================
	# UTILITY
	# ============================
	
	# transparent string hash
	# 允许unordered_xxx之类的容器通过string_view查找key为string的元素
	${CMAKE_CURRENT_SOURCE_DIR}/src/utility/

	# id与任意类型的双向映射
	${CMAKE_CURRENT_SOURCE_DIR}/src/utility/map.hpp

	# ============================
	# CONFIG
	# ============================

	# 翻页(循环)动画
	${CMAKE_CURRENT_SOURCE_DIR}/src/config/flip_animation.hpp

	# ============================
	# BLUEPRINT
	# ============================

	# 翻页(循环)动画
	${CMAKE_CURRENT_SOURCE_DIR}/src/blueprint/flip_animation.hpp

	# ============================
	# ASSET
	# ============================

	# 资源载入器
	${CMAKE_CURRENT_SOURCE_DIR}/src/asset/asset_loader.hpp
	# 字体资源载入器
	${CMAKE_CURRENT_SOURCE_DIR}/src/asset/font_loader.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/asset/font_loader.cpp
	# 纹理资源载入器
	${CMAKE_CURRENT_SOURCE_DIR}/src/asset/texture_loader.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/asset/texture_loader.cpp
	# 音效资源载入器
	${CMAKE_CURRENT_SOURCE_DIR}/src/asset/sound_loader.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/asset/sound_loader.cpp
	# 音乐资源载入器
	${CMAKE_CURRENT_SOURCE_DIR}/src/asset/music_loader.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/asset/music_loader.cpp

	# ============================
	# COMPONENTS
	# ============================

	# 游戏信息
	${CMAKE_CURRENT_SOURCE_DIR}/src/components/game.hpp
	# 世界信息
	${CMAKE_CURRENT_SOURCE_DIR}/src/components/world.hpp
	
	# 方向(一些静态物体的朝向)
	#${CMAKE_CURRENT_SOURCE_DIR}/src/components/direction.hpp
	# 位置/大小/旋转等信息
	${CMAKE_CURRENT_SOURCE_DIR}/src/components/transform.hpp

	# 动画信息
	${CMAKE_CURRENT_SOURCE_DIR}/src/components/flip_animation.hpp
	
	# ============================
	# SYSTEMS
	# ============================
		
	${CMAKE_CURRENT_SOURCE_DIR}/src/systems/system.hpp

	${CMAKE_CURRENT_SOURCE_DIR}/src/systems/world.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/systems/world.cpp

	${CMAKE_CURRENT_SOURCE_DIR}/src/systems/blueprint.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/systems/blueprint.cpp

	${CMAKE_CURRENT_SOURCE_DIR}/src/systems/asset.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/systems/asset.cpp

	${CMAKE_CURRENT_SOURCE_DIR}/src/systems/flip_animation.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/systems/flip_animation.cpp

	${CMAKE_CURRENT_SOURCE_DIR}/src/systems/debug.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/systems/debug.cpp

	# ============================
	# SCENE
	# ============================

	# 游戏场景
	${CMAKE_CURRENT_SOURCE_DIR}/src/scene/scene.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/scene/scene.cpp
	# 游戏退出场景
	${CMAKE_CURRENT_SOURCE_DIR}/src/scene/quit_game.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/scene/quit_game.cpp
	# 游戏主场景
	${CMAKE_CURRENT_SOURCE_DIR}/src/scene/main.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/scene/main.cpp

	# ============================
	# GAME
	# ============================

	# 宏定义
	${CMAKE_CURRENT_SOURCE_DIR}/src/game/defines.hpp
	# 常量定义
	${CMAKE_CURRENT_SOURCE_DIR}/src/game/constants.hpp
	# 命名实体
	${CMAKE_CURRENT_SOURCE_DIR}/src/game/names.hpp
	# 测试用
	${CMAKE_CURRENT_SOURCE_DIR}/src/game/playground.hpp

	# 游戏
	${CMAKE_CURRENT_SOURCE_DIR}/src/game/game.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/game/game.cpp

	# ============================
	# MAIN
	# ============================

	${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
)

target_include_directories(
		${PROJECT_NAME}

		PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}/src
		${CMAKE_CURRENT_BINARY_DIR}
)

target_compile_options(
		${PROJECT_NAME}
		PUBLIC

		${PD_COMPILE_FLAGS}
)

target_compile_features(
		${PROJECT_NAME}
		PRIVATE
		cxx_std_23
)

target_link_libraries(
		${PROJECT_NAME}
		PRIVATE

		PrometheusCore
		${PD_SFML_LIBRARIES}
		imgui::imgui
		EnTT::EnTT
		box2d::box2d
		spdlog::spdlog
)

# 复制 assets 文件夹到输出目录
add_custom_target(
    copy_resources 
    ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different ${CMAKE_SOURCE_DIR}/assets ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets
)

add_dependencies(${PROJECT_NAME} copy_resources)
