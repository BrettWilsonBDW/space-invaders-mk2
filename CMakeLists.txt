cmake_minimum_required(VERSION 3.22)

# Get project name from the folder name
get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)
project(${PROJECT_NAME} CXX)

set(CMAKE_CXX_STANDARD 20)

##########################################################################################
# Configurable options
##########################################################################################

# library toggles
option(USE_SDL3 "Use SDL3" ON)
option(USE_SDL2 "Use SDL2" OFF)
option(USE_GLAD "Use Glad" OFF)
option(USE_IMGUI "Use ImGui" OFF)
option(USE_LDTKLOADER "Use LDtk Loader" OFF)
option(USE_FMT "Use fmt" OFF)
option(USE_BOX2D "Use Box2D" OFF)
option(USE_BULLET3 "Use Bullet Physics 3D" OFF)
option(USE_REACTPHYSICS3D "Use React Physics 3D" OFF)
option(USE_JOLTPHYSICS "Use Jolt Physics 3D" OFF)
option(USE_PHYSX "Use PhysX" OFF)

# build options
option(BUILD_SDL_SHARED_LIB "Build SDL as a shared library" OFF)

##########################################################################################
# MSVC runtime linking
##########################################################################################

# this is here to ensure the runtime is linked staticly for msvc
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDebug")
    else()
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")
    endif()
endif()

##########################################################################################
# Platform Configuration
##########################################################################################

if (NOT DEFINED PLATFORM)
    set(PLATFORM "default")
endif()

##########################################################################################
# Add dependencies with FetchContent
##########################################################################################

# libraries array to be linked
set(LIBRARIES ${PROJECT_NAME})

include(FetchContent)
function(add_git_dependency libName gitURL gitTag)

        FetchContent_Declare(${libName}
            GIT_REPOSITORY ${gitURL}
            GIT_TAG        ${gitTag}
            GIT_SHALLOW    TRUE
            GIT_PROGRESS   TRUE
        )

        if(NOT ("${libName}" STREQUAL "libname"))
            FetchContent_MakeAvailable(${libName})
        endif()

endfunction()

##########################################################################################
# SDL3 Setup
##########################################################################################

if(USE_SDL3)
    set(SDL3IMAGE_INSTALL OFF)
    set(BUILD_SHARED_LIBS FALSE)
    set(SDL_LIB_TYPE static)

    if(BUILD_SDL_SHARED_LIB)
        set(BUILD_SHARED_LIBS TRUE)
        set(SDL_LIB_TYPE shared)
        option(SDL_SHARED "Build SDL as a shared library" ON)
    endif()
    
    set(dep SDL3)
    add_git_dependency(
        ${dep}
        https://github.com/libsdl-org/SDL.git
        head
    )
    
    FetchContent_GetProperties(${dep})
    if(NOT ${dep}_POPULATED)
    endif()

    list(APPEND LIBRARIES SDL3::SDL3-${SDL_LIB_TYPE})
endif()

if(USE_SDL3)
    set(SDL3IMAGE_INSTALL OFF)

    if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        set(SDLIMAGE_AVIF OFF CACHE BOOL "Support loading AVIF images" FORCE)
    endif()
    
    set(dep SDL3_image)
    add_git_dependency(
        ${dep}
        https://github.com/libsdl-org/SDL_image.git
        head
    )
    
    FetchContent_GetProperties(${dep})
    if(NOT ${dep}_POPULATED)
    endif()

    include_directories(RECURSE build/_deps/sdl3-src/include/SDL3/)
        
    list(APPEND LIBRARIES SDL3_image::SDL3_image-${SDL_LIB_TYPE})
endif()

##########################################################################################
# SDL2 Setup
##########################################################################################

if(USE_SDL2 AND NOT ${PLATFORM} STREQUAL "Web")
    set(CMAKE_USE_FORWARD_SLASHES ON)

    set(SDL2IMAGE_INSTALL OFF)
    set(BUILD_SHARED_LIBS FALSE)

    if(BUILD_SDL_SHARED_LIB)
        set(BUILD_SHARED_LIBS TRUE)
        option(SDL_SHARED "Build SDL as a shared library" ON)
    endif()

    set(dep SDL2)
    add_git_dependency(
        ${dep}
        https://github.com/libsdl-org/SDL.git
        SDL2
    )
    
    FetchContent_GetProperties(${dep})
    if(NOT ${dep}_POPULATED)
    endif()

    list(APPEND LIBRARIES SDL2main)
    
    if(BUILD_SDL_SHARED_LIB)
        list(APPEND LIBRARIES SDL2)
    else()
        list(APPEND LIBRARIES SDL2-static)
    endif()
