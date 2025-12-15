#include "Note.h"
#include "MediaContent.h"
#include "TextContent.h"    
#include "ImageContent.h"     
#include "AudioContent.h"   
#include "VideoContent.h"   
#include <iostream>
#include <sstream>
#include <iomanip>
#include <chrono>
// Конструкторы
Note::Note() {
    id = generateId();
    created = modified = std::chrono::system_clock::now();
}

Note::Note(const std::string& title, std::unique_ptr<MediaContent> content)
    : title(title), content(std::move(content)) {
    id = generateId();
    created = modified = std::chrono::system_clock::now();
}

// Геттеры
const std::string& Note::getId() const {
    return id;
}

const std::string& Note::getTitle() const {
    return title;
}

std::string Note::getCreatedTime() const {
    auto time = std::chrono::system_clock::to_time_t(created);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

std::string Note::getModifiedTime() const {
    auto time = std::chrono::system_clock::to_time_t(modified);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

const std::set<std::string>& Note::getTags() const {
    return tags;
}

std::string Note::getTagsString() const {
    std::string result;
    for (const auto& tag : tags) {
        if (!result.empty()) {
            result += ", ";
        }
        result += tag;
    }
    return result;
}

MediaContent* Note::getContent() {
    return content.get();
}

const MediaContent* Note::getContent() const {
    return content.get();
}

// Сеттеры
void Note::setTitle(const std::string& newTitle) {
    title = newTitle;
    updateModified();
}

void Note::setContent(std::unique_ptr<MediaContent> newContent) {
    content = std::move(newContent);
    updateModified();
}

void Note::addTag(const std::string& tag) {
    tags.insert(tag);
    updateModified();
}

void Note::removeTag(const std::string& tag) {
    tags.erase(tag);
    updateModified();
}

void Note::clearTags() {
    tags.clear();
    updateModified();
}

// Метод отображения
void Note::display() const {
    std::cout << "========================================\n";
    std::cout << "ID: " << id << "\n";
    std::cout << "Заголовок: " << title << "\n";
    std::cout << "Создано: " << getCreatedTime() << "\n";
    std::cout << "Изменено: " << getModifiedTime() << "\n";
    std::cout << "Теги: " << getTagsString() << "\n";
    
    if (content) {
        std::cout << "Тип: " << content->getType() << "\n";
        std::cout << "Содержимое: " << content->getInfo() << "\n";
    }
    
    std::cout << "========================================\n";
}

// Методы сериализации
void Note::save(std::ofstream& out) const {
    // Сохраняем базовые поля
    size_t idSize = id.size();
    out.write(reinterpret_cast<const char*>(&idSize), sizeof(idSize));
    out.write(id.c_str(), idSize);
    
    size_t titleSize = title.size();
    out.write(reinterpret_cast<const char*>(&titleSize), sizeof(titleSize));
    out.write(title.c_str(), titleSize);
    
    out.write(reinterpret_cast<const char*>(&created), sizeof(created));
    out.write(reinterpret_cast<const char*>(&modified), sizeof(modified));
    
    // Сохраняем теги
    size_t tagsSize = tags.size();
    out.write(reinterpret_cast<const char*>(&tagsSize), sizeof(tagsSize));
    
    for (const auto& tag : tags) {
        size_t tagSize = tag.size();
        out.write(reinterpret_cast<const char*>(&tagSize), sizeof(tagSize));
        out.write(tag.c_str(), tagSize);
    }
    
    // Сохраняем тип контента и сам контент
    if (content) {
        std::string type = content->getType();
        size_t typeSize = type.size();
        out.write(reinterpret_cast<const char*>(&typeSize), sizeof(typeSize));
        out.write(type.c_str(), typeSize);
        
        content->save(out);
    } else {
        size_t typeSize = 0;
        out.write(reinterpret_cast<const char*>(&typeSize), sizeof(typeSize));
    }
}

void Note::load(std::ifstream& in) {
    // Загружаем базовые поля
    size_t idSize;
    in.read(reinterpret_cast<char*>(&idSize), sizeof(idSize));
    id.resize(idSize);
    in.read(&id[0], idSize);
    
    size_t titleSize;
    in.read(reinterpret_cast<char*>(&titleSize), sizeof(titleSize));
    title.resize(titleSize);
    in.read(&title[0], titleSize);
    
    in.read(reinterpret_cast<char*>(&created), sizeof(created));
    in.read(reinterpret_cast<char*>(&modified), sizeof(modified));
    
    // Загружаем теги
    size_t tagsSize;
    in.read(reinterpret_cast<char*>(&tagsSize), sizeof(tagsSize));
    tags.clear();
    
    for (size_t i = 0; i < tagsSize; ++i) {
        size_t tagSize;
        in.read(reinterpret_cast<char*>(&tagSize), sizeof(tagSize));
        std::string tag;
        tag.resize(tagSize);
        in.read(&tag[0], tagSize);
        tags.insert(tag);
    }
    
    // Загружаем контент
    size_t typeSize;
    in.read(reinterpret_cast<char*>(&typeSize), sizeof(typeSize));
    
    if (typeSize > 0) {
        std::string type;
        type.resize(typeSize);
        in.read(&type[0], typeSize);
        
        // Создаем соответствующий тип контента
        if (type == "text") {
            content = std::make_unique<TextContent>();
        } else if (type == "image") {
            content = std::make_unique<ImageContent>();
        } else if (type == "audio") {
            content = std::make_unique<AudioContent>();
        } else if (type == "video") {
            content = std::make_unique<VideoContent>();
        }
        
        if (content) {
            content->load(in);
        }
    }
}

// Приватные методы
std::string Note::generateId() {
    static int counter = 0;
    auto now = std::chrono::system_clock::now();
    auto timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()).count();
    return "note_" + std::to_string(++counter) + "_" + std::to_string(timestamp);
}

void Note::updateModified() {
    modified = std::chrono::system_clock::now();
}