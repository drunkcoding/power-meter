cmake_minimum_required(VERSION "3.12.1")
project (POWER_METER)

set(CMAKE_CXX_STANDARD 11)
set(CUDA_NVCC_FLAGS "${CUDA_NVCC_FLAGS} -std=c++11" )

find_package(CUDA 9.1 REQUIRED)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/release-1.11.0.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


# FetchContent_Declare(
#   pcm
#   URL https://github.com/opcm/pcm/archive/refs/tags/202202.zip
# )
# FetchContent_MakeAvailable(pcm)

enable_testing()

include_directories(
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/include
  ${googletest_SOURCE_DIR}/googletest/include/gtest
  ${pcm_SOURCE_DIR}/src
)

link_directories(${PROJECT_SOURCE_DIR}/../pcm/build/lib)

file(GLOB INCLUDE_SRC 
  ${PROJECT_SOURCE_DIR}/include/gpu_meter.cc
  ${PROJECT_SOURCE_DIR}/include/log_stream.cc
  ${PROJECT_SOURCE_DIR}/include/logging.cc
  # ${PROJECT_SOURCE_DIR}/include/cpu_meter.cc
)
# file(GLOB PCM_SRC ${pcm_SOURCE_DIR}/src/*.cpp)

file(GLOB STRUCT_SRC tests/test_sanity.cc)

cuda_add_executable(
  test_sanity
  ${STRUCT_SRC}
  ${INCLUDE_SRC}
  # ${PCM_SRC}
)
target_link_libraries(
  test_sanity
  gtest_main
  nvidia-ml
  cublas
)

include(GoogleTest)
gtest_discover_tests(test_sanity)

# add_subdirectory (include)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11 -O3 -Wall")
