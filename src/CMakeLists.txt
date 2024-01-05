
add_compile_options(-Wall -Wextra -pedantic -Werror -fvisibility=hidden -Wno-gnu-zero-variadic-macro-arguments)

include_directories(.)

set(ADAPTER_FILES
    stdio-adapter.c
    fcntl-adapter.c
    errno-adapter.c
    string-adapter.c

    glibc-adapter.c
)

add_library(glibc-adapter SHARED ${ADAPTER_FILES})

target_link_libraries(glibc-adapter PRIVATE log)

install(TARGETS glibc-adapter
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

