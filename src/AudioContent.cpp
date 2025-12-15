#include "AudioContent.h"
#include <string>

AudioContent::AudioContent() 
    : description(""), duration(0.0), sampleRate(44100) {}

AudioContent::AudioContent(const std::string& desc, double duration, int sampleRate)
    : description(desc), duration(duration), sampleRate(sampleRate) {}

std::string AudioContent::getType() const {
    return "audio";
}

std::string AudioContent::getInfo() const {
    return "Аудио [" + std::to_string(duration) + " сек, " + 
           std::to_string(sampleRate) + " Гц]: " + description;
}

size_t AudioContent::getMemoryUsage() const {
    return sizeof(*this) + description.capacity();
}

const std::string& AudioContent::getDescription() const {
    return description;
}

double AudioContent::getDuration() const {
    return duration;
}

int AudioContent::getSampleRate() const {
    return sampleRate;
}

void AudioContent::setDescription(const std::string& desc) {
    description = desc;
}

void AudioContent::setDuration(double dur) {
    duration = dur;
}

void AudioContent::setSampleRate(int rate) {
    sampleRate = rate;
}

void AudioContent::save(std::ofstream& out) const {
    size_t descSize = description.size();
    out.write(reinterpret_cast<const char*>(&descSize), sizeof(descSize));
    out.write(description.c_str(), descSize);
    
    out.write(reinterpret_cast<const char*>(&duration), sizeof(duration));
    out.write(reinterpret_cast<const char*>(&sampleRate), sizeof(sampleRate));
}

void AudioContent::load(std::ifstream& in) {
    size_t descSize;
    in.read(reinterpret_cast<char*>(&descSize), sizeof(descSize));
    description.resize(descSize);
    in.read(&description[0], descSize);
    
    in.read(reinterpret_cast<char*>(&duration), sizeof(duration));
    in.read(reinterpret_cast<char*>(&sampleRate), sizeof(sampleRate));
}