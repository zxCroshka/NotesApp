#include "VideoContent.h"
#include <string>

VideoContent::VideoContent() 
    : description(""), duration(0.0), width(0), height(0) {}

VideoContent::VideoContent(const std::string& desc, double duration, 
                         int width, int height)
    : description(desc), duration(duration), width(width), height(height) {}

std::string VideoContent::getType() const {
    return "video";
}

std::string VideoContent::getInfo() const {
    return "Видео [" + std::to_string(duration) + " сек, " + 
           std::to_string(width) + "x" + std::to_string(height) + "]: " + description;
}

size_t VideoContent::getMemoryUsage() const {
    return sizeof(*this) + description.capacity();
}

const std::string& VideoContent::getDescription() const {
    return description;
}

double VideoContent::getDuration() const {
    return duration;
}

int VideoContent::getWidth() const {
    return width;
}

int VideoContent::getHeight() const {
    return height;
}

void VideoContent::setDescription(const std::string& desc) {
    description = desc;
}

void VideoContent::setDuration(double dur) {
    duration = dur;
}

void VideoContent::setDimensions(int w, int h) {
    width = w;
    height = h;
}

void VideoContent::save(std::ofstream& out) const {
    size_t descSize = description.size();
    out.write(reinterpret_cast<const char*>(&descSize), sizeof(descSize));
    out.write(description.c_str(), descSize);
    
    out.write(reinterpret_cast<const char*>(&duration), sizeof(duration));
    out.write(reinterpret_cast<const char*>(&width), sizeof(width));
    out.write(reinterpret_cast<const char*>(&height), sizeof(height));
}

void VideoContent::load(std::ifstream& in) {
    size_t descSize;
    in.read(reinterpret_cast<char*>(&descSize), sizeof(descSize));
    description.resize(descSize);
    in.read(&description[0], descSize);
    
    in.read(reinterpret_cast<char*>(&duration), sizeof(duration));
    in.read(reinterpret_cast<char*>(&width), sizeof(width));
    in.read(reinterpret_cast<char*>(&height), sizeof(height));
}