endif()

if(USE_SDL2 AND NOT ${PLATFORM} STREQUAL "Web")
    set(SDL2IMAGE_INSTALL OFF)

    if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        set(SDL2IMAGE_AVIF OFF CACHE BOOL "Support loading AVIF images" FORCE)
    endif()
    
    set(dep SDL2_image)
    add_git_dependency(
        ${dep}
        https://github.com/libsdl-org/SDL_image.git
        SDL2
    )
    
    FetchContent_GetProperties(${dep})
    if(NOT ${dep}_POPULATED)
    endif()

    include_directories(RECURSE build/_deps/sdl2-src/include/SDL2/)

    list(APPEND LIBRARIES SDL2_image)
endif()

if((USE_SDL2) AND (${PLATFORM} STREQUAL "Web"))
    FetchContent_Declare(SDL2
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG        SDL2
        GIT_SHALLOW    TRUE
        GIT_PROGRESS   TRUE
    )

    if(NOT SDL2_POPULATED)
        FetchContent_Populate(SDL2)
    endif()

    FetchContent_Declare(SDL2_image
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
        GIT_TAG        SDL2
        GIT_SHALLOW    TRUE
        GIT_PROGRESS   TRUE
    )

    if(NOT SDL2_image_POPULATED)
        FetchContent_Populate(SDL2_image)
    endif()

    include_directories(RECURSE build-em/_deps/sdl2-src/include)
    include_directories(RECURSE build-em/_deps/sdl2_image-src/include)
endif()


##########################################################################################
# Glad Setup
##########################################################################################

if(USE_GLAD)
    set(dep glad)
    add_git_dependency(
        ${dep}
        https://github.com/Dav1dde/glad.git
        master
    )
    
    FetchContent_GetProperties(${dep})
    if(NOT ${dep}_POPULATED)
        FetchContent_Populate(${dep})

        set(GLAD_PROFILE "core" CACHE STRING "OpenGL profile")
        set(GLAD_API "gl=" CACHE STRING "API type/version pairs, like \"gl=3.2,gles=\", no version means latest")
        set(GLAD_GENERATOR "c" CACHE STRING "Language to generate the binding for")

        find_package(OpenGL REQUIRED)
        
        add_subdirectory(${${dep}_SOURCE_DIR} ${${dep}_BINARY_DIR})
    endif()

    # add the dependency here to be linked
    list(APPEND LIBRARIES ${dep})
endif()


##########################################################################################
# ImGui Setup
##########################################################################################

if(USE_IMGUI)
    add_git_dependency(
        imgui
        https://github.com/ocornut/imgui.git
        head
    )

    FetchContent_GetProperties(imgui)
    if (NOT imgui_POPULATED)
        FetchContent_Populate(imgui)
        add_subdirectory(${imgui_SOURCE_DIR})
    endif()

    if(USE_SDL3)
        set(SDL_VERSION 3)
    elseif(USE_SDL2)
        set(SDL_VERSION 2)
    else()
        message(FATAL_ERROR "Either USE_SDL3 or USE_SDL2 must be defined")
    endif()

    add_library(imgui
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui.h
        ${imgui_SOURCE_DIR}/imconfig.h
        ${imgui_SOURCE_DIR}/imgui_demo.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_internal.h
        ${imgui_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp
        ${imgui_SOURCE_DIR}/imstb_rectpack.h
        ${imgui_SOURCE_DIR}/imstb_textedit.h
        ${imgui_SOURCE_DIR}/imstb_truetype.h
        ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl${SDL_VERSION}.h
        ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl${SDL_VERSION}.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_sdlrenderer${SDL_VERSION}.h
        ${imgui_SOURCE_DIR}/backends/imgui_impl_sdlrenderer${SDL_VERSION}.cpp
    )

    target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR})
    target_include_directories(imgui PRIVATE ${CMAKE_CURRENT_LIST_DIR}/build/_deps/sdl${SDL_VERSION}-src/include/)

    if(BUILD_SDL_SHARED_LIB)
        if (USE_SDL3)
            target_link_libraries(imgui SDL3::SDL3-shared)
        else()
            target_link_libraries(imgui SDL2)
        endif()
    else()
        if (USE_SDL3)
            target_link_libraries(imgui SDL3::SDL3-static)
        else()
            target_link_libraries(imgui SDL2-static)
        endif()
    endif()

    FetchContent_MakeAvailable(imgui)
    list(APPEND LIBRARIES imgui)
