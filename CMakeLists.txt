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
	${PROJECT_NAME}-new
		
	# ============================
	# EXTERNAL
	# ============================

	${CMAKE_CURRENT_SOURCE_DIR}/src-new/external/imgui-SFML.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/external/imgui-SFML.cpp

	# ============================
	# UTILITY
	# ============================
	
	# 字符串哈希(支持异构)
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/utility/string_hash.hpp
	# 固定矩阵
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/utility/fixed_matrix.hpp
	# 物理定义
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/utility/physics.hpp

	# ============================
	# MANAGER
	# 管理器
	# ============================

	# 随机数生成器
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/manager/random.hpp
	# 资源管理器
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/manager/resource_fwd.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/manager/resource.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/manager/resource.cpp
	# 音频播放器
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/manager/audio_player.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/manager/audio_player.cpp
	# 事件分发器
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/manager/event.hpp
	# 
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/manager/i18n.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/manager/i18n.cpp
	# (全局)计时器
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/manager/clock.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/manager/clock.cpp
	
	# ============================
	# VAR
	# 
	# ============================

	# 窗口
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/var/window.hpp
	# 音频
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/var/audio.hpp
	# 按键动作映射
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/var/key_action_mapper.hpp

	# ============================
	# EVENT
	# 事件
	# ============================
	
	# 窗口
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/event/window.hpp
	# 场景
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/event/scene.hpp

	# 相机
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/event/camera.hpp

	# 物理
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/event/physics.hpp
	
	# Actor
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/event/actor.hpp

	# 地下城 -- 关卡 -- 房间 -- 门
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/event/door.hpp
	# 地下城 -- 关卡 -- 房间
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/event/room.hpp

	# ============================
	# BLUEPRINT
	# 蓝图
	# ============================
	
	# 布局
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/blueprint/detail/layout.hpp
	# 标量
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/blueprint/detail/scalar.hpp

	# 变换
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/blueprint/detail/transform.hpp
	# 渲染
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/blueprint/detail/render.hpp
	# 碰撞体
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/blueprint/detail/collision.hpp
	# 属性
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/blueprint/detail/property.hpp
	# AI
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/blueprint/detail/ai.hpp
	# 名称
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/blueprint/detail/name.hpp

	# 计划任务
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/blueprint/detail/scheduled_task.hpp

	# (枚举)定义
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/blueprint/def.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/blueprint/def_name.hpp

	# 粒子
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/blueprint/particle.hpp
	# 粒子发射器
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/blueprint/particle_emitter.hpp
	# 飞弹
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/blueprint/projectile.hpp

	# 敌人
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/blueprint/enemy.hpp
	# NPC
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/blueprint/npc.hpp
	# 玩家
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/blueprint/player.hpp

	# 地下城 -- 关卡 -- 房间 -- 瓦片
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/blueprint/tile.hpp
	# 地下城 -- 关卡 -- 房间 -- 门
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/blueprint/door.hpp
	# 地下城 -- 关卡 -- 房间 -- 边界
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/blueprint/bounding.hpp
	# 地下城 -- 关卡 -- 房间
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/blueprint/room.hpp
	# 地下城 -- 关卡
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/blueprint/level.hpp
	# 地下城
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/blueprint/dungeon.hpp

	# ============================
	# DESIGNER
	# 生成蓝图
	# ============================
	
	# 粒子
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/designer/particle.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/designer/particle.cpp
	# 粒子发射器
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/designer/particle_emitter.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/designer/particle_emitter.cpp
	# 飞弹
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/designer/projectile.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/designer/projectile.cpp

	# 敌人
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/designer/enemy.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/designer/enemy.cpp
	# NPC
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/designer/npc.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/designer/npc.cpp
	# 玩家
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/designer/player.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/designer/player.cpp

	# 地下城 -- 关卡 -- 房间 -- 瓦片
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/designer/tile.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/designer/tile.cpp
	# 地下城 -- 关卡 -- 房间 -- 门
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/designer/door.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/designer/door.cpp
	# 地下城 -- 关卡 -- 房间
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/designer/room.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/designer/room.cpp
	# 地下城 -- 关卡
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/designer/level.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/designer/level.cpp
	# 地下城
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/designer/dungeon.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/designer/dungeon.cpp
	
	# ============================
	# COMPONENT
	# 实体的组件
	# ============================
	
	# 标签
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/component/tags.hpp

	# 变换
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/component/transform.hpp
	# 渲染
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/component/render.hpp
	# 渲染特效
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/component/render_effect.hpp
	# 渲染器
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/component/renderer.hpp
	# 碰撞体
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/component/collision.hpp
	# 属性
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/component/property.hpp
	# 伤害统计
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/component/damage_statistics.hpp
	# AI移动行为
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/component/move_behavior.hpp
	# AI
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/component/ai.hpp
	# 名称
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/component/name.hpp

	# 计划任务
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/component/scheduled_task.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/component/scheduled_task_context.hpp

	# 粒子
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/component/particle.hpp
	# 粒子发射器
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/component/particle_emitter.hpp
	# 飞弹
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/component/projectile.hpp

	# 敌人
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/component/enemy.hpp
	# NPC
	#${CMAKE_CURRENT_SOURCE_DIR}/src-new/component/npc.hpp
	# 玩家
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/component/player.hpp

	# 地下城 -- 关卡 -- 房间 -- 瓦片
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/component/tile.hpp
	# 地下城 -- 关卡 -- 房间 -- 门
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/component/door.hpp
	# 地下城 -- 关卡 -- 房间 -- 房间边界
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/component/bounding.hpp
	# 地下城 -- 关卡 -- 房间
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/component/room.hpp
	# 地下城 -- 关卡
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/component/level.hpp
	# 地下城
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/component/dungeon.hpp

	# 相机(可视区域)
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/component/camera.hpp
	# 屏幕
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/component/screen.hpp
	# 玩家控制器
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/component/player_controller.hpp
	
	# ============================
	# SYSTEM -- ASSEMBLY
	# 实体 + 蓝图 --> 组件
	# ============================

	# 变换
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/assembly/transform.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/assembly/transform.cpp
	# 渲染
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/assembly/render.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/assembly/render.cpp
	# 渲染特效
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/assembly/render_effect.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/assembly/render_effect.cpp
	# 碰撞体
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/assembly/collision.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/assembly/collision.cpp
	# 属性
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/assembly/property.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/assembly/property.cpp
	# AI
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/assembly/ai.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/assembly/ai.cpp
	# 名称
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/assembly/name.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/assembly/name.cpp

	# 粒子发射器
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/assembly/particle_emitter.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/assembly/particle_emitter.cpp

	# ============================
	# SYSTEM -- FACTORY
	# 蓝图 --> 实体
	# ============================
	
	# 计划任务
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/factory/scheduled_task.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/factory/scheduled_task.cpp

	# 粒子
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/factory/particle.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/factory/particle.cpp
	# 飞弹
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/factory/projectile.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/factory/projectile.cpp

	# 敌人
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/factory/enemy.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/factory/enemy.cpp
	# 玩家
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/factory/player.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/factory/player.cpp

	# 地下城 -- 关卡 -- 房间 -- 瓦片
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/factory/tile.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/factory/tile.cpp
	# 地下城 -- 关卡 -- 房间 -- 门
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/factory/door.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/factory/door.cpp
	# 地下城 -- 关卡 -- 房间 -- 房间边界
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/factory/bounding.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/factory/bounding.cpp
	# 地下城 -- 关卡 -- 房间
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/factory/room.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/factory/room.cpp
	# 地下城 -- 关卡
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/factory/level.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/factory/level.cpp
	# 地下城
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/factory/dungeon.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/factory/dungeon.cpp
	
	# ============================
	# SYSTEM -- ACCESSOR
	# registry + entity                       --> read component
	# registry + entity + new-component-value --> write component
	# ============================
	
	# 变换
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/accessor/transform.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/accessor/transform.cpp
	# 动态精灵
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/accessor/dynamic_sprite.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/accessor/dynamic_sprite.cpp
	# 碰撞体
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/accessor/collision.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/accessor/collision.cpp
	# 属性
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/accessor/property.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/accessor/property.cpp
	
	# 相机(可视区域)
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/accessor/camera.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/accessor/camera.cpp

	# 玩家控制器
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/accessor/player_controller.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/accessor/player_controller.cpp

	# ============================
	# SYSTEM -- HELPER
	# 辅助读写组件(避免直接访问registry)
	# ============================

	# 动态精灵
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/helper/dynamic_sprite.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/helper/dynamic_sprite.cpp

	# 属性
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/helper/property.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/helper/property.cpp

	# 玩家控制器
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/helper/player_controller.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/helper/player_controller.cpp
	# 作弊器
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/helper/cheat.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/helper/cheat.cpp

	# 飞弹
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/helper/projectile.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/helper/projectile.cpp

	# 敌人
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/helper/enemy.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/helper/enemy.cpp
	# 玩家
	#${CMAKE_CURRENT_SOURCE_DIR}/src-new/helper/player.hpp
	#${CMAKE_CURRENT_SOURCE_DIR}/src-new/helper/player.cpp
	# 地下城 -- 关卡 -- 房间 -- 门
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/helper/door.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/helper/door.cpp
	# 地下城 -- 关卡 -- 房间
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/helper/room.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/helper/room.cpp
	# 地下城 -- 关卡
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/helper/level.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/helper/level.cpp

	# ============================
	# SYSTEM -- UNDERTAKER
	# 处理那些不能*自动*销毁的组件
	# ============================
	
	# 计划任务
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/undertaker/scheduled_task.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/undertaker/scheduled_task.cpp

	# 碰撞体
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/undertaker/collision.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/undertaker/collision.cpp

	# ============================
	# SYSTEM -- UPDATE
	# 帧更新系统
	# ============================
	
	# 墓地
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/update/graveyard.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/update/graveyard.cpp

	# 房间(相机)监控
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/update/room_guardian.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/update/room_guardian.cpp

	# 新实体标记
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/update/new_entity_tag.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/update/new_entity_tag.cpp

	# 玩家控制器
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/update/player_controller.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/update/player_controller.cpp

	# (AI)移动行为
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/update/move_behavior.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/update/move_behavior.cpp

	# 物理世界
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/update/physics_world.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/update/physics_world.cpp
	# 同步物理世界更新的变换信息
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/update/sync_physics_transform.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/update/sync_physics_transform.cpp
	# 处理物理世界事件
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/update/process_physics_events.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/update/process_physics_events.cpp
	
	# 检测房间内存活的敌人
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/update/room_alive_check.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/update/room_alive_check.cpp

	# 计划任务
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/update/scheduled_task.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/update/scheduled_task.cpp

	# 粒子发射器
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/update/particle_emitter.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/update/particle_emitter.cpp

	# 动态精灵
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/update/dynamic_sprite.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/update/dynamic_sprite.cpp

	# 渲染特效
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/update/render_effect.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/update/render_effect.cpp

	# ============================
	# SYSTEM -- RENDER
	# 帧渲染系统
	# ============================
	
	# 收集渲染项
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/render/collect_render_item.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/render/collect_render_item.cpp

	# 应用渲染效果
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/render/apply_render_effect.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/render/apply_render_effect.cpp

	# 构建渲染队列
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/render/build_render_queue.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/render/build_render_queue.cpp

	# 提交渲染队列
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/render/commit_render_queue.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/render/commit_render_queue.cpp

	# 玩家目标状态
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/render/player_target_status.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/render/player_target_status.cpp

	# 玩家状态
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/render/player_status.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/render/player_status.cpp

	# ============================
	# MENU
	# 菜单(各个场景用到的菜单界面的封装)
	# ============================

	# 菜单基类(其实这个基类没啥必要)
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/menu/menu.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/menu/menu.cpp

	# 主菜单
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/menu/main.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/menu/main.cpp

	# 游戏内暂停菜单
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/menu/pause.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/menu/pause.cpp

	# ============================
	# SCENE
	# 场景
	# ============================

	# 场景类型
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/scene/category.hpp

	# 游戏场景基类
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/scene/scene.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/scene/scene.cpp

	# 游戏主菜单场景
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/scene/main_menu.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/scene/main_menu.cpp
	
	# 游戏主场景
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/scene/game.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/scene/game.cpp

	# ============================
	# MAIN
	# ============================

	${CMAKE_CURRENT_SOURCE_DIR}/src-new/game.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/game.cpp
	
	${CMAKE_CURRENT_SOURCE_DIR}/src-new/main.cpp
)

target_include_directories(
		${PROJECT_NAME}-new

		PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}/src-new
		${CMAKE_CURRENT_BINARY_DIR}
)

target_compile_options(
		${PROJECT_NAME}-new
		PUBLIC

		${PD_COMPILE_FLAGS}
)

target_compile_features(
		${PROJECT_NAME}-new
		PRIVATE
		cxx_std_23
)

target_link_libraries(
		${PROJECT_NAME}-new
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

add_dependencies(${PROJECT_NAME}-new copy_resources)
