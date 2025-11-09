# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "25_CPP_autogen"
  "CMakeFiles\\25_CPP_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\25_CPP_autogen.dir\\ParseCache.txt"
  )
endif()