endif()

##########################################################################################
# LDtk Loader Setup
##########################################################################################

if(USE_LDTKLOADER)
    set(dep LDtkLoader)
    add_git_dependency(
        ${dep}
        https://github.com/Madour/LDtkLoader.git
        head
    )

    FetchContent_GetProperties(${dep})

    # add the dependency here to be linked
    list(APPEND LIBRARIES ${dep})
endif()

##########################################################################################
# fmt Setup
##########################################################################################

if(USE_FMT)
    set(dep fmt)
    add_git_dependency(
        ${dep}
        https://github.com/fmtlib/fmt.git
        head
    )

    FetchContent_GetProperties(${dep})
        
    # add the dependency here to be linked
    list(APPEND LIBRARIES ${dep})
endif()

##########################################################################################
# Box2D Setup
##########################################################################################

if(USE_BOX2D)
    set(BOX2D_BUILD_TESTBED OFF CACHE BOOL "" FORCE) # don't build the supplied examples
    set(BOX2D_BUILD_UNIT_TESTS OFF CACHE BOOL "" FORCE) # don't build the supplied examples

    set(dep box2d)
    add_git_dependency(
        ${dep}
        https://github.com/erincatto/box2d.git
        head
    )

    # add the dependency here to be linked
    list(APPEND LIBRARIES ${dep})
endif()

##########################################################################################
# Bullet Physics 3D Setup
##########################################################################################

if(USE_BULLET3)
    set(BUILD_EXTRAS OFF CACHE BOOL "" FORCE) # don't build the supplied examples
    set(BUILD_CPU_DEMOS OFF CACHE BOOL "" FORCE) # don't build the supplied examples
    set(BUILD_UNIT_TESTS OFF CACHE BOOL "" FORCE) # don't build the supplied examples
    set(BUILD_BULLET2_DEMOS OFF CACHE BOOL "" FORCE) # don't build the supplied examples

    set(dep BULLET_PHYSICS3)
    add_git_dependency(
        ${dep}
        https://github.com/bulletphysics/bullet3.git
        head
    )

    FetchContent_GetProperties(${dep})
    if(NOT ${dep}_POPULATED)
        
    endif()

    include_directories(RECURSE build/_deps/bullet_physics3-src/src)
        
    # add the dependency here to be linked BulletDynamics BulletCollision LinearMath BulletSoftBody
    list(APPEND LIBRARIES BulletDynamics)
    list(APPEND LIBRARIES BulletCollision)
    list(APPEND LIBRARIES LinearMath)
    list(APPEND LIBRARIES BulletSoftBody)
endif()

##########################################################################################
# ReactPhysics3D Setup
##########################################################################################

if(USE_REACTPHYSICS3D)
    set(dep reactphysics3d)
    add_git_dependency(
        ${dep}
        https://github.com/DanielChappuis/ReactPhysics3D.git
        head
    )
    FetchContent_GetProperties(${dep})

    include_directories(RECURSE build/_deps/reactphysics3d-src/include)
        
    # add the dependency here to be linked
    list(APPEND LIBRARIES ${dep})
endif()

##########################################################################################
# Jolt Physics Setup
##########################################################################################

