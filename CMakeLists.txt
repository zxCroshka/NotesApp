cmake_minimum_required(VERSION 3.10)
project(NotesApp)

set(CMAKE_CXX_STANDARD 20)

# Установка путей
include_directories(${PROJECT_SOURCE_DIR}/include)

# Список исходных файлов - явное указание
set(SOURCE_FILES
    ${PROJECT_SOURCE_DIR}/src/main.cpp
    ${PROJECT_SOURCE_DIR}/src/Note.cpp
    ${PROJECT_SOURCE_DIR}/src/TextContent.cpp
    ${PROJECT_SOURCE_DIR}/src/ImageContent.cpp
    ${PROJECT_SOURCE_DIR}/src/AudioContent.cpp
    ${PROJECT_SOURCE_DIR}/src/VideoContent.cpp
    ${PROJECT_SOURCE_DIR}/src/NotesManager.cpp
    ${PROJECT_SOURCE_DIR}/src/NoteFactory.cpp
    ${PROJECT_SOURCE_DIR}/src/NotesApp.cpp
)

# Проверка существования файлов
foreach(source_file ${SOURCE_FILES})
    if(NOT EXISTS ${source_file})
        message(WARNING "Файл не найден: ${source_file}")
    endif()
endforeach()

# Создание исполняемого файла
add_executable(NotesApp ${SOURCE_FILES})

# Настройки компиляции
if(MSVC)
    target_compile_options(NotesApp PRIVATE /W4)
else()
    target_compile_options(NotesApp PRIVATE -Wall -Wextra)
endif()