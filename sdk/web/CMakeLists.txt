cmake_minimum_required(VERSION 3.20)
project(lcom_ng_web LANGUAGES C CXX)
if(NOT EMSCRIPTEN)
  message(FATAL_ERROR "Use emcmake cmake for the browser SDK")
endif()
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
get_filename_component(LCOM_SDK "${CMAKE_CURRENT_SOURCE_DIR}/.." ABSOLUTE)
if(EXISTS "${LCOM_SDK}/../runtime/core/Machine.cpp")
  set(LCOM_RUNTIME "${LCOM_SDK}/../runtime")
else()
  set(LCOM_RUNTIME "${LCOM_SDK}/runtime")
endif()
include("${LCOM_RUNTIME}/core/sources.cmake")
add_library(machinelab STATIC ${MACHINE_LAB_CORE_SOURCES}
  "${LCOM_RUNTIME}/embedded/Client.cpp" "${LCOM_RUNTIME}/web/BrowserClient.cpp" "${LCOM_SDK}/client/src/port_widths.c")
target_include_directories(machinelab PUBLIC "${LCOM_SDK}/include" "${LCOM_RUNTIME}")
target_compile_options(machinelab PRIVATE -O2 -Wall -Wextra)
add_library(lcom_minix STATIC
  "${LCOM_SDK}/compat/src/compat.c" "${LCOM_SDK}/compat/src/lcf.c"
  "${LCOM_SDK}/compat/src/printf.c"
  "${LCOM_SDK}/compat/src/xpm.c" output_log.c)
target_include_directories(lcom_minix PUBLIC "${LCOM_SDK}/include" "${LCOM_SDK}/compat/include")
target_compile_options(lcom_minix PRIVATE -O2 -fsigned-char)
