
include_directories(.)
include_directories(../src)

set(TEST_FILES
   test-glibc-adapter.c
)

add_executable(test-adapter ${TEST_FILES})
target_link_libraries(test-adapter PRIVATE glibc-adapter)