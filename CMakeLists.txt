
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
		src/Containers/Singleton.hpp 
		src/Maths/Lerp.hpp 
		src/Maths/Vec3.hpp 
		src/Maths/Vec3.cpp 
		src/Maths/Plane.hpp 
		src/Maths/Plane.cpp 
		src/Maths/Polygon.hpp 
		src/Maths/Polygon.cpp 
		src/Text/Format.hpp 
		src/Text/Lexer.hpp 
		src/Text/Lexer.cpp 
		src/Time/Timer.hpp 
		src/Time/Timer.cpp )

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
	set_target_properties( AdmUtils PROPERTIES
			ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${ADMUTIL_ROOT}/libs 
			ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${ADMUTIL_ROOT}/libs 
			)
	
	## Include directories
	target_include_directories( AdmUtils PRIVATE
			${ADMUTIL_INCLUDE_DIRECTORY} )

	## Precompiled headers
	target_precompile_headers( AdmUtils PRIVATE 
			src/Precompiled.hpp )
else()
	include_directories( ${ADMUTIL_INCLUDE_DIRECTORY} )
endif()
