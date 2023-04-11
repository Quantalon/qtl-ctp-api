cmake_minimum_required(VERSION 3.18...3.22)

project(qtl-ctp-api)

# Perform a release build by default
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

set(CMAKE_INSTALL_RPATH $ORIGIN)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

find_package(Python 3.8 COMPONENTS Interpreter Development.Module REQUIRED)

execute_process(
        COMMAND "${Python_EXECUTABLE}" -m nanobind --cmake_dir
        OUTPUT_STRIP_TRAILING_WHITESPACE OUTPUT_VARIABLE NB_DIR)
list(APPEND CMAKE_PREFIX_PATH "${NB_DIR}")

find_package(nanobind CONFIG REQUIRED)

nanobind_add_module(_ctp STABLE_ABI NB_STATIC
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
