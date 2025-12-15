#include "NoteFactory.h"

std::unique_ptr<Note> NoteFactory::createTextNote(const std::string& title, const std::string& text) {
    auto content = std::make_unique<TextContent>(text);
    return std::make_unique<Note>(title, std::move(content));
}

std::unique_ptr<Note> NoteFactory::createImageNote(const std::string& title, const std::string& description,
                                                  const std::string& format, int width, int height) {
    auto content = std::make_unique<ImageContent>(description, format, width, height);
    return std::make_unique<Note>(title, std::move(content));
}

std::unique_ptr<Note> NoteFactory::createAudioNote(const std::string& title, const std::string& description,
                                                  double duration, int sampleRate) {
    auto content = std::make_unique<AudioContent>(description, duration, sampleRate);
    return std::make_unique<Note>(title, std::move(content));
}

std::unique_ptr<Note> NoteFactory::createVideoNote(const std::string& title, const std::string& description,
                                                  double duration, int width, int height) {
    auto content = std::make_unique<VideoContent>(description, duration, width, height);
    return std::make_unique<Note>(title, std::move(content));
}