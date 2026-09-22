cmake_minimum_required(VERSION 3.16)

project(BibliotecaHash LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(biblioteca
    src/catalogo.cpp
    src/funcao_hash.cpp
    src/indice_titulos_hash.cpp
    src/livro.cpp
    src/livros_exemplo.cpp
    src/main.cpp
    src/menu.cpp
    src/normalizacao.cpp
    src/tabela_hash_extensivel_isbn.cpp
)

target_include_directories(biblioteca PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

if(MSVC)
    target_compile_options(biblioteca PRIVATE /W4 /utf-8)
else()
    target_compile_options(biblioteca PRIVATE
        -Wall
        -Wextra
        -pedantic
    )

    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        target_compile_options(biblioteca PRIVATE
            -finput-charset=UTF-8
            -fexec-charset=UTF-8
        )
    endif()
endif()
