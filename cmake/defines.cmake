# ===================================================================================================
# PLATFORM

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
	set(
			PD_PLATFORM_WINDOWS
			ON
			CACHE BOOL
			""
	)
	# ==> macro.hpp
	set(PD_PLATFORM_NAME PD_PLATFORM_WINDOWS)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
	set(
			PD_PLATFORM_LINUX
			ON
			CACHE BOOL
			""
	)
	# ==> macro.hpp
	set(PD_PLATFORM_NAME PD_PLATFORM_LINUX)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
	set(
			PD_PLATFORM_DARWIN
			ON
			CACHE BOOL
			""
	)
	# ==> macro.hpp
	set(PD_PLATFORM_NAME PD_PLATFORM_DARWIN)
else ()
	message(FATAL_ERROR "[BPP] Unknown Platform: ${CMAKE_SYSTEM_NAME}")
endif (CMAKE_SYSTEM_NAME STREQUAL "Windows")

# ===================================================================================================
# ARCHITECTURE

if (CMAKE_SYSTEM_PROCESSOR)
	string(TOLOWER "${CMAKE_SYSTEM_PROCESSOR}" PROC_LOWER)

	if (PROC_LOWER MATCHES "amd64|x86_64|x64|win64")
		set(
				PD_ARCH_X64
				ON
				CACHE BOOL
				""
		)
		# ==> macro.hpp
		set(PD_ARCH_NAME PD_ARCH_X64)
	elseif (PROC_LOWER MATCHES "i.86|i86|i686|i586|i486|i386|x86")
		set(
				PD_ARCH_X86
				ON
				CACHE BOOL
				""
		)
		# ==> macro.hpp
		set(PD_ARCH_NAME PD_ARCH_X86)
	elseif (PROC_LOWER MATCHES "aarch64|arm64|armv8")
		set(
				PD_ARCH_ARM64
				ON
				CACHE BOOL
				""
		)
		# ==> macro.hpp
		set(PD_ARCH_NAME PD_ARCH_ARM64)
	elseif (PROC_LOWER MATCHES "arm.*")
		set(
				PD_ARCH_ARM
				ON
				CACHE BOOL
				""
		)
		# ==> macro.hpp
		set(PD_ARCH_NAME PD_ARCH_ARM)
	else ()
		set(
				PD_ARCH_UNKNOWN
				ON
				CACHE BOOL
				""
		)
		# ==> macro.hpp
		set(PD_ARCH_NAME PD_ARCH_UNKNOWN)
	endif ()
else ()
	set(
			PD_ARCH_UNKNOWN
			ON
			CACHE BOOL
			""
	)
	# ==> macro.hpp
	set(PD_ARCH_NAME PD_ARCH_UNKNOWN)
endif (CMAKE_SYSTEM_PROCESSOR)

# ===================================================================================================
# COMPILER

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	set(
			PD_COMPILER_MSVC
			ON
			CACHE BOOL
			""
	)
	# ==> macro.hpp
	set(PD_COMPILER_NAME PD_COMPILER_MSVC)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	if (CMAKE_CXX_SIMULATE_ID STREQUAL "MSVC")
		set(
				PD_COMPILER_CLANG_CL
				ON
				CACHE BOOL
				""
		)
		# ==> macro.hpp
		set(PD_COMPILER_NAME PD_COMPILER_CLANG_CL)
	else ()
		set(
				PD_COMPILER_CLANG
				ON
				CACHE BOOL
				""
		)
		# ==> macro.hpp
		set(PD_COMPILER_NAME PD_COMPILER_CLANG)
	endif (CMAKE_CXX_SIMULATE_ID STREQUAL "MSVC")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	set(
			PD_COMPILER_GNU
			ON
			CACHE BOOL
			""
	)
	# ==> macro.hpp
	set(PD_COMPILER_NAME PD_COMPILER_GNU)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
	set(
			PD_COMPILER_CLANG_APPLE
			ON
			CACHE BOOL
			""
	)
	# ==> macro.hpp
	set(PD_COMPILER_NAME PD_COMPILER_CLANG_APPLE)
else ()
	message(FATAL_ERROR "[BPP] Unknown compiler: ${CMAKE_CXX_COMPILER}")
endif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")

# ===================================================================================================
# COMPILE FLAGS

if (PD_COMPILER_MSVC)
	set(
			PD_COMPILE_FLAGS
			"/D_CRT_SECURE_NO_WARNINGS"
			"/DNOMINMAX"
			"/DWIN32_LEAN_AND_MEAN"
			"/DVC_EXTRALEAN"
			"/DSTRICT"
			"/utf-8"
			"/W3"
			"/WX"
			"/Zc:preprocessor"
			"/permissive-"
			CACHE STRING
			""
	)
elseif (PD_COMPILER_CLANG_CL)
	set(
			PD_COMPILE_FLAGS
			"/D_CRT_SECURE_NO_WARNINGS"
			"/DNOMINMAX"
			"/DWIN32_LEAN_AND_MEAN"
			"/DVC_EXTRALEAN"
			"/DSTRICT"
			"/utf-8"
			"/W3"
			"/WX"
			"/permissive-"
			CACHE STRING
			""
	)
elseif (PD_COMPILER_CLANG)
	set(
			PD_COMPILE_FLAGS
			"-Wall"
			"-Wextra"
			"-Wpedantic"
			"-Werror"
			"-Wconversion"
			"-Wshadow"
			"-Wold-style-cast"
			"-Wnull-dereference"
			"-Wdouble-promotion"
			CACHE STRING
			""
	)
elseif (PD_COMPILER_GNU)
	set(
			PD_COMPILE_FLAGS
			"-Wall"
			"-Wextra"
			"-Wpedantic"
			"-Werror"
			"-Wconversion"
			"-Wshadow"
			"-Wold-style-cast"
			"-Wnull-dereference"
			"-Wlogical-op"
			"-Wduplicated-cond"
			"-Wduplicated-branches"
			CACHE STRING
			""
	)
elseif (PD_COMPILER_CLANG_APPLE)
	set(
			PD_COMPILE_FLAGS
			"-Wall"
			"-Wextra"
			"-Wpedantic"
			"-Werror"
			"-Wconversion"
	)
endif (PD_COMPILER_MSVC)

# ===================================================================================================
# GIT

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/git.cmake)

# ===================================================================================================
# OUTPUT INFO

message(STATUS "")
message(STATUS "================================================================")
message(STATUS "  ${PROJECT_NAME} - v${PD_VERSION}")
message(STATUS "================================================================")
# PLATFORM + ARCHITECTURE + COMPILER + COMPILE FLAGS
message(STATUS "  Platform: ${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS "  CMake Version: ${CMAKE_VERSION}")
message(STATUS "  Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "  Compile Flags: ${PD_COMPILE_FLAGS}")
message(STATUS "  Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  Build Test: ${CLP_TEST}")
# GIT
if (PROJECT_IS_TOP_LEVEL)
	message(STATUS "  Git: ")
	message(STATUS "      Branch: ${PD_GIT_BRANCH_NAME}")
	message(STATUS "      Commit: ${PD_GIT_COMMIT_HASH}${PD_GIT_DIRTY_FLAG}")
	message(STATUS "      Date: ${PD_GIT_COMMIT_DATE}")
	message(STATUS "      Tag: ${PD_GIT_TAG}")
	message(STATUS "      Status: ${PD_GIT_DIRTY_STATUS}")
	message(STATUS "================================================================")
	message(STATUS "")
else ()
endif (PROJECT_IS_TOP_LEVEL)

