#ifndef IMAGECONTENT_H
#define IMAGECONTENT_H

#include "MediaContent.h"
#include <string>
#include <fstream>

class ImageContent : public MediaContent {
private:
    std::string description;
    std::string format;
    int width;
    int height;
    
public:
    ImageContent();
    ImageContent(const std::string& desc, const std::string& format, 
                 int width, int height);
    
    std::string getType() const override;
    std::string getInfo() const override;
    size_t getMemoryUsage() const override;
    
    // Геттеры
    const std::string& getDescription() const;
    const std::string& getFormat() const;
    int getWidth() const;
    int getHeight() const;
    
    // Сеттеры
    void setDescription(const std::string& desc);
    void setFormat(const std::string& fmt);
    void setDimensions(int w, int h);
    
    void save(std::ofstream& out) const override;
    void load(std::ifstream& in) override;
};

#endif // IMAGECONTENT_H