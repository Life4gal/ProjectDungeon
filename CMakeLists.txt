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

	# 如果后续不使用string/filesystem::path作为key,而是改用ID的话,这两个文件会有用
	${CMAKE_CURRENT_SOURCE_DIR}/src/utility/string_to_id.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/utility/path_to_id.hpp

	# ============================
	# CONFIG
	# ============================
	
	# 配置文件读取器
	${CMAKE_CURRENT_SOURCE_DIR}/src/config/config_reader.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/config/config_reader.cpp

	# 集合
	${CMAKE_CURRENT_SOURCE_DIR}/src/config/set.hpp
	# 渲染层级
	${CMAKE_CURRENT_SOURCE_DIR}/src/config/render_layer.hpp
	# 碰撞掩码
	${CMAKE_CURRENT_SOURCE_DIR}/src/config/collision_mask.hpp
	# 一些(枚举)类型定义
	${CMAKE_CURRENT_SOURCE_DIR}/src/config/types.hpp
	# 位置
	${CMAKE_CURRENT_SOURCE_DIR}/src/config/position.hpp

	# 地下城 -- 动画
	${CMAKE_CURRENT_SOURCE_DIR}/src/config/animation.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/config/animation.cpp
	# 地下城 -- 瓦片
	${CMAKE_CURRENT_SOURCE_DIR}/src/config/tile.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/config/tile.cpp
	# 地下城 -- 房间
	${CMAKE_CURRENT_SOURCE_DIR}/src/config/room.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/config/room.cpp
	# 地下城 -- 关卡
	${CMAKE_CURRENT_SOURCE_DIR}/src/config/level.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/config/level.cpp
	# 地下城
	${CMAKE_CURRENT_SOURCE_DIR}/src/config/dungeon.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/config/dungeon.cpp

	# 玩家
	${CMAKE_CURRENT_SOURCE_DIR}/src/config/player.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/config/player.cpp
	# 敌人
	${CMAKE_CURRENT_SOURCE_DIR}/src/config/enemy.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/config/enemy.cpp

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

	# 游戏信息组件
	${CMAKE_CURRENT_SOURCE_DIR}/src/components/game.hpp
	# 世界信息组件
	${CMAKE_CURRENT_SOURCE_DIR}/src/components/world.hpp
	# 物理世界信息组件
	${CMAKE_CURRENT_SOURCE_DIR}/src/components/physics_world.hpp
	
	# 地下城 -- 房间组件
	${CMAKE_CURRENT_SOURCE_DIR}/src/components/room.hpp
	# 地下城 -- 关卡组件
	${CMAKE_CURRENT_SOURCE_DIR}/src/components/level.hpp
	# 地下城 -- 地下城组件
	${CMAKE_CURRENT_SOURCE_DIR}/src/components/dungeon.hpp

	# 名称组件
	${CMAKE_CURRENT_SOURCE_DIR}/src/components/name.hpp
	# 变换组件
	${CMAKE_CURRENT_SOURCE_DIR}/src/components/transform.hpp
	# 物理体组件
	${CMAKE_CURRENT_SOURCE_DIR}/src/components/physics_body.hpp
	# 动画组件
	${CMAKE_CURRENT_SOURCE_DIR}/src/components/animation.hpp
	# 渲染组件
	${CMAKE_CURRENT_SOURCE_DIR}/src/components/render.hpp
	# 玩家/敌人实体通用组件
	${CMAKE_CURRENT_SOURCE_DIR}/src/components/actor.hpp
	# 敌人组件
	#${CMAKE_CURRENT_SOURCE_DIR}/src/components/enemy.hpp
	# 玩家组件
	${CMAKE_CURRENT_SOURCE_DIR}/src/components/player.hpp

	# ============================
	# SYSTEMS
	# ============================
			
	# ==========
	# HELPER
	# ==========
	
	${CMAKE_CURRENT_SOURCE_DIR}/src/systems/helper/font_manager.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/systems/helper/font_manager.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/systems/helper/texture_manager.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/systems/helper/texture_manager.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/systems/helper/sound_manager.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/systems/helper/sound_manager.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/systems/helper/music_manager.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/systems/helper/music_manager.cpp

	${CMAKE_CURRENT_SOURCE_DIR}/src/systems/helper/world.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/systems/helper/world.cpp

	${CMAKE_CURRENT_SOURCE_DIR}/src/systems/helper/physics_world.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/systems/helper/physics_world.cpp

	# 为实体附加动画组件
	${CMAKE_CURRENT_SOURCE_DIR}/src/systems/helper/animation.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/systems/helper/animation.cpp
	# 为实体附加渲染组件(不含变换组件的实体即使有渲染组件也不会渲染)(不含动画组件的实体不会更新渲染组件的纹理)
	${CMAKE_CURRENT_SOURCE_DIR}/src/systems/helper/render.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/systems/helper/render.cpp
	# 为实体附加物理组件(不含变换组件的实体即使有物理组件也不会参与物理世界的更新)
	${CMAKE_CURRENT_SOURCE_DIR}/src/systems/helper/physics_body.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/systems/helper/physics_body.cpp
	# 为实体附加变换组件
	${CMAKE_CURRENT_SOURCE_DIR}/src/systems/helper/transform.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/systems/helper/transform.cpp
	# 为实体附加actor组件
	${CMAKE_CURRENT_SOURCE_DIR}/src/systems/helper/actor.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/systems/helper/actor.cpp

	# 地下城 -- 房间
	${CMAKE_CURRENT_SOURCE_DIR}/src/systems/helper/room.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/systems/helper/room.cpp
	# 地下城 -- 关卡
	${CMAKE_CURRENT_SOURCE_DIR}/src/systems/helper/level.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/systems/helper/level.cpp
	# 地下城 -- 地下城
	${CMAKE_CURRENT_SOURCE_DIR}/src/systems/helper/dungeon.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/systems/helper/dungeon.cpp

	# 敌人
	#${CMAKE_CURRENT_SOURCE_DIR}/src/systems/helper/enemy.hpp
	#${CMAKE_CURRENT_SOURCE_DIR}/src/systems/helper/enemy.cpp
	# 玩家
	${CMAKE_CURRENT_SOURCE_DIR}/src/systems/helper/player.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/systems/helper/player.cpp

	# ==========
	# UPDATE
	# ==========
		
	# 更新世界(主要是涉及帧时长/游戏历时等信息)
	${CMAKE_CURRENT_SOURCE_DIR}/src/systems/update/world.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/systems/update/world.cpp
	
	# 更新房间状态(确定敌人是否被清空,如果清空则打开所有门)
	${CMAKE_CURRENT_SOURCE_DIR}/src/systems/update/update_room.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/systems/update/update_room.cpp

	# 应用玩家移动
	${CMAKE_CURRENT_SOURCE_DIR}/src/systems/update/apply_player_movement.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/systems/update/apply_player_movement.cpp

	# 更新物理世界(主要是调用box2d的step函数)
	${CMAKE_CURRENT_SOURCE_DIR}/src/systems/update/physics_world.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/systems/update/physics_world.cpp

	# 同步物理世界更新的变换(transform)信息
	${CMAKE_CURRENT_SOURCE_DIR}/src/systems/update/sync_transform.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/systems/update/sync_transform.cpp

	# 处理接触事件
	${CMAKE_CURRENT_SOURCE_DIR}/src/systems/update/process_contact_events.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/systems/update/process_contact_events.cpp

	# 更新动画帧(同时根据动画帧设置渲染纹理)
	${CMAKE_CURRENT_SOURCE_DIR}/src/systems/update/animation.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/systems/update/animation.cpp

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

	# 常量定义
	${CMAKE_CURRENT_SOURCE_DIR}/src/game/constants.hpp
	# 实体创建/销毁时输出日志
	${CMAKE_CURRENT_SOURCE_DIR}/src/game/log_entity.hpp

	# 测试用
	${CMAKE_CURRENT_SOURCE_DIR}/src/game/playground.hpp

	# 游戏
	${CMAKE_CURRENT_SOURCE_DIR}/src/game/game.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/game/game.cpp

	# ============================
	# MAIN
	# ============================

	# 宏定义
	${CMAKE_CURRENT_SOURCE_DIR}/src/defines.hpp
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
