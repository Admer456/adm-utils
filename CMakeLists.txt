
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

## C++17 babyyyy
set(CMAKE_CXX_STANDARD 17) 

OPTION( ADMUTIL_NONLIB "Bundle the library into another project instead of compiling a .lib; will define ADMUTIL_ALL_SRC which you can then include in your project" OFF )

if ( NOT ADMUTIL_NONLIB )
    project( AdmUtils )
endif()

set( ADMUTIL_ROOT ${CMAKE_CURRENT_SOURCE_DIR} )

set( ADMUTIL_SOURCES
		src/Precompiled.hpp
        src/Containers/Dictionary.hpp
        src/Containers/Dictionary.cpp
        src/Containers/Singleton.hpp 
		src/Maths/Vec3.hpp 
		src/Maths/Vec3.cpp 
        src/Text/Lexer.hpp        
        src/Text/Lexer.cpp 
		src/Time/Timer.hpp
		src/Time/Timer.cpp )

## User of this library: this is what you're interested in
set ( ADMUTIL_INCLUDE_DIRECTORY
        ${ADMUTIL_ROOT}/src )

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
    		LIBRARY_OUTPUT_DIRECTORY ${ADMUTIL_ROOT}/libs )
    
    ## Include directories
    target_include_directories( AdmUtils PRIVATE
    		${ADMUTIL_INCLUDE_DIRECTORY} )

    ## Precompiled headers
    target_precompile_headers( AdmUtils PRIVATE 
            src/Precompiled.hpp )

    ## Output directory
    install( TARGETS AdmUtils
            RUNTIME DESTINATION ${ADMUTIL_ROOT}/libs
            LIBRARY DESTINATION ${ADMUTIL_ROOT}/libs 
            ARCHIVE DESTINATION ${ADMUTIL_ROOT}/libs )
else()
    include_directories( ${ADMUTIL_INCLUDE_DIRECTORY} )
endif()
