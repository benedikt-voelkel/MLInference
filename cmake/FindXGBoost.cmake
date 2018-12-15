# Find XGBoost

set(XGBoost_FOUND FALSE)
if(NOT XGBoost_DIR)
  message(STATUS "XGBoost cannot be found. Specify -DXGBoost_DIR pointing to the top
                  of the install tree.")
  return()
endif()

set(LIBRARY_NAME xgboost)

find_path(XGBoost_INCLUDE_DIR
          NAMES ${LIBRARY_NAME}/c_api.h
          PATHS ${XGBoost_DIR}/include
)
find_path(XGBoost_LIBRARY_DIR
          NAMES "lib${LIBRARY_NAME}.so"
          PATHS ${XGBoost_DIR}/lib
)

if(NOT XGBoost_INCLUDE_DIR )
  message(FATAL_ERROR "Could not find XGBoost include")
endif()

if(NOT XGBoost_LIBRARY_DIR)
  message(FATAL_ERROR "Could not find XGBoost libs")
endif()

set(XGBoost_LIBRARIES "-L${XGBoost_LIBRARY_DIR} -l${LIBRARY_NAME}")

# Everything found
message(STATUS "Found XGBoost")
message(STATUS "Found XGBoost includes in ${XGBoost_INCLUDE_DIR}")
message(STATUS "Found XGBoost libraries in ${XGBoost_LIBRARY_DIR}")
set(XGBoost_FOUND TRUE)
