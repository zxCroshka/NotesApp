#ifndef TEXTCONTENT_H
#define TEXTCONTENT_H

#include "MediaContent.h"
#include <string>
#include <fstream>

class TextContent : public MediaContent {
private:
    std::string text;
    
public:
    TextContent();
    TextContent(const std::string& text);
    
    std::string getType() const override;
    std::string getInfo() const override;
    size_t getMemoryUsage() const override;
    
    void setText(const std::string& newText);
    const std::string& getText() const;
    
    void save(std::ofstream& out) const override;
    void load(std::ifstream& in) override;
};

#endif // TEXTCONTENT_H