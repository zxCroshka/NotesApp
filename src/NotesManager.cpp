#include "NotesManager.h"
#include "Note.h"
#include "MediaContent.h"  // Добавьте этот include
#include "TextContent.h"   // И эти
#include "ImageContent.h"
#include "AudioContent.h"
#include "VideoContent.h"
#include <iostream>
#include <algorithm>
#include <fstream>

// Добавление заметки
void NotesManager::addNote(std::unique_ptr<Note> note) {
    const std::string& id = note->getId();
    notes[id] = std::move(note);
    updateIndexes(id);
}

// Получение заметки по ID
Note* NotesManager::getNote(const std::string& id) {
    auto it = notes.find(id);
    return it != notes.end() ? it->second.get() : nullptr;
}

// Удаление заметки
bool NotesManager::removeNote(const std::string& id) {
    auto it = notes.find(id);
    if (it == notes.end()) return false;
    
    removeFromIndexes(id, *it->second);
    notes.erase(it);
    return true;
}

// Поиск по тегу
std::vector<Note*> NotesManager::searchByTag(const std::string& tag) {
    std::vector<Note*> result;
    auto it = tagIndex.find(tag);
    if (it != tagIndex.end()) {
        for (const auto& id : it->second) {
            auto noteIt = notes.find(id);
            if (noteIt != notes.end()) {
                result.push_back(noteIt->second.get());
            }
        }
    }
    return result;
}

// Поиск по нескольким тегам
std::vector<Note*> NotesManager::searchByTags(const std::vector<std::string>& tags) {
    if (tags.empty()) return getAllNotes();
    
    std::unordered_set<std::string> resultIds;
    bool first = true;
    
    for (const auto& tag : tags) {
        auto it = tagIndex.find(tag);
        if (it == tagIndex.end()) return {};
        
        if (first) {
            resultIds = it->second;
            first = false;
        } else {
            // Пересечение множеств
            std::unordered_set<std::string> intersection;
            for (const auto& id : it->second) {
                if (resultIds.find(id) != resultIds.end()) {
                    intersection.insert(id);
                }
            }
            resultIds = std::move(intersection);
        }
        
        if (resultIds.empty()) break;
    }
    
    std::vector<Note*> result;
    for (const auto& id : resultIds) {
        auto noteIt = notes.find(id);
        if (noteIt != notes.end()) {
            result.push_back(noteIt->second.get());
        }
    }
    return result;
}

// Поиск по типу
std::vector<Note*> NotesManager::searchByType(const std::string& type) {
    std::vector<Note*> result;
    auto it = typeIndex.find(type);
    if (it != typeIndex.end()) {
        for (const auto& id : it->second) {
            auto noteIt = notes.find(id);
            if (noteIt != notes.end()) {
                result.push_back(noteIt->second.get());
            }
        }
    }
    return result;
}

// Получение всех заметок
std::vector<Note*> NotesManager::getAllNotes() {
    std::vector<Note*> result;
    for (const auto& pair : notes) {
        result.push_back(pair.second.get());
    }
    return result;
}

// Получение всех тегов
std::vector<std::string> NotesManager::getAllTags() {
    std::vector<std::string> tags;
    for (const auto& pair : tagIndex) {
        tags.push_back(pair.first);
    }
    return tags;
}

// Сохранение в файл
bool NotesManager::saveToFile(const std::string& filename) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) return false;
    
    size_t count = notes.size();
    file.write(reinterpret_cast<const char*>(&count), sizeof(count));
    
    for (const auto& pair : notes) {
        pair.second->save(file);
    }
    
    return true;
}

// Загрузка из файла
bool NotesManager::loadFromFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) return false;
    
    notes.clear();
    tagIndex.clear();
    typeIndex.clear();
    
    size_t count;
    file.read(reinterpret_cast<char*>(&count), sizeof(count));
    
    for (size_t i = 0; i < count; ++i) {
        auto note = std::make_unique<Note>();
        note->load(file);
        const std::string& id = note->getId();
        notes[id] = std::move(note);
        updateIndexes(id);
    }
    
    return true;
}

// Статистика
void NotesManager::showStatistics() const {
    std::cout << "\n=== Статистика ===\n";
    std::cout << "Всего заметок: " << notes.size() << "\n";
    std::cout << "Всего тегов: " << tagIndex.size() << "\n";
    
    std::unordered_map<std::string, int> typeStats;
    for (const auto& pair : typeIndex) {
        typeStats[pair.first] = pair.second.size();
    }
    
    std::cout << "По типам:\n";
    for (const auto& pair : typeStats) {
        std::cout << "  " << pair.first << ": " << pair.second << "\n";
    }
}

// Приватные методы
void NotesManager::updateIndexes(const std::string& id) {
    auto noteIt = notes.find(id);
    if (noteIt == notes.end()) return;
    
    Note* note = noteIt->second.get();
    
    // Обновляем индекс тегов
    for (const auto& tag : note->getTags()) {
        tagIndex[tag].insert(id);
    }
    
    // Обновляем индекс типов
    if (note->getContent()) {
        typeIndex[note->getContent()->getType()].insert(id);
    }
}

void NotesManager::removeFromIndexes(const std::string& id, const Note& note) {
    // Удаляем из индекса тегов
    for (const auto& tag : note.getTags()) {
        auto tagIt = tagIndex.find(tag);
        if (tagIt != tagIndex.end()) {
            tagIt->second.erase(id);
            if (tagIt->second.empty()) {
                tagIndex.erase(tagIt);
            }
        }
    }
    
    // Удаляем из индекса типов
    if (note.getContent()) {
        auto typeIt = typeIndex.find(note.getContent()->getType());
        if (typeIt != typeIndex.end()) {
            typeIt->second.erase(id);
            if (typeIt->second.empty()) {
                typeIndex.erase(typeIt);
            }
        }
    }
}