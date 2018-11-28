# CMakeLists.txt for event package. It creates a library with dictionary and a main program
cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(ModelTree)

# You need to tell CMake where to find the ROOT installation. This can be done in a number of ways:
#   - ROOT built with classic configure/make use the provided $ROOTSYS/etc/cmake/FindROOT.cmake
#   - ROOT built with CMake. Add in CMAKE_PREFIX_PATH the installation prefix for ROOT
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})

#---Locate the ROOT package and defines a number of variables (e.g. ROOT_INCLUDE_DIRS)
find_package(ROOT REQUIRED COMPONENTS MathCore RIO Hist Tree Net)

#---Define useful ROOT functions and macros (e.g. ROOT_GENERATE_DICTIONARY)
include(${ROOT_USE_FILE})

set(INCLUDE_DIRECTORIES
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/Converters
  ${CMAKE_SOURCE_DIR}/macros
  ${ROOT_INCLUDE_DIRS}
)

include_directories(${INCLUDE_DIRECTORIES}
)
add_definitions(${ROOT_CXX_FLAGS})

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_C_FLAGS "-O2")

set(ModelTree_headers_files
  Track.h
  EventHeader.h
  Event.h
)

set(ModelTree_cxx_files
  Track.cxx
  EventHeader.cxx
  Event.cxx
)

# add_subdirectory(Converters)

#---Generate dictionary for ModelTree
ROOT_GENERATE_DICTIONARY(G__ModelTree
  ${ModelTree_headers_files}
LINKDEF ModelTree.LinkDef.h)

#---Create a shared library with geneated dictionary for ModelTree class
add_library(ModelTree SHARED
  ${ModelTree_cxx_files}
G__ModelTree.cxx)

target_link_libraries(ModelTree ${ROOT_LIBRARIES})

add_subdirectory(Converters)
add_subdirectory(macros)