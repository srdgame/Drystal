cmake_minimum_required(VERSION 2.8.5 FATAL_ERROR)
project(drystal)

set(DRYSTAL_VERSION "Drystal 1.1")

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(Utils)
include(CMakeDependentOption)

check_compilers_unknown()
check_git_submodules_initialized()

option(BUILD_MANPAGES        "Build manpages" ON)
option(BUILD_ENABLE_COVERAGE "Enable code coverage" OFF)
option(BUILD_PHYSICS         "Enable physics module" ON)
option(BUILD_WEB             "Enable web module" ON)
option(BUILD_AUDIO           "Enable audio module" ON)
option(BUILD_GRAPHICS        "Enable graphics module" ON)
option(BUILD_UTILS           "Enable utils module" ON)
option(BUILD_LFS             "Enable lfs module" ON)

cmake_dependent_option(BUILD_PARTICLE     "Enable particle module (needs BUILD_GRAPHICS as dependency)" ON "BUILD_GRAPHICS" OFF)
cmake_dependent_option(BUILD_STORAGE      "Enable storage module (needs BUILD_UTILS as dependency)" ON "BUILD_UTILS" OFF)
cmake_dependent_option(BUILD_FONT         "Enable font module (needs BUILD_GRAPHICS as dependency)" ON "BUILD_GRAPHICS" OFF)
cmake_dependent_option(BUILD_LIVECODING   "Enable livecoding (available only on Linux)" ON "NOT EMSCRIPTEN" OFF)

if(BUILD_LIVECODING AND NOT ${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	message(FATAL_ERROR "Cannot enable BUILD_LIVECODING: Linux only")
endif()

if (EMSCRIPTEN)
	set(LINK_FLAGS "${LINK_FLAGS} -s TOTAL_MEMORY=67108864")
	set(LINK_FLAGS "${LINK_FLAGS} --memory-init-file 1")
	set(LINK_FLAGS "${LINK_FLAGS} -s NO_EXIT_RUNTIME=1")
	set(LINK_FLAGS "${LINK_FLAGS} -s ASM_JS=1")
	set(LINK_FLAGS "${LINK_FLAGS} -s ERROR_ON_UNDEFINED_SYMBOLS=1")

	set(LINK_FLAGS_RELEASE "${LINK_FLAGS_RELEASE} -s DISABLE_EXCEPTION_CATCHING=1")
	set(LINK_FLAGS_RELEASE "${LINK_FLAGS_RELEASE} -s AGGRESSIVE_VARIABLE_ELIMINATION=1")
	set(LINK_FLAGS_RELEASE "${LINK_FLAGS_RELEASE} -s ASSERTIONS=0")

	set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} ${LINK_FLAGS} ${LINK_FLAGS_RELEASE}")
	set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} ${LINK_FLAGS} ${LINK_FLAGS_RELEASE}")
	set(CMAKE_EXE_LINKER_FLAGS_MINSIZEREL "${CMAKE_EXE_LINKER_FLAGS_MINSIZEREL} ${LINK_FLAGS} ${LINK_FLAGS_RELEASE}")
	set(CMAKE_SHARED_LINKER_FLAGS_MINSIZEREL "${CMAKE_SHARED_LINKER_FLAGS_MINSIZEREL} ${LINK_FLAGS} ${LINK_FLAGS_RELEASE}")
	set(CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO "${CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO} ${LINK_FLAGS} ${LINK_FLAGS_DEBUG}")
	set(CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO "${CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO} ${LINK_FLAGS} ${LINK_FLAGS_DEBUG}")
endif()

if(BUILD_MANPAGES AND NOT EMSCRIPTEN)
	add_subdirectory(doc/man)
endif()
add_subdirectory(tools)

add_subdirectory(external)
add_subdirectory(src)
add_subdirectory(misc/shell-completion/zsh/)
