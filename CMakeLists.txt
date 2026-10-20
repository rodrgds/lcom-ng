cmake_minimum_required(VERSION 3.20)

project(lcom-ng VERSION 0.3.0 LANGUAGES C CXX)

file(READ ${CMAKE_CURRENT_SOURCE_DIR}/package.json MACHINE_LAB_PACKAGE_JSON)
string(JSON MACHINE_LAB_PACKAGE_VERSION GET "${MACHINE_LAB_PACKAGE_JSON}" version)
if(NOT MACHINE_LAB_PACKAGE_VERSION VERSION_EQUAL PROJECT_VERSION)
  message(FATAL_ERROR
    "package.json version ${MACHINE_LAB_PACKAGE_VERSION} does not match PROJECT_VERSION ${PROJECT_VERSION}")
endif()

option(MACHINE_LAB_WITH_SDL "Build the SDL3 desktop display/input backend" OFF)
if(DEFINED LCOM_WITH_SDL)
  set(MACHINE_LAB_WITH_SDL "${LCOM_WITH_SDL}" CACHE BOOL
    "Build the SDL3 desktop display/input backend" FORCE)
endif()

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(CTest)
find_package(CLI11 CONFIG QUIET)
if(NOT CLI11_FOUND)
  include(FetchContent)
  FetchContent_Declare(
    CLI11
    URL https://github.com/CLIUtils/CLI11/archive/refs/tags/v2.5.0.tar.gz
  )
  FetchContent_MakeAvailable(CLI11)
endif()

include(runtime/core/sources.cmake)
add_library(machinelab_core STATIC ${MACHINE_LAB_CORE_SOURCES})
target_include_directories(machinelab_core PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/sdk/include
  ${CMAKE_CURRENT_SOURCE_DIR}/common
  ${CMAKE_CURRENT_SOURCE_DIR}/runtime
)
target_compile_options(machinelab_core PRIVATE -Wall -Wextra -Wpedantic)

# App hosts embed the device core instead of launching a separate student process.
add_library(lcom_embedded STATIC runtime/embedded/Client.cpp sdk/client/src/port_widths.c)
target_link_libraries(lcom_embedded PUBLIC machinelab_core)
target_compile_options(lcom_embedded PRIVATE -Wall -Wextra -Wpedantic)

add_library(machinelab_c STATIC sdk/client/src/lcom.c sdk/client/src/port_widths.c)
set_target_properties(machinelab_c PROPERTIES OUTPUT_NAME machinelab)
target_include_directories(machinelab_c PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/sdk/include
  ${CMAKE_CURRENT_SOURCE_DIR}/common
)
target_compile_options(machinelab_c PRIVATE -Wall -Wextra -Wpedantic)
find_package(Threads REQUIRED)
add_library(lcom_minix STATIC
  sdk/compat/src/compat.c
  sdk/compat/src/output_log.c
  sdk/compat/src/lcf.c
  sdk/compat/src/printf.c
  sdk/compat/src/xpm.c
)
target_include_directories(lcom_minix PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/sdk/compat/include
  ${CMAKE_CURRENT_SOURCE_DIR}/sdk/include
)
target_link_libraries(lcom_minix PUBLIC machinelab_c Threads::Threads)
target_compile_options(lcom_minix PRIVATE -fsigned-char -Wall -Wextra -Wpedantic)
add_custom_command(TARGET machinelab_c POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:machinelab_c>
    $<TARGET_FILE_DIR:machinelab_c>/${CMAKE_STATIC_LIBRARY_PREFIX}lowlab${CMAKE_STATIC_LIBRARY_SUFFIX}
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:machinelab_c>
    $<TARGET_FILE_DIR:machinelab_c>/${CMAKE_STATIC_LIBRARY_PREFIX}lcom${CMAKE_STATIC_LIBRARY_SUFFIX}
)

set(MACHINE_LAB_RUNTIME_SOURCES
  runtime/main.cpp
  runtime/cli/FileOps.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/generated/LabCatalog.cpp
  runtime/core/RuntimeServer.cpp
  runtime/core/PairRuntimeServer.cpp
)

