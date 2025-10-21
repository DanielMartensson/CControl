cmake_minimum_required(VERSION 3.28.3)
project(CControl)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

# MinGW är inte kompatibel med Intel MKL!
if(MINGW)
    message(STATUS "MinGW detected - MKL will be disabled.")
else()
    # Leta efter MKL (ej REQUIRED, vi hanterar fallback)
    set(MKL_LINK "static")
    set(MKL_THREADING "sequential")   # Viktigt om du vill ha sekventiell MKL
    set(MKL_INTERFACE "ilp64")        # Vanligt standardval för 64-bitars indexering
    set(MKL_MPI "intelmpi")           # Om du använder MPI, annars kan du testa "none" eller ta bort
    find_package(MKL CONFIG REQUIRED)
    message(STATUS "Imported oneMKL targets: ${MKL_IMPORTED_TARGETS}")
endif()

# Hämta alla C filer
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "${PROJECT_SOURCE_DIR}/src/*.c"
    "${PROJECT_SOURCE_DIR}/src/*.cpp"  # Om du har C++
)
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS
    "${PROJECT_SOURCE_DIR}/src/*.h"
)

# Filtrera bort CLapack-källor om MKL hittas
if(MKL_FOUND)
    message(STATUS "MKL found. CLapack sources will be excluded.")
    list(FILTER SOURCES EXCLUDE REGEX ".*/CLapack/.*|.*\\\\CLapack\\\\.*")
else()
    message(STATUS "MKL not found. CLapack sources will be used.")
endif()

# Skapa exekverbart mål
add_executable(${PROJECT_NAME} ${SOURCES})

# Lägg till include paths automatiskt (baserat på .h-filer)
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/**/*.h")
foreach(header ${HEADERS})
    get_filename_component(dir ${header} DIRECTORY)
    list(APPEND INCLUDE_DIRS ${dir})
endforeach()
list(REMOVE_DUPLICATES INCLUDE_DIRS)
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIRS})

# Länka MKL om det finns
if(MKL_FOUND)
    target_link_libraries(${PROJECT_NAME} PRIVATE MKL::MKL)
    target_compile_options(${PROJECT_NAME} PUBLIC $<TARGET_PROPERTY:MKL::MKL,INTERFACE_COMPILE_OPTIONS>)
    target_include_directories(${PROJECT_NAME} PUBLIC $<TARGET_PROPERTY:MKL::MKL,INTERFACE_INCLUDE_DIRECTORIES>)
    target_link_libraries(${PROJECT_NAME} PUBLIC $<LINK_ONLY:MKL::MKL>)
    target_compile_definitions(${PROJECT_NAME} PUBLIC MKL_LAPACK_USED)
    target_compile_definitions(${PROJECT_NAME} PUBLIC MKL_FFT_USED)
else()
    # Comment this line below if you want to use the lightweight algorithms
    target_compile_definitions(${PROJECT_NAME} PUBLIC CLAPACK_USED)
endif()

# Länka math.h
if(UNIX)
    target_link_libraries(${PROJECT_NAME} PUBLIC m)
endif()
