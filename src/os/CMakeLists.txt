set(PLATFORM_SOURCE_FILES
        sleeper.cpp
        browse_dialog.cpp
)

add_library(os STATIC ${PLATFORM_SOURCE_FILES})
target_include_directories(os PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(os PUBLIC
        $<$<BOOL:${WIN32}>:xinput winmm>)