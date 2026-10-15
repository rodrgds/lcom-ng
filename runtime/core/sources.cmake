# Shared device core, used by native and WebAssembly builds.
get_filename_component(MACHINE_LAB_RUNTIME_DIR "${CMAKE_CURRENT_LIST_DIR}/.." ABSOLUTE)
set(MACHINE_LAB_CORE_SOURCES
  ${MACHINE_LAB_RUNTIME_DIR}/core/Trace.cpp
  ${MACHINE_LAB_RUNTIME_DIR}/core/IrqController.cpp
  ${MACHINE_LAB_RUNTIME_DIR}/core/Bus.cpp
  ${MACHINE_LAB_RUNTIME_DIR}/core/Machine.cpp
  ${MACHINE_LAB_RUNTIME_DIR}/core/Script.cpp
  ${MACHINE_LAB_RUNTIME_DIR}/backends/AudioBackend.cpp
  ${MACHINE_LAB_RUNTIME_DIR}/devices/Ac97.cpp
  ${MACHINE_LAB_RUNTIME_DIR}/devices/Pit8254.cpp
  ${MACHINE_LAB_RUNTIME_DIR}/devices/I8042.cpp
  ${MACHINE_LAB_RUNTIME_DIR}/devices/RtcCmos.cpp
  ${MACHINE_LAB_RUNTIME_DIR}/devices/Vbe.cpp
  ${MACHINE_LAB_RUNTIME_DIR}/devices/Uart16550.cpp
)
