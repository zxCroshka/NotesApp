#ifndef VIDEOCONTENT_H
#define VIDEOCONTENT_H

#include "MediaContent.h"
#include <string>
#include <fstream>

class VideoContent : public MediaContent {
private:
    std::string description;
    double duration;
    int width;
    int height;
    
public:
    VideoContent();
    VideoContent(const std::string& desc, double duration, 
                 int width, int height);
    
    std::string getType() const override;
    std::string getInfo() const override;
    size_t getMemoryUsage() const override;
    
    // Геттеры
    const std::string& getDescription() const;
    double getDuration() const;
    int getWidth() const;
    int getHeight() const;
    
    // Сеттеры
    void setDescription(const std::string& desc);
    void setDuration(double dur);
    void setDimensions(int w, int h);
    
    void save(std::ofstream& out) const override;
    void load(std::ifstream& in) override;
};

#endif // VIDEOCONTENT_H