if(USE_JOLTPHYSICS)
    set(DOUBLE_PRECISION OFF)
    set(GENERATE_DEBUG_SYMBOLS ON)
    set(CROSS_PLATFORM_DETERMINISTIC OFF)
    set(INTERPROCEDURAL_OPTIMIZATION ON)
    set(FLOATING_POINT_EXCEPTIONS_ENABLED OFF)
    set(OBJECT_LAYER_BITS 16)
    set(USE_SSE4_1 ON)
    set(USE_SSE4_2 ON)
    set(USE_AVX ON)
    set(USE_AVX2 ON)
    set(USE_AVX512 OFF)
    set(USE_LZCNT ON)
    set(USE_TZCNT ON)
    set(USE_F16C ON)
    set(USE_FMADD ON)

    if (${PLATFORM} STREQUAL "Web")
        option(ENABLE_MEMORY_PROFILER "Enable emscriptens memory profiler to help find leaks" OFF)
        option(DOUBLE_PRECISION "Compile the library in double precision mode" OFF)
        option(ENABLE_SIMD "Enable SIMD instructions (warning: last major browser to support this was Safari 16.4 in March 2023)" OFF)
        option(ENABLE_MULTI_THREADING "Compile the library in multi threading mode (warning: doesn't work in Safari nor with any JS callbacks currently)'" OFF) # SharedArrayBuffer is not defined when ON
        option(BUILD_WASM_COMPAT_ONLY "Compile online the WASM compat version of the library (speeds up compilation)" ON)

        set(CMAKE_CONFIGURATION_TYPES "Debug;Release;Distribution")
        set(FLOATING_POINT_EXCEPTIONS_ENABLED OFF)
        set(INTERPROCEDURAL_OPTIMIZATION OFF)
        set(GENERATE_DEBUG_SYMBOLS ON)
        set(DISABLE_CUSTOM_ALLOCATOR ON)
        set(OBJECT_LAYER_BITS 32)

        # Enable multi threading
        if (ENABLE_MULTI_THREADING)
            set(MULTI_THREADED_FLAG 
                -pthread 
                -s SHARED_MEMORY)
            set(ENVIRONMENT_FLAG
                -s ENVIRONMENT='web,node,worker' 
                -s PTHREAD_POOL_SIZE=16)
            set(CMAKE_CXX_FLAGS "-pthread -s SHARED_MEMORY")
        else()
            set(MULTI_THREADED_FLAG "")
            set(ENVIRONMENT_FLAG "-s ENVIRONMENT='web,node'")
        endif()
    endif()

    FetchContent_Declare(
        JoltPhysics
        GIT_REPOSITORY "https://github.com/jrouwe/JoltPhysics"
        GIT_TAG "v5.0.0"
        SOURCE_SUBDIR "Build"
    )
    FetchContent_MakeAvailable(JoltPhysics)

    set(CMAKE_EXE_LINKER_FLAGS_DISTRIBUTION "${CMAKE_EXE_LINKER_FLAGS_RELEASE}")
    
    # add the dependency here to be linked
    list(APPEND LIBRARIES Jolt)
endif()

##########################################################################################
# Physx Setup
##########################################################################################

if(USE_PHYSX)
    if (${PLATFORM} STREQUAL "Web")
        set(physx_tag "PhysX-web")
        set(build_dir "build-em")
    else()
        set(physx_tag "head")
        set(build_dir "build")
    endif()

    if(WIN32)
        option(USE_UNIX "" OFF)
    endif()

    set(dep physx)
    add_git_dependency(
        ${dep}
        https://github.com/BrettWilsonBDW/PhysX-cmake.git
        ${physx_tag}
    )

    include_directories(${CMAKE_CURRENT_LIST_DIR}/${build_dir}/_deps/physx-src/physx/include)

    list(APPEND LIBRARIES ${dep})
endif()


##########################################################################################
# Project executable setup
##########################################################################################

