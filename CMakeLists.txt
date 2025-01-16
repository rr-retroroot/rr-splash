cmake_minimum_required(VERSION 3.15)

project(rr-splash)

file(GLOB SRC
        src/*.c
        src/fbg/*.c
        src/lodepng/*.c
        )

if (RETROROOT)
    list(APPEND SRC src/fbg/fbdev/fbg_fbdev.c)
    list(APPEND INC src/fbg/fbdev)
    list(APPEND CFLAGS -D__RETROROOT__)
else ()
    find_package(GLEW REQUIRED)
    find_package(glfw3 3.3 REQUIRED)
    find_package(OpenGL REQUIRED)
    list(APPEND SRC src/fbg/glfw/fbg_glfw.c)
    list(APPEND INC src/fbg/glfw ${GLEW_INCLUDE_DIRS} ${GLFW_INCLUDE_DIRS})
    list(APPEND LIBS ${GLEW_LIBRARIES} glfw OpenGL::GL)
endif ()

add_executable(rr-splash ${SRC})
target_compile_definitions(rr-splash PRIVATE -DWITHOUT_JPEG -DWITHOUT_STB_IMAGE ${CFLAGS})
target_include_directories(rr-splash PRIVATE src src/fbg ${INC})
target_link_libraries(rr-splash PRIVATE ${LIBS} m)

#####################
# installation
#####################
install(PROGRAMS ${PROJECT_BINARY_DIR}/${CMAKE_PROJECT_NAME} DESTINATION bin)
install(FILES ${CMAKE_SOURCE_DIR}/data/logo.png DESTINATION share/retroroot/rr-splash)
install(FILES ${CMAKE_SOURCE_DIR}/data/logo-90.png DESTINATION share/retroroot/rr-splash)
