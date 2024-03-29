
#
## Input:
## - ADMUTIL_NONLIB
##   Whether or not the library should be embedded into a project,
##   instead of being used as a static library
#
## Output:
## - ADMUTIL_INCLUDE_DIRECTORY
##   When you use the library, you use this.
##   No need to do find_package( AdmUtils ) or anything like that
#
## You can expect .lib files in the libs/ folder after installing
#

## Minimum is 3.16 for PCH support
cmake_minimum_required( VERSION 3.16 )

## C++17's filesystem and inline static initialisers are pretty nice
set( CMAKE_CXX_STANDARD 17 )

option( ADMUTIL_NONLIB "Bundle the library into another project instead of compiling a .lib; will define ADMUTIL_ALL_SRC which you can then include in your project" OFF )
option( ADMUTIL_USE_SSE41 "Use the SSE 4.1 instruction set (should be supported on most CPUs)" ON )
if ( UNIX )
	option( ADMUTIL_USE_WAYLAND "Use Wayland instead of X11" OFF )
endif()

if ( NOT ADMUTIL_NONLIB )
	project( AdmUtils )
endif()

set( ADMUTIL_ROOT ${CMAKE_CURRENT_SOURCE_DIR} )

set( ADMUTIL_SOURCES
		src/Platform.hpp
		src/Precompiled.hpp
		src/Containers/Chain.hpp
		src/Containers/Dictionary.hpp
		src/Containers/Dictionary.cpp
		src/Containers/NTree.hpp
		src/Containers/Singleton.hpp
		src/Maths/AABB.hpp
		src/Maths/Lerp.hpp
		src/Maths/Mat4.hpp
		src/Maths/Mat4.inl
		src/Maths/Mat4.cpp
		src/Maths/Vec2.hpp
		src/Maths/Vec2.cpp
		src/Maths/Vec3.hpp
		src/Maths/Vec3.cpp
		src/Maths/Vec4.hpp
		src/Maths/Vec4.cpp
		src/Maths/Plane.hpp
		src/Maths/Plane.cpp
		src/Maths/Polygon.hpp
		src/Maths/Polygon.cpp
		src/Text/Format.hpp
		src/Text/JSON.hpp
		src/Text/Lexer.hpp
		src/Text/Lexer.cpp
		src/Time/DateTime.hpp
		src/Time/DateTime.cpp
		src/Time/Timer.hpp
		src/System/Library.hpp
		src/System/Library.cpp )

## User of this library: this is what you're interested in
set ( ADMUTIL_INCLUDE_DIRECTORY
		${ADMUTIL_ROOT}/src
		CACHE INTERNAL "" )

## If someone's embedding this, let them group the sources 
## as they wish
if ( NOT ADMUTIL_NONLIB )
	source_group( TREE ${ADMUTIL_ROOT} FILES ${ADMUTIL_SOURCES} )
endif()

if ( NOT ADMUTIL_NONLIB )
	## AdmUtils.lib
	add_library( AdmUtils 
			${ADMUTIL_SOURCES} )
	
	## Lib output dir
	install( TARGETS AdmUtils
		ARCHIVE DESTINATION ${ADMUTIL_ROOT}/lib )

	## Include directories
	target_include_directories( AdmUtils PUBLIC
			${ADMUTIL_INCLUDE_DIRECTORY}
			extern/date/include
			extern/nlohmann-json/include )

	## On Linux, choose between X11 and Wayland
	if ( UNIX )
		if ( ADMUTIL_USE_WAYLAND )
			target_compile_definitions( AdmUtils PUBLIC ADM_USE_WAYLAND=1 )
			target_compile_definitions( AdmUtils PUBLIC ADM_USE_X11=0 )
		else()
			target_compile_definitions( AdmUtils PUBLIC ADM_USE_WAYLAND=0 )
			target_compile_definitions( AdmUtils PUBLIC ADM_USE_X11=1 )
		endif()
	else()
		target_compile_definitions( AdmUtils PUBLIC ADM_USE_WAYLAND=0 )
		target_compile_definitions( AdmUtils PUBLIC ADM_USE_X11=0 )
	endif()

	if ( ADMUTIL_USE_SSE41 )
		target_compile_definitions( AdmUtils PUBLIC ADM_USE_SSE41=1 )
	else()
		target_compile_definitions( AdmUtils PUBLIC ADM_USE_SSE41=0 )
	endif()

	if ( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC" )
		## Nothing, there's no special SSE4 flag for MSVC it seems
	else() ## GCC, Clang
		set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse4.1" )
	endif()

	## Precompiled headers
	target_precompile_headers( AdmUtils PRIVATE
			src/Precompiled.hpp )
else()
	include_directories( ${ADMUTIL_INCLUDE_DIRECTORY} )
endif()
