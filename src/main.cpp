#include "NotesApp.h"
#include <locale>

int main() {
    // Установка локали для русского языка
    std::locale::global(std::locale(""));
    
    NotesApp app;
    app.run();
    
    return 0;
}