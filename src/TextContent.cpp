#include "TextContent.h"
#include <string>

TextContent::TextContent() : text("") {}

TextContent::TextContent(const std::string& text) : text(text) {}

std::string TextContent::getType() const {
    return "text";
}

std::string TextContent::getInfo() const {
    std::string info = "Текст (" + std::to_string(text.length()) + " символов): ";
    if (text.length() > 50) {
        return info + text.substr(0, 47) + "...";
    }
    return info + text;
}

size_t TextContent::getMemoryUsage() const {
    return sizeof(*this) + text.capacity();
}

void TextContent::setText(const std::string& newText) {
    text = newText;
}

const std::string& TextContent::getText() const {
    return text;
}

void TextContent::save(std::ofstream& out) const {
    size_t size = text.size();
    out.write(reinterpret_cast<const char*>(&size), sizeof(size));
    out.write(text.c_str(), size);
}

void TextContent::load(std::ifstream& in) {
    size_t size;
    in.read(reinterpret_cast<char*>(&size), sizeof(size));
    text.resize(size);
    in.read(&text[0], size);
}