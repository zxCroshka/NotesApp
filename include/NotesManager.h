#ifndef NOTESMANAGER_H
#define NOTESMANAGER_H

#include "Note.h"
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <fstream>

class NotesManager {
private:
    std::unordered_map<std::string, std::unique_ptr<Note>> notes;
    std::unordered_map<std::string, std::unordered_set<std::string>> tagIndex;
    std::unordered_map<std::string, std::unordered_set<std::string>> typeIndex;
    
    void updateIndexes(const std::string& id);
    void removeFromIndexes(const std::string& id, const Note& note);
    
public:
    // Добавление заметки
    void addNote(std::unique_ptr<Note> note);
    
    // Получение заметки по ID
    Note* getNote(const std::string& id);
    
    // Удаление заметки
    bool removeNote(const std::string& id);
    
    // Поиск по тегу
    std::vector<Note*> searchByTag(const std::string& tag);
    
    // Поиск по нескольким тегам
    std::vector<Note*> searchByTags(const std::vector<std::string>& tags);
    
    // Поиск по типу
    std::vector<Note*> searchByType(const std::string& type);
    
    // Получение всех заметок
    std::vector<Note*> getAllNotes();
    
    // Получение всех тегов
    std::vector<std::string> getAllTags();
    
    // Сохранение в файл
    bool saveToFile(const std::string& filename);
    
    // Загрузка из файла
    bool loadFromFile(const std::string& filename);
    
    // Статистика
    void showStatistics() const;
};

#endif // NOTESMANAGER_H