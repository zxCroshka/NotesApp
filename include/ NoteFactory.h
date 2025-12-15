#ifndef NOTEFACTORY_H
#define NOTEFACTORY_H

#include "Note.h"
#include "TextContent.h"
#include "ImageContent.h"
#include "AudioContent.h"
#include "VideoContent.h"
#include <memory>
#include <string>

class NoteFactory {
public:
    static std::unique_ptr<Note> createTextNote(const std::string& title, const std::string& text);
    static std::unique_ptr<Note> createImageNote(const std::string& title, const std::string& description,
                                                const std::string& format, int width, int height);
    static std::unique_ptr<Note> createAudioNote(const std::string& title, const std::string& description,
                                                double duration, int sampleRate);
    static std::unique_ptr<Note> createVideoNote(const std::string& title, const std::string& description,
                                                double duration, int width, int height);
};

#endif // NOTEFACTORY_H