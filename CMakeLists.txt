## Minimum is 3.16 for PCH support
cmake_minimum_required( VERSION 3.16 )

## C++17 babyyyy
set(CMAKE_CXX_STANDARD 17) 

OPTION( ADMUTIL_NONLIB "Bundle the library into another project instead of compiling a .lib; will define ADMUTIL_ALL_SRC which you can then include in your project" OFF )

if ( NOT ADMUTIL_NONLIB )
    project( AdmUtils )
endif()

set( ADMUTIL_ROOT ${CMAKE_CURRENT_SOURCE_DIR} )

set( ADMUTIL_SRC
		src/Precompiled.hpp )

set ( ADMUTIL_CONTAINERS_SRC 
        src/Containers/Dictionary.hpp
        src/Containers/Dictionary.cpp
        src/Containers/Singleton.hpp )

set( ADMUTIL_MATHS_SRC 
		src/Maths/Vec3.hpp 
		src/Maths/Vec3.cpp )

set( ADMUTIL_TEXT_SRC 
        src/Text/Lexer.hpp        
        src/Text/Lexer.cpp )

set( ADMUTIL_TIME_SRC 
		src/Time/Timer.hpp
		src/Time/Timer.cpp )

source_group( "Source\\" FILES ${ADMUTIL_SRC} )
source_group( "Source\\Containers\\" FILES ${ADMUTIL_CONTAINERS_SRC} )
source_group( "Source\\Maths\\" FILES ${ADMUTIL_MATHS_SRC} )
source_group( "Source\\Text\\" FILES ${ADMUTIL_TEXT_SRC} )
source_group( "Source\\Time\\" FILES ${ADMUTIL_TIME_SRC} )

set ( ADMUTIL_ALL_SRC 
        ${ADMUTIL_SRC}
        ${ADMUTIL_CONTAINERS_SRC}
        ${ADMUTIL_MATHS_SRC}
        ${ADMUTIL_TEXT_SRC}
        ${ADMUTIL_TIME_SRC} )

if ( NOT ADMUTIL_NONLIB )
    ## AdmUtils.lib
    add_library( AdmUtils 
            ${ADMUTIL_ALL_SRC} )
    
    ## Lib output dir
    set_target_properties( AdmUtils PROPERTIES
    		LIBRARY_OUTPUT_DIRECTORY ${ADMUTIL_ROOT}/libs )
    
    ## Include directories
    target_include_directories( AdmUtils PRIVATE
    		${ADMUTIL_ROOT}/src )

    ## Precompiled headers
    target_precompile_headers( AdmUtils PRIVATE 
            src/Precompiled.hpp )
else()
    include_directories( ${ADMUTIL_ROOT}/src )
endif()
