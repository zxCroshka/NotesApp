#ifndef AUDIOCONTENT_H
#define AUDIOCONTENT_H

#include "MediaContent.h"
#include <string>
#include <fstream>

class AudioContent : public MediaContent {
private:
    std::string description;
    double duration;
    int sampleRate;
    
public:
    AudioContent();
    AudioContent(const std::string& desc, double duration, int sampleRate);
    
    std::string getType() const override;
    std::string getInfo() const override;
    size_t getMemoryUsage() const override;
    
    // Геттеры
    const std::string& getDescription() const;
    double getDuration() const;
    int getSampleRate() const;
    
    // Сеттеры
    void setDescription(const std::string& desc);
    void setDuration(double dur);
    void setSampleRate(int rate);
    
    void save(std::ofstream& out) const override;
    void load(std::ifstream& in) override;
};

#endif // AUDIOCONTENT_H