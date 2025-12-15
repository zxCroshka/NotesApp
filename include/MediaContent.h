#ifndef MEDIACONTENT_H
#define MEDIACONTENT_H

#include <string>
#include <fstream>

class MediaContent {
public:
    virtual ~MediaContent() = default;
    virtual std::string getType() const = 0;
    virtual std::string getInfo() const = 0;
    virtual size_t getMemoryUsage() const = 0;
    virtual void save(std::ofstream& out) const = 0;
    virtual void load(std::ifstream& in) = 0;
};

#endif // MEDIACONTENT_H
