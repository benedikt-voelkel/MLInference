# Find LWTNN

set(LWTNN_FOUND FALSE)
if(NOT LWTNN_DIR)
  message(STATUS "LWTNN cannot be found. Specify -DLWTNN_DIR pointing to the top
                  of the install tree.")
  return()
endif()

set(LIBRARY_NAME lwtnn)

find_path(LWTNN_INCLUDE_DIR
          NAMES ${LIBRARY_NAME}/Graph.hh
          PATHS ${LWTNN_DIR}/include
)
find_path(LWTNN_LIBRARY_DIR
          NAMES "lib${LIBRARY_NAME}.so"
          PATHS ${LWTNN_DIR}/lib
)
find_path(LWTNN_CONVERTER_DIR
          NAMES "kerasfunc2json.py"
          PATHS ${LWTNN_DIR}/converters
)
find_path(LWTNN_BIN_DIR
          NAMES "lwtnn-test-graph"
          PATHS ${LWTNN_DIR}/bin
)

if(NOT LWTNN_INCLUDE_DIR AND NOT LWTNN_LIBRARY_DIR)
  message(FATAL_ERROR "Could not find LWTNN")
endif()

set(LWTNN_LIBRARIES "-L${LWTNN_LIBRARY_DIR} -l${LIBRARY_NAME}")

# Everything found
message(STATUS "Found LWTNN")
message(STATUS "Found LWTNN includes in ${LWTNN_INCLUDE_DIR}")
message(STATUS "Found LWTNN libraries in ${LWTNN_LIBRARY_DIR}")
message(STATUS "Found LWTNN convrters in ${LWTNN_CONVERTER_DIR}")
message(STATUS "Found LWTNN binaries in ${LWTNN_BIN_DIR}")
set(LWTNN_FOUND TRUE)
