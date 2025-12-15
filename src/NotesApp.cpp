#include "NotesApp.h"
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <locale>

void NotesApp::run() {
    // Установка локали для русского языка
    std::locale::global(std::locale(""));
    std::cout.imbue(std::locale());
    
    std::cout << "=== ПРОГРАММА ДЛЯ ЗАМЕТОК ===\n\n";
    
    // Загрузка данных из файла
    if (manager.loadFromFile("notes.dat")) {
        std::cout << "Заметки загружены из файла\n";
    }
    
    bool running = true;
    while (running) {
        showMenu();
        int choice = getChoice(1, 9);
        
        switch (choice) {
            case 1: createNote(); break;
            case 2: viewAllNotes(); break;
            case 3: searchByTag(); break;
            case 4: searchByType(); break;
            case 5: editNote(); break;
            case 6: deleteNote(); break;
            case 7: showTags(); break;
            case 8: showStatistics(); break;
            case 9: running = false; break;
        }
    }
    
    // Сохранение данных в файл
    if (manager.saveToFile("notes.dat")) {
        std::cout << "Заметки сохранены в файл\n";
    }
    
    std::cout << "До свидания!\n";
}

void NotesApp::showMenu() {
    std::cout << "\n=== МЕНЮ ===\n";
    std::cout << "1. Создать заметку\n";
    std::cout << "2. Просмотреть все заметки\n";
    std::cout << "3. Поиск по тегу\n";
    std::cout << "4. Поиск по типу\n";
    std::cout << "5. Редактировать заметку\n";
    std::cout << "6. Удалить заметку\n";
    std::cout << "7. Показать все теги\n";
    std::cout << "8. Статистика\n";
    std::cout << "9. Выход\n";
    std::cout << "Выберите действие: ";
}

int NotesApp::getChoice(int min, int max) {
    int choice;
    while (true) {
        std::cin >> choice;
        if (std::cin.fail() || choice < min || choice > max) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Неверный ввод. Введите число от " << min << " до " << max << ": ";
        } else {
            std::cin.ignore(); // Очистка буфера
            break;
        }
    }
    return choice;
}

void NotesApp::createNote() {
    std::cout << "\n=== СОЗДАНИЕ ЗАМЕТКИ ===\n";
    
    std::cout << "Выберите тип заметки:\n";
    std::cout << "1. Текстовая\n";
    std::cout << "2. Графическая\n";
    std::cout << "3. Аудио\n";
    std::cout << "4. Видео\n";
    std::cout << "Ваш выбор: ";
    
    int typeChoice = getChoice(1, 4);
    
    std::string title;
    std::cout << "Введите заголовок заметки: ";
    std::getline(std::cin, title);
    
    std::unique_ptr<Note> note;
    
    switch (typeChoice) {
        case 1: {
            std::string text;
            std::cout << "Введите текст заметки: ";
            std::getline(std::cin, text);
            note = NoteFactory::createTextNote(title, text);
            break;
        }
        case 2: {
            std::string desc;
            std::cout << "Введите описание изображения: ";
            std::getline(std::cin, desc);
            
            std::string format;
            std::cout << "Введите формат (png/jpg/gif): ";
            std::getline(std::cin, format);
            
            int width, height;
            std::cout << "Введите ширину: ";
            std::cin >> width;
            std::cout << "Введите высоту: ";
            std::cin >> height;
            std::cin.ignore();
            
            note = NoteFactory::createImageNote(title, desc, format, width, height);
            break;
        }
        case 3: {
            std::string desc;
            std::cout << "Введите описание аудио: ";
            std::getline(std::cin, desc);
            
            double duration;
            std::cout << "Введите длительность (сек): ";
            std::cin >> duration;
            
            int sampleRate;
            std::cout << "Введите частоту дискретизации: ";
            std::cin >> sampleRate;
            std::cin.ignore();
            
            note = NoteFactory::createAudioNote(title, desc, duration, sampleRate);
            break;
        }
        case 4: {
            std::string desc;
            std::cout << "Введите описание видео: ";
            std::getline(std::cin, desc);
            
            double duration;
            std::cout << "Введите длительность (сек): ";
            std::cin >> duration;
            
            int width, height;
            std::cout << "Введите ширину: ";
            std::cin >> width;
            std::cout << "Введите высоту: ";
            std::cin >> height;
            std::cin.ignore();
            
            note = NoteFactory::createVideoNote(title, desc, duration, width, height);
            break;
        }
    }
    
    // Добавление тегов
    std::cout << "Хотите добавить теги? (y/n): ";
    char choice;
    std::cin >> choice;
    std::cin.ignore();
    
    if (choice == 'y' || choice == 'Y') {
        std::string tagsInput;
        std::cout << "Введите теги через запятую: ";
        std::getline(std::cin, tagsInput);
        
        std::stringstream ss(tagsInput);
        std::string tag;
        while (std::getline(ss, tag, ',')) {
            // Удаляем пробелы в начале и конце
            tag.erase(0, tag.find_first_not_of(" \t"));
            tag.erase(tag.find_last_not_of(" \t") + 1);
            if (!tag.empty()) {
                note->addTag(tag);
            }
        }
    }
    
    manager.addNote(std::move(note));
    std::cout << "Заметка создана успешно!\n";
}

