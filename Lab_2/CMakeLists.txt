cmake_minimum_required(VERSION 3.11)

project(functions CXX)

set(CMAKE_CXX_STANDARD 23)

# Специфичные настройки для компилятора Visual Studio
if(MSVC)
    # Генерируем статическую заглушку для динамических библиотек
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS true)

    # Включаем высокий уровень предупреждений
    if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
        string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    endif()

    # Добавляем флаг для поддержки OpenMP SIMD
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /openmp:experimental")
endif()

# Добавляем поддержку OpenMP
find_package(OpenMP REQUIRED)

# Объявляем библиотеку
add_library(matrix SHARED "src/matrix.cc")
target_include_directories(matrix PUBLIC "include")

# Если OpenMP найден, связываем его с библиотекой
if(OpenMP_CXX_FOUND)
    target_link_libraries(matrix PRIVATE OpenMP::OpenMP_CXX)
endif()

# Создаем исполнимый файл для консоли
add_executable(console "console/console.cc")
target_link_libraries(console PRIVATE matrix)

# Если OpenMP найден, связываем его с исполнимым файлом
if(OpenMP_CXX_FOUND)
    target_link_libraries(console PRIVATE OpenMP::OpenMP_CXX)
endif()

