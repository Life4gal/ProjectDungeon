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
	${CMAKE_CURRENT_SOURCE_DIR}/src/utility/string_hash.hpp

	# JSON读取器
	${CMAKE_CURRENT_SOURCE_DIR}/src/utility/json_reader.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/utility/json_reader.cpp

	# 如果后续不使用string/filesystem::path作为key,而是改用ID的话,这两个文件会有用
	${CMAKE_CURRENT_SOURCE_DIR}/src/utility/string_to_id.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/utility/path_to_id.hpp

	# ============================
	# CONFIG
	# ============================

	# 动画
	${CMAKE_CURRENT_SOURCE_DIR}/src/config/animation.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/config/animation.cpp
	
	# ============================
	# BLUEPRINT
	# ============================

	# 动画
	${CMAKE_CURRENT_SOURCE_DIR}/src/blueprint/animation.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/blueprint/animation.cpp

	# ============================
	# ASSET
	# ============================

	# entt::resource_cache的包装类
	${CMAKE_CURRENT_SOURCE_DIR}/src/asset/resource_cache.hpp

	# 字体资源
	${CMAKE_CURRENT_SOURCE_DIR}/src/asset/font.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/asset/font.cpp
	# 纹理资源
	${CMAKE_CURRENT_SOURCE_DIR}/src/asset/texture.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/asset/texture.cpp
	# 音效资源
	${CMAKE_CURRENT_SOURCE_DIR}/src/asset/sound.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/asset/sound.cpp
	# 音乐资源
	${CMAKE_CURRENT_SOURCE_DIR}/src/asset/music.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/asset/music.cpp

	# ============================
	# COMPONENTS
	# ============================

	# 游戏信息
	${CMAKE_CURRENT_SOURCE_DIR}/src/components/game.hpp
	# 世界信息
	${CMAKE_CURRENT_SOURCE_DIR}/src/components/world.hpp
	# 物理世界信息
	${CMAKE_CURRENT_SOURCE_DIR}/src/components/physics_world.hpp

	# 位置/大小/旋转等信息
	${CMAKE_CURRENT_SOURCE_DIR}/src/components/transform.hpp

	# 动画信息
	${CMAKE_CURRENT_SOURCE_DIR}/src/components/animation.hpp
		
	# 描述一个实体要被渲染需要哪些信息
	${CMAKE_CURRENT_SOURCE_DIR}/src/components/render.hpp

	# ============================
	# CTX (直接上下文访问封装,避免出错)
	# ============================

	# components/world
	${CMAKE_CURRENT_SOURCE_DIR}/src/ctx/world.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/ctx/world.cpp

	# components/physics_world
	${CMAKE_CURRENT_SOURCE_DIR}/src/ctx/physics_world.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/ctx/physics_world.cpp
	
	# components/animation
	${CMAKE_CURRENT_SOURCE_DIR}/src/ctx/animation.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/ctx/animation.cpp

	# asset/font
	${CMAKE_CURRENT_SOURCE_DIR}/src/ctx/font.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/ctx/font.cpp
	# asset/texture
	${CMAKE_CURRENT_SOURCE_DIR}/src/ctx/texture.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/ctx/texture.cpp
	# asset/sound
	${CMAKE_CURRENT_SOURCE_DIR}/src/ctx/sound.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/ctx/sound.cpp
	# asset/music
	${CMAKE_CURRENT_SOURCE_DIR}/src/ctx/music.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/ctx/music.cpp

	# ============================
	# SYSTEMS
	# ============================
				
	# ==========
	# INITIALIZE
	# ==========

	# 载入所有蓝图
	${CMAKE_CURRENT_SOURCE_DIR}/src/systems/initialize/blueprint.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/systems/initialize/blueprint.cpp	

	# 载入蓝图所需的资源
	${CMAKE_CURRENT_SOURCE_DIR}/src/systems/initialize/asset.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/systems/initialize/asset.cpp	

	# 测试用
	${CMAKE_CURRENT_SOURCE_DIR}/src/systems/initialize/debug.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/systems/initialize/debug.cpp	

	# ==========
	# UPDATE
	# ==========
		
	# 更新世界(主要是涉及帧时长/游戏历时等信息)
	${CMAKE_CURRENT_SOURCE_DIR}/src/systems/update/world.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/systems/update/world.cpp

	# 更新动画帧(同时根据动画帧设置渲染纹理)
	${CMAKE_CURRENT_SOURCE_DIR}/src/systems/update/animation.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/systems/update/animation.cpp

	# 测试用
	${CMAKE_CURRENT_SOURCE_DIR}/src/systems/update/debug.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/systems/update/debug.cpp	

	# ==========
	# RENDER
	# ==========

	# 渲染实体
	${CMAKE_CURRENT_SOURCE_DIR}/src/systems/render/entity.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/systems/render/entity.cpp

	# 测试用
	${CMAKE_CURRENT_SOURCE_DIR}/src/systems/render/debug.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/systems/render/debug.cpp	

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
