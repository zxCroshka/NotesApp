#ifndef NOTESAPP_H
#define NOTESAPP_H

#include "NotesManager.h"
#include "NoteFactory.h"
#include <iostream>
#include <string>
#include <sstream>

class NotesApp {
private:
    NotesManager manager;
    
    void showMenu();
    int getChoice(int min, int max);
    void createNote();
    void viewAllNotes();
    void searchByTag();
    void searchByType();
    void editNote();
    void deleteNote();
    void showTags();
    void showStatistics();
    
public:
    void run();
};

#endif // NOTESAPP_H