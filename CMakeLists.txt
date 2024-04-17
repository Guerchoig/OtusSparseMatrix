cmake_minimum_required(VERSION 3.10)

set(PATCH_VERSION "0" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(main VERSION ${PROJECT_VESRION})

# include(FetchContent)
# FetchContent_Declare(
#   googletest
#   URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
# )
# For Windows: Prevent overriding the parent project's compiler/linker settings
# set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
# FetchContent_MakeAvailable(googletest)

# configure_file(config.h.in config.h)

add_executable(sparce_matrix sparce_matrix.cpp)
# add_library(main_lib main_lib.cpp)
# add_executable(test_main test_main.cpp)

set_target_properties(sparce_matrix  PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
)
target_include_directories(sparce_matrix
    PRIVATE "${CMAKE_BINARY_DIR}"
)

# target_link_libraries(main PRIVATE main_lib)
# target_link_libraries(test_main
#     GTest::gtest_main
#     main_lib
# )

if (MSVC)
    target_compile_options(sparce_matrix PRIVATE
        /W4
    )
    #  target_compile_options(test_main PRIVATE
    #     /W4
    # )
else ()
    target_compile_options(sparce_matrix PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    
endif()



install(TARGETS sparce_matrix RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT alex-guerchoig@yandex.ru)

include(CPack)

# enable_testing()
# include(GoogleTest)
# gtest_discover_tests(test_main)
# add_test(test_main  test_main)



