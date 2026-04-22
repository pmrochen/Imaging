# Imaging

cmake_minimum_required(VERSION 3.25)

if(NOT DEFINED CACHE{IMAGING_LIB_VERSION})
	string(TIMESTAMP VER_YEAR "%y")
	string(TIMESTAMP VER_MONTH "%m")
	math(EXPR VER_MONTH "${VER_MONTH} + 0")
    set(IMAGING_LIB_VERSION "${VER_YEAR}.${VER_MONTH}" CACHE STRING "Project version (auto-set at first configure)")
endif()

project(Imaging
	VERSION ${IMAGING_LIB_VERSION}
	LANGUAGES CXX
)

set(IMAGING_LIB_NAME Imaging)

find_package(OpenMP)
if(MSVC)
	set(OpenMP_RUNTIME_MSVC "llvm")
endif()

option(IMAGING_ENABLE_SIMD "Enable SIMD support" ON)
option(IMAGING_ENABLE_LIBPNG "Enable PNG format support" ON)
option(IMAGING_USE_ZLIB_NG "Use zlib-ng instead of zlib" ON)

#set(IMAGING_HAS_SIMD OFF)
set(IMAGING_HAS_SIMD "${IMAGING_ENABLE_SIMD}")
#set(IMAGING_HAS_LIBPNG OFF)
set(IMAGING_HAS_LIBPNG "${IMAGING_ENABLE_LIBPNG}")

set(SIMD_TARGET "Simd")
set(SIMD_DIR "Simd")
#set(SIMD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Simd")
set(LIBPNG_DIR "libpng")

#if(IMAGING_ENABLE_SIMD)
#    if(EXISTS "${SIMD_DIR}/CMakeLists.txt")
#        if(NOT TARGET "${SIMD_TARGET}")
#            add_subdirectory("${SIMD_DIR}")
#        endif()
#        set(IMAGING_HAS_SIMD ON)
#        message(STATUS "+++ SIMD support: ENABLED")
#    else()
#        message(STATUS "--- SIMD support: DISABLED (Source files not found)")
#    endif()
#else()
#    message(STATUS "--- SIMD support: DISABLED")
#endif()

if(IMAGING_HAS_SIMD)
	if(NOT TARGET "${SIMD_TARGET}")
		add_subdirectory("${SIMD_DIR}")
	endif()
endif()

if(IMAGING_HAS_LIBPNG)
	if(IMAGING_USE_ZLIB_NG)
		configure_file(zlib-ng/zconf.h.in zlib-ng/zconf.h COPYONLY)
		configure_file(zlib-ng/zlib_name_mangling.h.empty zlib_name_mangling.h COPYONLY)
		configure_file(zlib-ng/zlib.h.in zlib.h @ONLY)
	endif()
	configure_file(libpng/pnglibconf.h.prebuilt libpng/pnglibconf.h COPYONLY)
endif()

add_library(${IMAGING_LIB_NAME} STATIC)

target_include_directories(${IMAGING_LIB_NAME}
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/half/include>
		$<INSTALL_INTERFACE:half/include>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/bitwise-enum>
		$<INSTALL_INTERFACE:bitwise-enum>
)

target_sources(${IMAGING_LIB_NAME}
	PRIVATE
		PixelFormatInfo.cpp
		RectangleBinPack/MaxRectsBinPack.cpp
				
	PRIVATE
    	FILE_SET privateHeaders
	    TYPE HEADERS
		#BASE_DIRS Source
    	FILES
			RectangleBinPack/MaxRectsBinPack.h
			Tuples/Tuples.hpp
			Tuples/Tuple2.hpp
			Tuples/Tuple3.hpp
			Tuples/Tuple4.hpp
				
	PUBLIC
    	FILE_SET publicHeaders
	    TYPE HEADERS
		#BASE_DIRS Source
    	FILES
			half/include/half.hpp
			bitwise-enum/bitwise_enum.hpp
			ChannelOrder.hpp
			Color3.hpp
			Color4.hpp
			ColorMask.hpp
			ColorSpace.hpp
			ImageFormat.hpp
			Imaging.hpp
			Pixel.inl
			PixelFormat.hpp
			PixelFormatInfo.hpp
			Point.hpp
			Rectangle.hpp
			Size.hpp
)

target_compile_features(${IMAGING_LIB_NAME} 
    PUBLIC 
        cxx_std_20
)

target_compile_definitions(${IMAGING_LIB_NAME}
	PUBLIC
		IMAGING_HAS_SIMD=$<BOOL:${IMAGING_HAS_SIMD}>
		IMAGING_HAS_LIBPNG=$<BOOL:${IMAGING_HAS_LIBPNG}>
)