set(LAB_CATALOG_JSON ${CMAKE_CURRENT_SOURCE_DIR}/course/labs/function-requests.json)
set(LAB_CATALOG_GENERATOR ${CMAKE_CURRENT_SOURCE_DIR}/scripts/generate_lab_catalog.cmake)
set(GENERATED_LAB_CATALOG ${CMAKE_CURRENT_BINARY_DIR}/generated/LabCatalog.cpp)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/generated)
execute_process(
  COMMAND ${CMAKE_COMMAND}
    -DINPUT=${LAB_CATALOG_JSON}
    -DOUTPUT=${GENERATED_LAB_CATALOG}
    -P ${LAB_CATALOG_GENERATOR}
  RESULT_VARIABLE LAB_CATALOG_RESULT
)
if(NOT LAB_CATALOG_RESULT EQUAL 0)
  message(FATAL_ERROR "Could not generate the lab catalog")
endif()
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS
  ${LAB_CATALOG_JSON}
  ${LAB_CATALOG_GENERATOR}
)

if(MACHINE_LAB_WITH_SDL)
  find_package(PkgConfig REQUIRED)
  pkg_check_modules(SDL3 REQUIRED IMPORTED_TARGET sdl3)
  pkg_check_modules(SDL3_TTF REQUIRED IMPORTED_TARGET sdl3-ttf)
  list(APPEND MACHINE_LAB_RUNTIME_SOURCES runtime/backends/SdlBackend.cpp)
  list(APPEND MACHINE_LAB_RUNTIME_SOURCES runtime/backends/SdlAudioBackend.cpp)
endif()

add_executable(machinelab ${MACHINE_LAB_RUNTIME_SOURCES})
add_dependencies(machinelab lcom_minix)
target_link_libraries(machinelab PRIVATE machinelab_core)
target_compile_options(machinelab PRIVATE -Wall -Wextra -Wpedantic)
target_link_libraries(machinelab PRIVATE CLI11::CLI11)
if(MACHINE_LAB_WITH_SDL)
  target_compile_definitions(machinelab PRIVATE
    MACHINE_LAB_WITH_SDL
    MACHINE_LAB_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}"
    MACHINE_LAB_BINARY_DIR="${CMAKE_BINARY_DIR}"
  )
  target_link_libraries(machinelab PRIVATE PkgConfig::SDL3 PkgConfig::SDL3_TTF)
else()
  target_compile_definitions(machinelab PRIVATE
    MACHINE_LAB_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}"
    MACHINE_LAB_BINARY_DIR="${CMAKE_BINARY_DIR}"
  )
endif()

add_custom_command(TARGET machinelab POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:machinelab>
    $<TARGET_FILE_DIR:machinelab>/lcom-ng${CMAKE_EXECUTABLE_SUFFIX}
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:machinelab>
    $<TARGET_FILE_DIR:machinelab>/lowlab${CMAKE_EXECUTABLE_SUFFIX}
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:machinelab>
    $<TARGET_FILE_DIR:machinelab>/lcom${CMAKE_EXECUTABLE_SUFFIX}
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:machinelab>
    $<TARGET_FILE_DIR:machinelab>/lcom-run${CMAKE_EXECUTABLE_SUFFIX}
)

set(MACHINE_LAB_EXAMPLES
  ambient_aquarium
  hello
  audio_tone
  breakout_demo
  interrupt_scope
  timer_int
  keyboard_scan
  music_maker
  mouse_packet
  pixel_studio
  pomodoro_clock
  rtc_date
  sdl_demo
  serial_transfer
  serial_whiteboard
  tilemap_editor
  uart_loopback
  uart_pair
  uart_peer_sender
  uart_peer_receiver
  vbe_rectangle
  word_processor
)

foreach(example IN LISTS MACHINE_LAB_EXAMPLES)
  add_executable(${example} examples/${example}.c)
  target_link_libraries(${example} PRIVATE machinelab_c)
  target_compile_options(${example} PRIVATE -Wall -Wextra -Wpedantic)
  set_target_properties(${example} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/examples
  )
endforeach()

