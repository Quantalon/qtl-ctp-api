cmake_minimum_required(VERSION 3.15...3.26)

project(qtl-ctp-api LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

if (NOT SKBUILD)
    message(WARNING "\
  This CMake file is meant to be executed using 'scikit-build'. Running
  it directly will almost certainly not produce the desired result. If
  you are a user trying to install this package, please use the command
  below, which will install all necessary build dependencies, compile
  the package in an isolated environment, and then install it.
  =====================================================================
   $ pip install .
  =====================================================================
  If you are a software developer, and this is your own package, then
  it is usually much more efficient to install the build dependencies
  in your environment once and use the following command that avoids
  a costly creation of a new virtual environment at every compilation:
  =====================================================================
   $ pip install nanobind scikit-build-core[pyproject]
   $ pip install --no-build-isolation -ve .
  =====================================================================
  You may optionally add -Ceditable.rebuild=true to auto-rebuild when
  the package is imported. Otherwise, you need to re-run the above
  after editing C++ files.")
endif()

set(CMAKE_INSTALL_RPATH $ORIGIN)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

find_package(Python 3.8
        REQUIRED COMPONENTS Interpreter Development.Module
        OPTIONAL_COMPONENTS Development.SABIModule)

execute_process(
        COMMAND "${Python_EXECUTABLE}" -m nanobind --cmake_dir
        OUTPUT_STRIP_TRAILING_WHITESPACE OUTPUT_VARIABLE nanobind_ROOT)

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

install(TARGETS _ctp LIBRARY DESTINATION qtl_ctp_api)
install(FILES
        libs/ctp/lib/libthostmduserapi_se.so
        libs/ctp/lib/libthosttraderapi_se.so
        DESTINATION qtl_ctp_api)
