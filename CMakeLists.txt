cmake_minimum_required(VERSION 3.15)

project(qtl-ctp-api)

set(CMAKE_INSTALL_RPATH $ORIGIN)

execute_process(
  COMMAND
    "python" -c
    "import pybind11; print(pybind11.get_cmake_dir())"
  OUTPUT_VARIABLE _tmp_dir
  OUTPUT_STRIP_TRAILING_WHITESPACE COMMAND_ECHO STDOUT)
list(APPEND CMAKE_PREFIX_PATH "${_tmp_dir}")

find_package(pybind11 CONFIG REQUIRED)

pybind11_add_module(_ctp MODULE
  src/main.cpp
  src/bind_consts.cpp
  src/bind_md_api.cpp
  src/bind_td_api.cpp
  src/md_api.cpp
  src/td_api.cpp
  src/dispatch_queue.cpp
)
target_include_directories(_ctp PRIVATE libs/ctp/include)
target_link_directories(_ctp PRIVATE libs/ctp/lib)
target_link_libraries(_ctp PRIVATE thostmduserapi_se thosttraderapi_se)

install(TARGETS _ctp DESTINATION .)
install(FILES
        libs/ctp/lib/libthostmduserapi_se.so
        libs/ctp/lib/libthosttraderapi_se.so
        DESTINATION .)