add_executable(flappy_bird
  examples/flappy/main.c
  examples/flappy/app.c
  examples/flappy/menu.c
  examples/flappy/font.c
  examples/flappy/renderer.c
  examples/flappy/sound.c
)
target_link_libraries(flappy_bird PRIVATE machinelab_c)
target_compile_options(flappy_bird PRIVATE -Wall -Wextra -Wpedantic)
set_target_properties(flappy_bird PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/examples
)

file(GLOB NINJIX_SOURCES CONFIGURE_DEPENDS
  examples/ninjix/*.c
)
add_executable(ninjix ${NINJIX_SOURCES})
target_include_directories(ninjix PRIVATE examples/ninjix)
target_link_libraries(ninjix PRIVATE machinelab_c)
target_compile_options(ninjix PRIVATE -Wall -Wextra -Wpedantic)
set_target_properties(ninjix PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/examples
)

file(GLOB_RECURSE PVZ_SOURCES CONFIGURE_DEPENDS
  examples/pvz/src/*.c
)
add_executable(pvz ${PVZ_SOURCES})
target_include_directories(pvz PRIVATE examples/pvz/include)
target_link_libraries(pvz PRIVATE machinelab_c)
target_compile_options(pvz PRIVATE -Wall -Wextra -Wpedantic -Wno-strict-prototypes)
set_target_properties(pvz PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/examples
)

set(STUDENT_EXAMPLE_PATHS "")
foreach(example IN LISTS MACHINE_LAB_EXAMPLES ITEMS flappy_bird ninjix pvz)
  string(APPEND STUDENT_EXAMPLE_PATHS "$<TARGET_FILE:${example}>\n")
endforeach()
file(GENERATE OUTPUT "${CMAKE_BINARY_DIR}/student-examples.txt"
  CONTENT "${STUDENT_EXAMPLE_PATHS}")

add_executable(unit_tests
  tests/unit_tests.cpp
)
target_link_libraries(unit_tests PRIVATE machinelab_core)
target_compile_options(unit_tests PRIVATE -Wall -Wextra -Wpedantic)
set_target_properties(unit_tests PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests
)

# Exercise application input boundaries without a second copy of their logic.
set(NINJIX_TEST_SOURCES ${NINJIX_SOURCES})
list(FILTER NINJIX_TEST_SOURCES EXCLUDE REGEX "/proj\\.c$")
add_executable(ninjix_menu_tests tests/ninjix_menu_tests.c ${NINJIX_TEST_SOURCES})
target_include_directories(ninjix_menu_tests PRIVATE examples/ninjix)
add_executable(flappy_input_tests tests/flappy_input_tests.c
  examples/flappy/app.c examples/flappy/menu.c examples/flappy/font.c examples/flappy/renderer.c)
target_include_directories(flappy_input_tests PRIVATE examples/flappy)
foreach(test IN ITEMS ninjix_menu_tests flappy_input_tests)
  target_link_libraries(${test} PRIVATE machinelab_c)
  set_target_properties(${test} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests)
endforeach()

add_executable(clipboard_client tests/clipboard_client.c)
target_link_libraries(clipboard_client PRIVATE machinelab_c)
set_target_properties(clipboard_client PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests)

add_executable(client_protocol_tests tests/client_protocol_tests.c)
target_link_libraries(client_protocol_tests PRIVATE machinelab_c)
target_compile_options(client_protocol_tests PRIVATE -Wall -Wextra -Wpedantic)
set_target_properties(client_protocol_tests PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests
)

add_executable(framebuffer_dirty_client tests/framebuffer_dirty_client.c)
target_link_libraries(framebuffer_dirty_client PRIVATE machinelab_c)
target_compile_options(framebuffer_dirty_client PRIVATE -Wall -Wextra -Wpedantic)
set_target_properties(framebuffer_dirty_client PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests
)

function(add_lab_solution_test lab)
  add_executable(${lab}_solution_test
    dev/lab-tests/${lab}_test.c
    ${ARGN}
  )
  target_include_directories(${lab}_solution_test PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/dev/lab-tests
    ${CMAKE_CURRENT_SOURCE_DIR}/dev/lab-solutions/${lab}/include
  )
  target_link_libraries(${lab}_solution_test PRIVATE machinelab_c)
  target_compile_options(${lab}_solution_test PRIVATE -Wall -Wextra -Wpedantic)
  set_target_properties(${lab}_solution_test PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests
  )
endfunction()

add_lab_solution_test(lab1
  dev/lab-solutions/lab1/src/bitwise.c
  dev/lab-solutions/lab1/src/rtc_lib.c
)
add_lab_solution_test(lab2
  dev/lab-solutions/lab2/src/timer_lib.c
)
add_lab_solution_test(lab3
  dev/lab-solutions/lab3/src/keyboard_lib.c
)
add_lab_solution_test(lab4
  dev/lab-solutions/lab4/src/mouse_lib.c
)
add_lab_solution_test(lab5
  dev/lab-solutions/lab5/src/video_lib.c
)
add_lab_solution_test(lab6
  dev/lab-solutions/lab6/src/audio_lib.c
)
add_lab_solution_test(lab7
  dev/lab-solutions/lab7/src/uart_lib.c
)

if(BUILD_TESTING)
  find_package(Python3 COMPONENTS Interpreter REQUIRED)
  add_test(NAME hardware_config
    COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tests/hardware_config.py
      --build ${CMAKE_BINARY_DIR})
  set_tests_properties(hardware_config PROPERTIES TIMEOUT 45)
  add_test(NAME native_arguments
    COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tests/native_arguments.py
      --sdk ${CMAKE_CURRENT_SOURCE_DIR}/sdk)
  add_test(NAME android_apk_finalization
    COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tests/android_apk_finalization.py)
  add_test(NAME package_example_inventory
    COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tests/package_example_inventory.py)
  add_test(NAME community_source_regressions
    COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tests/community_source_regressions.py)
  add_test(NAME community_a141_regressions
    COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tests/community_a141_regressions.py)
  add_test(NAME community_a141_followup_regressions
    COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tests/community_a141_followup_regressions.py)
  add_test(NAME community_a143_regressions
    COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tests/community_a143_regressions.py)
  add_test(NAME community_expansion_regressions
    COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tests/community_expansion_regressions.py)
  if(MACHINE_LAB_WITH_SDL)
    add_executable(sdl_render_tests tests/sdl_render_tests.cpp runtime/backends/SdlBackend.cpp)
    target_compile_definitions(sdl_render_tests PRIVATE MACHINE_LAB_WITH_SDL)
    target_compile_options(sdl_render_tests PRIVATE -Wall -Wextra -Wpedantic)
    target_link_libraries(sdl_render_tests PRIVATE machinelab_core PkgConfig::SDL3 PkgConfig::SDL3_TTF)
    add_executable(embedded_sdl_tests tests/embedded_sdl_tests.cpp
      tests/embedded_sdl_host.cpp runtime/backends/SdlAudioBackend.cpp
      tests/fixtures/compat-sleep/main.c sdk/compat/src/compat.c sdk/compat/src/output_log.c)
    target_include_directories(embedded_sdl_tests PRIVATE "$<$<COMPILE_LANGUAGE:C>:${CMAKE_CURRENT_SOURCE_DIR}/sdk/compat/include>")
    target_compile_definitions(embedded_sdl_tests PRIVATE MACHINE_LAB_WITH_SDL LCOM_COMPAT_SLEEP_NATIVE_FIXTURE)
    target_link_libraries(embedded_sdl_tests PRIVATE lcom_embedded PkgConfig::SDL3 Threads::Threads)
    add_test(NAME embedded_sdl COMMAND embedded_sdl_tests)
    set_tests_properties(embedded_sdl PROPERTIES TIMEOUT 20
      ENVIRONMENT "SDL_VIDEODRIVER=dummy;SDL_AUDIODRIVER=dummy;SDL_RENDER_DRIVER=software"
      WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
    set_target_properties(sdl_render_tests PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests)
    add_test(NAME sdl_render COMMAND sdl_render_tests)
    set_tests_properties(sdl_render PROPERTIES
      TIMEOUT 30 ENVIRONMENT "SDL_VIDEODRIVER=dummy;SDL_RENDER_DRIVER=software")
  endif()
  add_executable(vbe_palette_tests tests/vbe_palette_tests.cpp runtime/devices/Vbe.cpp)
  target_include_directories(vbe_palette_tests PRIVATE sdk/include)
  add_test(NAME vbe_palette COMMAND vbe_palette_tests)

  add_executable(machine_config_tests tests/machine_config_tests.cpp)
  target_link_libraries(machine_config_tests PRIVATE machinelab_core)
  add_test(NAME machine_config COMMAND machine_config_tests)

  add_executable(compat_printf_tests tests/compat_printf.c sdk/compat/src/printf.c)
  target_include_directories(compat_printf_tests PRIVATE sdk/compat/include)
  target_compile_options(compat_printf_tests PRIVATE -Wall -Wextra -Wpedantic)
  add_test(NAME compat_printf COMMAND compat_printf_tests)

  add_executable(compat_bitmap_tests tests/compat_bitmap.c)
  target_include_directories(compat_bitmap_tests PRIVATE sdk/compat/include sdk/include)
  add_test(NAME compat_bitmap COMMAND compat_bitmap_tests)

  add_executable(compat_devio_tests tests/compat_devio.c)
  target_include_directories(compat_devio_tests PRIVATE sdk/compat/include sdk/include)
  add_test(NAME compat_devio COMMAND compat_devio_tests)

  add_executable(compat_xpm_tests tests/compat_xpm_tests.c)
  target_link_libraries(compat_xpm_tests PRIVATE lcom_minix)
  add_test(NAME compat_xpm COMMAND compat_xpm_tests)
  add_executable(compat_lcf tests/compat_lcf.c)
  target_link_libraries(compat_lcf PRIVATE lcom_minix)
  add_test(NAME compat_lcf
    COMMAND $<TARGET_FILE:machinelab> run --headless --max-ticks 600 -- $<TARGET_FILE:compat_lcf>)
  set_tests_properties(compat_lcf PROPERTIES TIMEOUT 15)
  foreach(timer_variant IN ITEMS fallback override)
    add_executable(compat_lcf_timer_${timer_variant} tests/compat_lcf_timer.c)
    target_link_libraries(compat_lcf_timer_${timer_variant} PRIVATE lcom_minix)
    if(timer_variant STREQUAL "override")
      target_compile_definitions(compat_lcf_timer_${timer_variant} PRIVATE LCOM_TEST_STRONG_TIMER)
    endif()
    add_test(NAME compat_lcf_timer_${timer_variant}
      COMMAND $<TARGET_FILE:machinelab> run --headless --max-ticks 60 -- $<TARGET_FILE:compat_lcf_timer_${timer_variant}>)
    set_tests_properties(compat_lcf_timer_${timer_variant} PROPERTIES TIMEOUT 15)
  endforeach()
  add_executable(compat_ps2_faults tests/compat_ps2_faults.c)
  target_link_libraries(compat_ps2_faults PRIVATE lcom_minix)
  add_test(NAME compat_ps2_faults
    COMMAND $<TARGET_FILE:machinelab> run --headless --max-ticks 30
      --script ${CMAKE_CURRENT_SOURCE_DIR}/tests/compat_ps2_faults.mlabscript
      -- $<TARGET_FILE:compat_ps2_faults>)
  add_test(NAME compat_ps2_faults_pair
    COMMAND $<TARGET_FILE:machinelab> run-pair --headless --max-ticks 30
      --left-script ${CMAKE_CURRENT_SOURCE_DIR}/tests/compat_ps2_faults.mlabscript
      --right-script ${CMAKE_CURRENT_SOURCE_DIR}/tests/compat_ps2_faults.mlabscript
      $<TARGET_FILE:compat_ps2_faults> --right $<TARGET_FILE:compat_ps2_faults>)
  set_tests_properties(compat_ps2_faults compat_ps2_faults_pair PROPERTIES TIMEOUT 15)
  add_executable(compat_output_log tests/compat_output_log.c)
  target_link_libraries(compat_output_log PRIVATE lcom_minix)
  add_test(NAME compat_output_log
    COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tests/compat_output_log.py
      $<TARGET_FILE:machinelab> $<TARGET_FILE:compat_output_log>)
  set_tests_properties(compat_output_log PROPERTIES TIMEOUT 30)
  # Feed deterministic runtime events through the real Minix notification owner.
  # Unused device adapters are discarded, so this fixture needs no runtime server.
  add_executable(compat_notifications tests/compat_notifications.c sdk/compat/src/compat.c)
  target_include_directories(compat_notifications PRIVATE sdk/compat/include sdk/include)
  target_compile_options(compat_notifications PRIVATE -ffunction-sections -fdata-sections)
  if(APPLE)
    target_link_options(compat_notifications PRIVATE -Wl,-dead_strip)
  else()
    target_link_options(compat_notifications PRIVATE -Wl,--gc-sections)
  endif()
  add_test(NAME compat_notifications COMMAND compat_notifications)
  add_executable(compat_asm_io tests/compat_asm_io.c sdk/compat/src/compat.c)
  target_include_directories(compat_asm_io PRIVATE sdk/compat/include sdk/include)
  target_compile_options(compat_asm_io PRIVATE -ffunction-sections -fdata-sections)
  if(APPLE)
    target_link_options(compat_asm_io PRIVATE -Wl,-dead_strip)
  else()
    target_link_options(compat_asm_io PRIVATE -Wl,--gc-sections)
  endif()
  add_test(NAME compat_asm_io
    COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tests/compat_asm_io.py $<TARGET_FILE:compat_asm_io>)
  add_test(NAME compat_byte_ports
    COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tests/compat_byte_ports.py
      $<TARGET_FILE:machinelab> ${CMAKE_CURRENT_SOURCE_DIR}/sdk ${CMAKE_BINARY_DIR}
      ${CMAKE_BINARY_DIR}/test-output/compat-byte-ports)
  set_tests_properties(compat_byte_ports PROPERTIES TIMEOUT 30)
  add_test(NAME compat_interrupts
    COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tests/compat_interrupts.py
      $<TARGET_FILE:machinelab> ${CMAKE_CURRENT_SOURCE_DIR}/sdk ${CMAKE_BINARY_DIR}
      ${CMAKE_BINARY_DIR}/test-output/compat-interrupts)
  set_tests_properties(compat_interrupts PROPERTIES TIMEOUT 30)
  add_executable(compat_paths tests/compat_paths.c)
  target_link_libraries(compat_paths PRIVATE lcom_minix)
  add_test(NAME compat_paths COMMAND $<TARGET_FILE:compat_paths>)
  # MSYS defaults to copying symlink targets, which cannot represent the
  # dangling-link save-path fixture. Its sys mode needs no Windows privilege.
  set_tests_properties(compat_paths PROPERTIES
    ENVIRONMENT "MSYS=winsymlinks:sys;CYGWIN=winsymlinks:sys")
  add_executable(compat_vbe_scanline tests/compat_vbe_scanline.c)
  target_link_libraries(compat_vbe_scanline PRIVATE lcom_minix)
  add_test(NAME compat_vbe_scanline
    COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tests/compat_vbe_scanline.py
      $<TARGET_FILE:machinelab> $<TARGET_FILE:compat_vbe_scanline> ${CMAKE_BINARY_DIR}/test-output/scanline)
  set_tests_properties(compat_vbe_scanline PROPERTIES TIMEOUT 30)
  add_executable(compat_vbe_dac tests/fixtures/dac-palette/main.c)
  target_link_libraries(compat_vbe_dac PRIVATE lcom_minix)
  add_test(NAME compat_vbe_dac
    COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tests/compat_vbe_dac.py
      $<TARGET_FILE:machinelab> $<TARGET_FILE:compat_vbe_dac> ${CMAKE_BINARY_DIR}/test-output/dac)
  set_tests_properties(compat_vbe_dac PROPERTIES TIMEOUT 30)
  add_executable(compat_client tests/compat_client.c)
  target_link_libraries(compat_client PRIVATE lcom_minix)
  add_test(NAME compat_client
    COMMAND $<TARGET_FILE:machinelab> run --headless --max-ticks 100
      --dump-frame ${CMAKE_BINARY_DIR}/test-output/compat-client.ppm -- $<TARGET_FILE:compat_client>)
  add_test(NAME compat_irq_policy
    COMMAND $<TARGET_FILE:machinelab> run --headless --max-ticks 100
      -- $<TARGET_FILE:compat_client> --irq-policy)
  add_test(NAME compat_client_pair
    COMMAND $<TARGET_FILE:machinelab> run-pair --headless --max-ticks 100
      --left-dump-frame ${CMAKE_BINARY_DIR}/test-output/compat-left.ppm
      --right-dump-frame ${CMAKE_BINARY_DIR}/test-output/compat-right.ppm
      $<TARGET_FILE:compat_client> --right $<TARGET_FILE:compat_client>)
  add_test(NAME compat_process
    COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tests/compat_process.py
      --sdk ${CMAKE_CURRENT_SOURCE_DIR}/sdk
      --runtime-build $<TARGET_FILE_DIR:lcom_minix>
      --runtime $<TARGET_FILE:machinelab>)
  add_test(NAME compat_legacy
    COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tests/compat_legacy.py
      --sdk ${CMAKE_CURRENT_SOURCE_DIR}/sdk --runtime-build ${CMAKE_BINARY_DIR}
      --runtime $<TARGET_FILE:machinelab>)
  add_test(NAME compat_import
    COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tests/compat_import_tests.py)
  foreach(mouse_boot IN ITEMS 0 1)
    add_test(NAME compat_mouse_boot_${mouse_boot}
      COMMAND $<TARGET_FILE:machinelab> run --headless --max-ticks 30
        --script ${CMAKE_CURRENT_SOURCE_DIR}/tests/compat_mouse.mlabscript
        -- $<TARGET_FILE:compat_client> --mouse-bootstrap)
    set_tests_properties(compat_mouse_boot_${mouse_boot} PROPERTIES
      TIMEOUT 15 ENVIRONMENT "LCOM_COMPAT_MOUSE_REPORTING=${mouse_boot}")
  endforeach()
  add_test(NAME compat_console
    COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tests/compat_console.py $<TARGET_FILE:machinelab>)
  set_tests_properties(compat_console PROPERTIES TIMEOUT 30)
  add_test(NAME runtime_child_errors
    COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tests/runtime_child_errors.py $<TARGET_FILE:machinelab>)
  set_tests_properties(runtime_child_errors PROPERTIES TIMEOUT 40)
  add_test(NAME native_uart_handshake
    COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tests/native_uart_handshake.py
      $<TARGET_FILE:machinelab> $<TARGET_FILE:uart_peer_sender> $<TARGET_FILE:uart_peer_receiver>)
  set_tests_properties(native_uart_handshake PROPERTIES TIMEOUT 30)
  add_test(NAME runtime_timeout
    COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tests/runtime_timeout.py $<TARGET_FILE:machinelab>)
  set_tests_properties(runtime_timeout PROPERTIES TIMEOUT 40)
  add_test(NAME compat_installed_cli
    COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tests/compat_installed_cli.py
      $<TARGET_FILE:machinelab> ${CMAKE_CURRENT_SOURCE_DIR}/sdk $<TARGET_FILE_DIR:lcom_minix>)
  set_tests_properties(compat_installed_cli PROPERTIES TIMEOUT 45)
  set_tests_properties(compat_client compat_client_pair PROPERTIES
    TIMEOUT 15 FIXTURES_SETUP compat_frames)
  add_test(NAME compat_frame_pixels
    COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tests/compat_frame_pixels.py
      ${CMAKE_BINARY_DIR}/test-output/compat-client.ppm
      ${CMAKE_BINARY_DIR}/test-output/compat-left.ppm
      ${CMAKE_BINARY_DIR}/test-output/compat-right.ppm)
  set_tests_properties(compat_frame_pixels PROPERTIES FIXTURES_REQUIRED compat_frames)
  file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/test-output)

  add_test(NAME ninjix_menu COMMAND ninjix_menu_tests)
  add_test(NAME flappy_input COMMAND flappy_input_tests)
  add_test(NAME presented_capture
    COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tests/presented_capture.py
      $<TARGET_FILE:machinelab> $<TARGET_FILE:framebuffer_dirty_client> $<TARGET_FILE:hello>)
  add_test(NAME studio_shortcuts
    COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tests/studio_shortcuts.py
            $<TARGET_FILE:machinelab> ${CMAKE_BINARY_DIR}/examples)
  set_tests_properties(studio_shortcuts PROPERTIES TIMEOUT 30)
  add_executable(embedded_tests tests/embedded_tests.cpp)
  target_link_libraries(embedded_tests PRIVATE lcom_embedded)
  add_test(NAME embedded_sdk COMMAND embedded_tests)
  add_executable(embedded_mouse_rate_tests tests/embedded_mouse_rate_tests.cpp)
  target_link_libraries(embedded_mouse_rate_tests PRIVATE lcom_embedded)
  add_test(NAME embedded_mouse_rate COMMAND embedded_mouse_rate_tests all)
  add_test(NAME unit COMMAND unit_tests)
  add_test(NAME client_protocol COMMAND client_protocol_tests)
  add_test(NAME host_clipboard COMMAND $<TARGET_FILE:machinelab> run --headless -- $<TARGET_FILE:clipboard_client>)
  add_test(NAME host_clipboard_pair COMMAND $<TARGET_FILE:machinelab> run-pair --headless
    $<TARGET_FILE:clipboard_client> --right $<TARGET_FILE:clipboard_client>)
  add_test(NAME lab1_solution
    COMMAND $<TARGET_FILE:machinelab> run --headless --rtc 2026-06-16T12:34:56 -- $<TARGET_FILE:lab1_solution_test>
  )
  add_test(NAME lab2_solution
    COMMAND $<TARGET_FILE:machinelab> run --headless -- $<TARGET_FILE:lab2_solution_test>
  )
  add_test(NAME lab3_solution
    COMMAND $<TARGET_FILE:machinelab> run --headless --script ${CMAKE_CURRENT_SOURCE_DIR}/scripts/type_a_esc.mlabscript -- $<TARGET_FILE:lab3_solution_test>
  )
  add_test(NAME lab4_solution
    COMMAND $<TARGET_FILE:machinelab> run --headless --script ${CMAKE_CURRENT_SOURCE_DIR}/scripts/mouse_move.mlabscript -- $<TARGET_FILE:lab4_solution_test>
  )
  add_test(NAME lab5_solution
    COMMAND $<TARGET_FILE:machinelab> run --headless --dump-frame ${CMAKE_BINARY_DIR}/test-output/lab5-solution.ppm -- $<TARGET_FILE:lab5_solution_test>
  )
  add_test(NAME lab6_solution
    COMMAND $<TARGET_FILE:machinelab> run --headless --audio-wav ${CMAKE_BINARY_DIR}/test-output/lab6-solution.wav -- $<TARGET_FILE:lab6_solution_test>
  )
  add_test(NAME lab7_solution
    COMMAND $<TARGET_FILE:machinelab> run --headless -- $<TARGET_FILE:lab7_solution_test>
  )
  add_test(NAME integration
    COMMAND sh ${CMAKE_CURRENT_SOURCE_DIR}/tests/integration.sh
      $<TARGET_FILE_DIR:machinelab>
      ${CMAKE_BINARY_DIR}/examples
      ${CMAKE_BINARY_DIR}/test-output
  )
  set_tests_properties(integration PROPERTIES
    DEPENDS unit
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    TIMEOUT 300
  )
endif()

if(BUILD_TESTING)
  add_executable(vga_text_tests tests/vga_text_tests.cpp)
  target_link_libraries(vga_text_tests PRIVATE machinelab_core)
  add_test(NAME vga_text COMMAND vga_text_tests)

  add_executable(compat_vga_text_client tests/compat_vga_text.c)
  target_link_libraries(compat_vga_text_client PRIVATE lcom_minix)
  add_test(NAME compat_vga_text
    COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tests/compat_vga_text.py
      $<TARGET_FILE:machinelab> $<TARGET_FILE:compat_vga_text_client>
      ${CMAKE_BINARY_DIR}/test-output/vga-text)
  set_tests_properties(compat_vga_text PROPERTIES TIMEOUT 30)
endif()
