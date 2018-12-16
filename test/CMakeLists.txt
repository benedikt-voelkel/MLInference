# Build tests

# Find boost unit tests dependencies
find_package(Boost REQUIRED COMPONENTS unit_test_framework)
include_directories(${Boost_INCLUDE_DIR})



# The basic test only checks core functionality without any backend.
set(CORE_TARGET runCore)
set(TEST_CORE "test${CORE_TARGET}")
set(JSON_TEST_FILE test_features.json)
file(COPY ${JSON_TEST_FILE} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
add_executable(${CORE_TARGET} "${CORE_TARGET}.cxx")
target_link_libraries(${CORE_TARGET} ${LIBRARY_NAME} ${Boost_LIBRARIES})
add_test(NAME ${TEST_CORE} COMMAND ${CORE_TARGET} ${JSON_TEST_FILE})


# Single tests for each backend
if(_backends)
  foreach(b ${_backends})
    if(NOT EXISTS "test${b}.cxx")
      message(WARNING "No source file exists for testing backend ${b}".)
      continue()
    endif(NOT EXISTS "test${b}.cxx")
    add_executable("test${b}" "test${b}.cxx")
    target_link_libraries("test${b}" ${LIBRARY_NAME} ${Boost_LIBRARIES})
    add_test(NAME "testBackend${b}" COMMAND "test${b}")
    set_test_properties("testBackend${b}" PROPERTIES DEPENDS ${TEST_CORE})
  endforeach()
endif(_backends)
