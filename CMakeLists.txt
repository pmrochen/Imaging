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
set(IMAGING_HAS_FILESYSTEM OFF)
#set(IMAGING_HAS_LIBPNG OFF)
set(IMAGING_HAS_LIBPNG "${IMAGING_ENABLE_LIBPNG}")

set(SIMD_TARGET "Simd")
set(SIMD_DIR "Simd")
#set(SIMD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Simd")
set(FILESYSTEM_TARGET "Core::Filesystem")
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
add_library(Core::${IMAGING_LIB_NAME} ALIAS ${IMAGING_LIB_NAME})
#set_target_properties(${IMAGING_LIB_NAME} PROPERTIES EXPORT_NAME Core::${IMAGING_LIB_NAME})

target_link_libraries(${IMAGING_LIB_NAME} INTERFACE ${SIMD_TARGET})

if(TARGET "${FILESYSTEM_TARGET}")
	target_link_libraries(${IMAGING_LIB_NAME} PUBLIC ${FILESYSTEM_TARGET})
	set(IMAGING_HAS_FILESYSTEM ON)
	message(STATUS "Found Core::Filesystem")
endif()

target_include_directories(${IMAGING_LIB_NAME}
	PUBLIC
		#$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Simd/Include>
		#$<INSTALL_INTERFACE:Simd/Include>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/half/include>
		$<INSTALL_INTERFACE:half/include>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/bitwise-enum>
		$<INSTALL_INTERFACE:bitwise-enum>
)

target_sources(${IMAGING_LIB_NAME}
	PRIVATE
		RectangleBinPack/MaxRectsBinPack.cpp
		#Source/Imaging/Image/Image.cpp
		#Source/Imaging/Image/ImageLayer.cpp
		#Source/Imaging/Image/ImageSequence.cpp
		#Source/Imaging/Image/ImageView.cpp
		#Source/Imaging/Image/LayeredImage.cpp
		#Source/Imaging/Image/SubImage.cpp
		#Source/Imaging/Image/TextureAtlas.cpp
		#Source/Imaging/Image/TextureImage.cpp
		Source/Imaging/Pixel/PixelFormatInfo.cpp

	PRIVATE
    	FILE_SET privateHeaders
	    TYPE HEADERS
		BASE_DIRS RectangleBinPack
    	FILES
			RectangleBinPack/MaxRectsBinPack.h
				
	PUBLIC
    	FILE_SET publicHeaders
	    TYPE HEADERS
		BASE_DIRS Source bitwise-enum half/include
    	FILES
			bitwise-enum/bitwise_enum.hpp
			half/include/half.hpp
			Source/Imaging/Color/Color3.hpp
			Source/Imaging/Color/Color4.hpp
			Source/Imaging/Color/ColorMask.hpp
			Source/Imaging/Color/ColorSpace.hpp
			Source/Imaging/Color/ErrorMetric.hpp
			Source/Imaging/Formats/ImageFormat.hpp
			Source/Imaging/Geometry/Point.hpp
			Source/Imaging/Geometry/Rectangle.hpp
			Source/Imaging/Geometry/Size.hpp
			Source/Imaging/Image/CubemapFace.hpp
			#Source/Imaging/Image/Image.hpp
			Source/Imaging/Image/ImageExtrapolationMode.hpp
			#Source/Imaging/Image/ImageLayer.hpp
			#Source/Imaging/Image/ImageSequence.hpp
			#Source/Imaging/Image/ImageView.hpp
			#Source/Imaging/Image/LayeredImage.hpp
			Source/Imaging/Image/MipmapFlags.hpp
			#Source/Imaging/Image/SubImage.hpp
			#Source/Imaging/Image/TextureAtlas.hpp
			#Source/Imaging/Image/TextureImage.hpp
			Source/Imaging/Image/TextureImageType.hpp
			Source/Imaging/Pixel/ChannelOrder.hpp
			Source/Imaging/Pixel/PixelFormat.hpp
			Source/Imaging/Pixel/PixelFormatInfo.hpp
			Source/Imaging/Pixel/Pixel.inl
			Source/Imaging/Sampling/ImageInterpolationMode.hpp
			Source/Imaging/Sampling/ResamplingFilter.hpp
			Source/Imaging/Tuples/Tuples.hpp
			Source/Imaging/Tuples/Tuple2.hpp
			Source/Imaging/Tuples/Tuple3.hpp
			Source/Imaging/Tuples/Tuple4.hpp
			Source/Imaging/Imaging.hpp
)

target_compile_features(${IMAGING_LIB_NAME} 
    PUBLIC 
        cxx_std_20
)

target_compile_definitions(${IMAGING_LIB_NAME}
	PUBLIC
		IMAGING_HAS_SIMD=$<BOOL:${IMAGING_HAS_SIMD}>
		IMAGING_HAS_FILESYSTEM=$<BOOL:${IMAGING_HAS_FILESYSTEM}>
		IMAGING_HAS_LIBPNG=$<BOOL:${IMAGING_HAS_LIBPNG}>
)
