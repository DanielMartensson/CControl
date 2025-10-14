cmake_minimum_required(VERSION 3.28.3)
project(CControl)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE "Release")

# Leta efter MKL (ej REQUIRED, vi hanterar fallback)
find_package(MKL CONFIG REQUIRED)

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

# Länka mot MKL med sekventiell backend
if(MKL_FOUND)
    # Manuell länkning till MKL i sekventiellt läge
    target_link_libraries(${PROJECT_NAME} PRIVATE
        mkl_intel_ilp64
        mkl_sequential
        mkl_core
        pthread
        m
        dl
    )

    target_compile_definitions(CControl PUBLIC MKL_LAPACK_USED)
    target_compile_definitions(CControl PUBLIC MKL_FFT_USED)
else()
    target_compile_definitions(CControl PUBLIC CLAPACK_USED)
endif()