void NotesApp::viewAllNotes() {
    std::cout << "\n=== ВСЕ ЗАМЕТКИ ===\n";
    auto notes = manager.getAllNotes();
    
    if (notes.empty()) {
        std::cout << "Заметок нет.\n";
        return;
    }
    
    for (size_t i = 0; i < notes.size(); ++i) {
        std::cout << "\n[" << (i + 1) << "] ";
        notes[i]->display();
    }
}

void NotesApp::searchByTag() {
    std::cout << "\n=== ПОИСК ПО ТЕГУ ===\n";
    
    std::cout << "Введите тег для поиска: ";
    std::string tag;
    std::getline(std::cin, tag);
    
    auto notes = manager.searchByTag(tag);
    
    if (notes.empty()) {
        std::cout << "Заметок с тегом '" << tag << "' не найдено.\n";
    } else {
        std::cout << "Найдено " << notes.size() << " заметок:\n";
        for (size_t i = 0; i < notes.size(); ++i) {
            std::cout << "\n[" << (i + 1) << "] ";
            notes[i]->display();
        }
    }
}

void NotesApp::searchByType() {
    std::cout << "\n=== ПОИСК ПО ТИПУ ===\n";
    
    std::cout << "Выберите тип:\n";
    std::cout << "1. Текст\n";
    std::cout << "2. Изображение\n";
    std::cout << "3. Аудио\n";
    std::cout << "4. Видео\n";
    std::cout << "Ваш выбор: ";
    
    int typeChoice = getChoice(1, 4);
    
    std::string type;
    switch (typeChoice) {
        case 1: type = "text"; break;
        case 2: type = "image"; break;
        case 3: type = "audio"; break;
        case 4: type = "video"; break;
    }
    
    auto notes = manager.searchByType(type);
    
    if (notes.empty()) {
        std::cout << "Заметок типа '" << type << "' не найдено.\n";
    } else {
        std::cout << "Найдено " << notes.size() << " заметок:\n";
        for (size_t i = 0; i < notes.size(); ++i) {
            std::cout << "\n[" << (i + 1) << "] ";
            notes[i]->display();
        }
    }
}

void NotesApp::editNote() {
    std::cout << "\n=== РЕДАКТИРОВАНИЕ ЗАМЕТКИ ===\n";
    
    viewAllNotes();
    auto notes = manager.getAllNotes();
    
    if (notes.empty()) return;
    
    std::cout << "\nВведите номер заметки для редактирования: ";
    int noteIndex = getChoice(1, notes.size()) - 1;
    
    Note* note = notes[noteIndex];
    
    bool editing = true;
    while (editing) {
        std::cout << "\nЧто вы хотите изменить?\n";
        std::cout << "1. Заголовок\n";
        std::cout << "2. Теги\n";
        std::cout << "3. Закончить редактирование\n";
        std::cout << "Ваш выбор: ";
        
        int editChoice = getChoice(1, 3);
        
        switch (editChoice) {
            case 1: {
                std::string newTitle;
                std::cout << "Введите новый заголовок: ";
                std::getline(std::cin, newTitle);
                note->setTitle(newTitle);
                std::cout << "Заголовок изменен.\n";
                break;
            }
            case 2: {
                std::cout << "Текущие теги: " << note->getTagsString() << "\n";
                std::cout << "1. Добавить тег\n";
                std::cout << "2. Удалить тег\n";
                std::cout << "3. Очистить все теги\n";
                std::cout << "Ваш выбор: ";
                
                int tagChoice = getChoice(1, 3);
                
                switch (tagChoice) {
                    case 1: {
                        std::string newTag;
                        std::cout << "Введите тег для добавления: ";
                        std::getline(std::cin, newTag);
                        note->addTag(newTag);
                        std::cout << "Тег добавлен.\n";
                        break;
                    }
                    case 2: {
                        std::string tagToRemove;
                        std::cout << "Введите тег для удаления: ";
                        std::getline(std::cin, tagToRemove);
                        note->removeTag(tagToRemove);
                        std::cout << "Тег удален.\n";
                        break;
                    }
                    case 3: {
                        note->clearTags();
                        std::cout << "Все теги удалены.\n";
                        break;
                    }
                }
                break;
            }
            case 3: {
                editing = false;
                break;
            }
        }
    }
}

void NotesApp::deleteNote() {
    std::cout << "\n=== УДАЛЕНИЕ ЗАМЕТКИ ===\n";
    
    viewAllNotes();
    auto notes = manager.getAllNotes();
    
    if (notes.empty()) return;
    
    std::cout << "\nВведите номер заметки для удаления: ";
    int noteIndex = getChoice(1, notes.size()) - 1;
    
    std::cout << "Вы уверены? (y/n): ";
    char confirm;
    std::cin >> confirm;
    std::cin.ignore();
    
    if (confirm == 'y' || confirm == 'Y') {
        if (manager.removeNote(notes[noteIndex]->getId())) {
            std::cout << "Заметка удалена.\n";
        } else {
            std::cout << "Ошибка при удалении заметки.\n";
        }
    }
}

void NotesApp::showTags() {
    std::cout << "\n=== ВСЕ ТЕГИ ===\n";
    auto tags = manager.getAllTags();
    
    if (tags.empty()) {
        std::cout << "Тегов нет.\n";
    } else {
        std::cout << "Всего тегов: " << tags.size() << "\n";
        for (const auto& tag : tags) {
            std::cout << "- " << tag << "\n";
        }
    }
}

void NotesApp::showStatistics() {
    manager.showStatistics();
}