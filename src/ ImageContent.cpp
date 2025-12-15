#include "ImageContent.h"
#include <string>

ImageContent::ImageContent() 
    : description(""), format("png"), width(0), height(0) {}

ImageContent::ImageContent(const std::string& desc, const std::string& format, 
                         int width, int height)
    : description(desc), format(format), width(width), height(height) {}

std::string ImageContent::getType() const {
    return "image";
}

std::string ImageContent::getInfo() const {
    return "Изображение [" + format + "] " + 
           std::to_string(width) + "x" + std::to_string(height) + 
           ": " + description;
}

size_t ImageContent::getMemoryUsage() const {
    return sizeof(*this) + description.capacity() + format.capacity();
}

const std::string& ImageContent::getDescription() const {
    return description;
}

const std::string& ImageContent::getFormat() const {
    return format;
}

int ImageContent::getWidth() const {
    return width;
}

int ImageContent::getHeight() const {
    return height;
}

void ImageContent::setDescription(const std::string& desc) {
    description = desc;
}

void ImageContent::setFormat(const std::string& fmt) {
    format = fmt;
}

void ImageContent::setDimensions(int w, int h) {
    width = w;
    height = h;
}

void ImageContent::save(std::ofstream& out) const {
    // Сохраняем описание
    size_t descSize = description.size();
    out.write(reinterpret_cast<const char*>(&descSize), sizeof(descSize));
    out.write(description.c_str(), descSize);
    
    // Сохраняем формат
    size_t formatSize = format.size();
    out.write(reinterpret_cast<const char*>(&formatSize), sizeof(formatSize));
    out.write(format.c_str(), formatSize);
    
    // Сохраняем размеры
    out.write(reinterpret_cast<const char*>(&width), sizeof(width));
    out.write(reinterpret_cast<const char*>(&height), sizeof(height));
}

void ImageContent::load(std::ifstream& in) {
    // Загружаем описание
    size_t descSize;
    in.read(reinterpret_cast<char*>(&descSize), sizeof(descSize));
    description.resize(descSize);
    in.read(&description[0], descSize);
    
    // Загружаем формат
    size_t formatSize;
    in.read(reinterpret_cast<char*>(&formatSize), sizeof(formatSize));
    format.resize(formatSize);
    in.read(&format[0], formatSize);
    
    // Загружаем размеры
    in.read(reinterpret_cast<char*>(&width), sizeof(width));
    in.read(reinterpret_cast<char*>(&height), sizeof(height));
}