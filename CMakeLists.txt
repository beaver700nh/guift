cmake_minimum_required(VERSION 3.20)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(avr)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY artifacts)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY artifacts)

project(guift)

###
### Global configuration
###

set(COMPILE_OPTIONS
	-g
	-CC
	-mmcu=$ENV{AVR_MCU_NAME}
	-DF_CPU=$ENV{AVR_MCU_FREQ}
	-DARDUINO=20000
	-DARDUINO_AVR_MEGA2560
	-DARDUINO_ARCH_AVR
	-D__AVR__
	-D__AVR_ATmega2560__
	-Wall
	-Wextra
	-Wno-unused-variable
	-fno-exceptions
	-ffunction-sections
	-fdata-sections
	-Os
)

file(GLOB BUILTIN_LIBS CONFIGURE_DEPENDS "${AVR_HW_DIR}/libraries/*/src")

set(INCLUDE_DIRS
	${AVR_TC_DIR}/../lib/gcc/avr/*/include
	${AVR_TC_DIR}/../avr/include
	${AVR_HW_DIR}/cores/arduino
	${AVR_HW_DIR}/variants/mega
	${BUILTIN_LIBS}
)

set(LINK_OPTIONS
	-g
	-mmcu=$ENV{AVR_MCU_NAME}
	-fuse-linker-plugin
	-O1
	-Wl,-m$ENV{AVR_MCU_ARCH},--gc-sections
)

link_directories(
	${AVR_TC_DIR}/../lib/gcc/avr/7.3.0/$ENV{AVR_MCU_ARCH} ####
	${AVR_TC_DIR}/../avr/lib/$ENV{AVR_MCU_ARCH}
)

function(apply_common_target_settings target_name)
	target_include_directories(${target_name} PUBLIC ${INCLUDE_DIRS})
	target_compile_options(${target_name} PUBLIC ${COMPILE_OPTIONS})
endfunction()

###
### Library configuration
###

set(LIBRARY_TARGETS "")

function(compile_library lib_dir)
	get_filename_component(LIB_NAME "${lib_dir}" NAME)
	string(MAKE_C_IDENTIFIER "${LIB_NAME}" LIB_TARGET_NAME)

	file(
		GLOB_RECURSE LIB_SOURCE_FILES
		CONFIGURE_DEPENDS
		"${lib_dir}/*.S"
		"${lib_dir}/*.c"
		"${lib_dir}/*.cc"
		"${lib_dir}/*.cpp"
		"${lib_dir}/*.cxx"
	)

	add_library(${LIB_TARGET_NAME} STATIC ${LIB_SOURCE_FILES})
	apply_common_target_settings(${LIB_TARGET_NAME})
	target_include_directories(${LIB_TARGET_NAME} PUBLIC "${lib_dir}")
	list(APPEND LIBRARY_TARGETS ${LIB_TARGET_NAME})
endfunction()

compile_library("${AVR_HW_DIR}/cores/arduino")

compile_library("${AVR_HW_DIR}/libraries/SPI")
compile_library("${AVR_HW_DIR}/libraries/Wire")

compile_library("lib/Adafruit-GFX-Library")
target_include_directories(Adafruit_GFX_Library PUBLIC
	lib/Adafruit_BusIO
)

###
### Executable configuration
###

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "src/*.cpp")
set(TARGET_NAME ${PROJECT_NAME}.elf)

add_executable(${TARGET_NAME} ${SOURCE_FILES})
apply_common_target_settings(${TARGET_NAME})

target_link_options(${TARGET_NAME} PUBLIC ${LINK_OPTIONS})
target_link_libraries(${TARGET_NAME} PUBLIC ${LIBRARY_TARGETS})

add_custom_command(
	TARGET ${TARGET_NAME}
	POST_BUILD
	COMMAND ${CMAKE_OBJCOPY}
	ARGS -O ihex -R .eeprom ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TARGET_NAME}
		${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TARGET_NAME}.hex
	COMMAND ${CMAKE_SIZE}
	ARGS -A ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TARGET_NAME}
)
