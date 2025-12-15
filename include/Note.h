#ifndef NOTE_H
#define NOTE_H

#include <iostream>
#include <string>
#include <memory>
#include <set>
#include <chrono>
#include <fstream>
#include <sstream>
#include <iomanip>

// Предварительное объявление MediaContent
class MediaContent;

class Note {
private:
    std::string id;
    std::string title;
    std::chrono::system_clock::time_point created;
    std::chrono::system_clock::time_point modified;
    std::set<std::string> tags;
    std::unique_ptr<MediaContent> content;
    
    // Приватные методы
    std::string generateId();
    void updateModified();
    
public:
    // Конструкторы
    Note();
    Note(const std::string& title, std::unique_ptr<MediaContent> content);
    
    // Геттеры
    const std::string& getId() const;
    const std::string& getTitle() const;
    std::string getCreatedTime() const;
    std::string getModifiedTime() const;
    const std::set<std::string>& getTags() const;
    std::string getTagsString() const;
    MediaContent* getContent();
    const MediaContent* getContent() const;
    
    // Сеттеры
    void setTitle(const std::string& newTitle);
    void setContent(std::unique_ptr<MediaContent> newContent);
    void addTag(const std::string& tag);
    void removeTag(const std::string& tag);
    void clearTags();
    
    // Методы отображения
    void display() const;
    
    // Методы сериализации
    void save(std::ofstream& out) const;
    void load(std::ifstream& in);
    
    // Деструктор
    ~Note() = default;
};

#endif // NOTE_H