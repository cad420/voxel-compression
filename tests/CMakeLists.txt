find_package(GTest REQUIRED)

file(GLOB_RECURSE VARCH_TEST_SRC
    test_*.cc
)

cuda_add_executable(varch_test_all ${VARCH_TEST_SRC})
target_link_libraries(varch_test_all gtest gtest_main pthread dl)
vm_target_dependency(varch_test_all voxel_archive PRIVATE)
vm_target_dependency(varch_test_all voxel_unarchive PRIVATE)

target_include_directories(varch_test_all PUBLIC
  ${PROJECT_SOURCE_DIR}/src
)
