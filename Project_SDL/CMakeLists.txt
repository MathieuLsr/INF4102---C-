cmake_minimum_required (VERSION 3.0)
project ("Project_SDL_sub")

IF(WIN32)
  message(STATUS "Building for windows")

  set(SDL2_INCLUDE_DIRS "SDL2/include")
  set(SDL2IMAGE_INCLUDE_DIRS "SDL2_image/include")

  # Support both 32 and 64 bit builds
  if (${CMAKE_SIZEOF_VOID_P} MATCHES 8)
    message(STATUS "Building for 64bit")
    set(SDL2_LINK_DIRS "SDL2/lib/x64")
    set(SDL2IMAGE_LINK_DIRS "SDL2_image/lib/x64")
  else ()
    message(STATUS "Building for 32bit")
    set(SDL2_LINK_DIRS "SDL2/lib/x86")
    set(SDL2IMAGE_LINK_DIRS "SDL2_image/lib/x86")
  endif ()

  include_directories(${SDL2_INCLUDE_DIRS})
  include_directories(${SDL2IMAGE_INCLUDE_DIRS})
  link_directories(${SDL2_LINK_DIRS}, ${SDL2IMAGE_LINK_DIRS})

  add_executable(SDL_part1 main.cpp Project_SDL1.cpp)
  target_link_libraries(SDL_part1 PUBLIC SDL2 SDL2main SDL2_image)
ELSE()
  message(STATUS "Building for Linux or Mac")

  include_directories(${SDL2_INCLUDE_DIRS})
  include_directories("/usr/local/Cellar/sdl2/2.24.1/include/")
  include_directories("/usr/local/Cellar/sdl2/2.24.1/include/SDL2/")
  include_directories("/usr/local/Cellar/sdl2_image/2.6.2/include/")
  include_directories("/usr/local/Cellar/sdl2_image/2.6.2/include/SDL2/")

  link_directories("/usr/local/Cellar/sdl2/2.24.1/lib/")
  link_directories("/usr/local/Cellar/sdl2_image/2.6.2/lib/")

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -l SDL2 -l SDL2_image")

  #find_package(SDL2 REQUIRED)
  #find_package(SDL2_IMAGE REQUIRED)
  #include_directories(${SDL2_INCLUDE_DIRS})
  #include_directories(${SDL2_IMAGE_INCLUDE_DIRS})

  add_executable(SDL_part1 main.cpp Project_SDL1.cpp)
  #target_link_libraries(SDL_part1 ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})
ENDIF()