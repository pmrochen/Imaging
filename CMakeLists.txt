cmake_minimum_required(VERSION 3.25)

project(Imaging LANGUAGES CXX)
set(IMAGING_LIB_NAME Imaging)

find_package(OpenMP)
if(MSVC)
	set(OpenMP_RUNTIME_MSVC "llvm")
endif()

option(IMAGING_USE_ZLIB_NG "Use zlib-ng instead of zlib" ON)
option(IMAGING_HAS_LIBPNG "Support PNG format" ON)

if(NOT TARGET "Simd")
	add_subdirectory(Simd)
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
    	FILES
			RectangleBinPack/MaxRectsBinPack.h
			Tuples/Tuples.hpp
			Tuples/Tuple2.hpp
			Tuples/Tuple3.hpp
			Tuples/Tuple4.hpp
				
	PUBLIC
    	FILE_SET publicHeaders
	    TYPE HEADERS
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