# Adding our source files
# Define PROJECT_SOURCES as a list of all source files
file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/src/*.cpp")

# Define PROJECT_INCLUDE to be the path to the include directory of the project
set(PROJECT_INCLUDE "${CMAKE_CURRENT_LIST_DIR}/src/")

# Declaring our executable
add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_INCLUDE})

##########################################################################################
# Compiler and linker flags
##########################################################################################

if(NOT ${PLATFORM} STREQUAL "Web")
    if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++ -static -pthread")
        target_link_libraries(${LIBRARIES} -static-libgcc -static-libstdc++ -static -lpthread -Wl,-Bstatic -lstdc++)
        # dont forget to set  "cmake.configureSettings": { "CMAKE_BUILD_TYPE": "${buildType}" } in settings.json for vs code
        if (CMAKE_BUILD_TYPE STREQUAL "Debug")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
        endif()
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        target_link_libraries(${LIBRARIES} -static-libgcc -static-libstdc++ -static -pthread -Wl,-Bstatic -lstdc++)
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
        target_link_libraries(${LIBRARIES} -static-intel -lpthread -Wl,-Bstatic -lstdc++) 
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        target_link_libraries(${LIBRARIES})

        if(OFF)
            set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")  
        endif()

        # dont forget to set  "cmake.configureSettings": { "CMAKE_BUILD_TYPE": "${buildType}" } in settings.json for vs code
        if (CMAKE_BUILD_TYPE STREQUAL "Debug")
            message("-- console enabled")
            set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDebug")
        else()
            set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")  
        endif()
    else()
        target_link_libraries(${LIBRARIES})
    endif()
else()
    target_link_libraries(${LIBRARIES})
endif()

##########################################################################################
# Build flags and additional settings
##########################################################################################

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/assets)
    message("copying assets in build")
    set(destination_dir ${CMAKE_CURRENT_BINARY_DIR}/assets)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        if (CMAKE_BUILD_TYPE STREQUAL "Debug")
            set(destination_dir ${CMAKE_CURRENT_BINARY_DIR}/debug/assets)
        else()
            set(destination_dir ${CMAKE_CURRENT_BINARY_DIR}/release/assets)
        endif()
    endif()
    add_custom_target(copyAssetsOver
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/assets
        ${destination_dir}
        COMMENT "Copying assets to build directory"
    )
    add_dependencies(${PROJECT_NAME} copyAssetsOver) 
endif()

add_definitions( -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} )

# for web build when using emscripten
if(ASYNCIFY)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s ASYNCIFY")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
endif()

if (useReactPhysics3D)
    set(CMAKE_CXX_FLAGS "-m64")
endif()

# Setting ASSETS_PATH
if (CMAKE_BUILD_TYPE MATCHES "Debug")
    SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -O0 -DDEBUG")
    message("-- console enabled")
    if(NOT noAssets AND NOT ${PLATFORM} STREQUAL "Web")
        # Set the asset path macro to the absolute path on the dev machine
        target_compile_definitions(${PROJECT_NAME} PUBLIC ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/")   
    endif()
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -DNDEBUG")
    if(NOT noAssets AND NOT ${PLATFORM} STREQUAL "Web")
        # Set the asset path macro in release mode to a relative path that assumes the assets folder is in the same directory as the game executable
        target_compile_definitions(${PROJECT_NAME} PUBLIC ASSETS_PATH="./assets/")
    endif()
    if(WIN32)
      if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mwindows") 
      endif()    
      add_compile_definitions(NDEBUG)
    endif()
endif()

# Set common compiler flags
# SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Wswitch")
# Set common compiler flags
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

if (${PLATFORM} STREQUAL "Web")
    # Tell Emscripten to build an .html file.
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
    SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Os")
    set(CMAKE_EXECUTABLE_SUFFIX ".html") # This line is used to set your executable to build with the emscripten html template so that you can directly open it.
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html")
    if(NOT noAssets)
        if(USE_SDL2)
            set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_SDL=2 -lSDL -s USE_SDL_IMAGE=2 -s ALLOW_MEMORY_GROWTH=1 --use-preload-plugins -s SDL2_IMAGE_FORMATS=\"['png']\" -s GL_UNSAFE_OPTS=0 --preload-file assets/ --shell-file ../vendor/minshell.html")
        endif()
        if(USE_SDL3)
            set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s ALLOW_MEMORY_GROWTH=1 -s GL_UNSAFE_OPTS=0 --use-preload-plugins --preload-file assets/ --shell-file ../vendor/minshell.html")
        endif(USE_SDL3)
        target_compile_definitions(${PROJECT_NAME} PUBLIC ASSETS_PATH="/assets/")
    else()
        if(USE_SDL2)
            set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_SDL=2 -lSDL -s USE_SDL_IMAGE=2 -s ALLOW_MEMORY_GROWTH=1 --use-preload-plugins -s SDL2_IMAGE_FORMATS=\"['png']\" -s GL_UNSAFE_OPTS=0 --shell-file ../vendor/minshell.html")
        endif()
        if(USE_SDL3)
            set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s ALLOW_MEMORY_GROWTH=1 -s GL_UNSAFE_OPTS=0 --use-preload-plugins --shell-file ../vendor/minshell.html")
        endif()
    endif()
endif()

# Ensure that hot-reload is enabled for VS
if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /ZI")
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /SAFESEH:NO")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SAFESEH:NO")
endif()