cmake_minimum_required(VERSION 3.2)

project(hello_world LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-DGLM_FORCE_CXX14 -DEVENTS_AS_CALLBACKS -DGLM_FORCE_SWIZZLE -Wno-psabi)

# The compiled ADA 
add_subdirectory(deps)
target_compile_definitions(ada PUBLIC EVENTS_AS_CALLBACKS)

file(GLOB ROOT_SOURCE 
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
)

add_executable(hello_world
    ${ROOT_SOURCE}
)

set_target_properties(hello_world PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)

# copy all files on assets on the build folder
file(COPY   assets/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )

if (EMSCRIPTEN)

    # create a hello_world.html 
    # set_target_properties(hello_world PROPERTIES SUFFIX ".html")

    # copy the index.html 
    file(COPY   assets/wasm/index.html DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )
    file(COPY   assets/wasm/thumbnail.png DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )

    set(LFLAGS "${LFLAGS} -pthread")
    set(LFLAGS "${LFLAGS} -s WASM=1")
    set(LFLAGS "${LFLAGS} -s USE_GLFW=3")
    # set(LFLAGS "${LFLAGS} -s FULL_ES2=1")
    # set(LFLAGS "${LFLAGS} -s FULL_ES3=1")
    # set(LFLAGS "${LFLAGS} -s MIN_WEBGL_VERSION=2")
    set(LFLAGS "${LFLAGS} -s MAX_WEBGL_VERSION=2")
    # set(LFLAGS "${LFLAGS} -s WEBGL2_BACKWARDS_COMPATIBILITY_EMULATION=1")
    # set(LFLAGS "${LFLAGS} -s ALLOW_MEMORY_GROWTH=1")
    set(LFLAGS "${LFLAGS} -s NO_DYNAMIC_EXECUTION=1")

    # set(LFLAGS "${LFLAGS} --preload-file file_in_assets_folder")
    
    set_target_properties(hello_world PROPERTIES LINK_FLAGS "${LFLAGS}")
    target_link_libraries(hello_world PRIVATE ada glfw )
    
else()
    target_link_libraries(hello_world PRIVATE ada)

endif()


