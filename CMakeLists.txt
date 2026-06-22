cmake_minimum_required(VERSION 3.10) # Указываем минимальную версию CMake

# Задаём имя проекта
project(ConsoleApp)

# Устанавливаем стандарт языка C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

foreach(CONFIG Debug Release RelWithDebInfo MinSizeRel)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CONFIG} ${CMAKE_SOURCE_DIR}/bin)
endforeach()

# Добавляем исполняемый файл из вашего исходного кода
add_executable(${PROJECT_NAME}
        main.cpp
        fileUtils.cpp
        fileUtils.h
        drawManager.cpp
        drawManager.h
        eventsHandler.cpp
        eventsHandler.h
)

if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /utf-8)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -finput-charset=UTF-8 -fexec-charset=UTF-8)
endif()
