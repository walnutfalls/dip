set(PLATFORM_SOURCE_FILES
        sleeper.cpp
        browse_dialog.cpp
        portable-file-dialogs.h
)

add_library(os STATIC ${PLATFORM_SOURCE_FILES})
target_include_directories(os PUBLIC ${PROJECT_SOURCE_DIR}/src)
target_link_libraries(os PUBLIC
        $<$<BOOL:${WIN32}>:xinput winmm>)