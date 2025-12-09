#include <text-editor/editor.hpp>
#include <iostream>
#include <fstream>

Editor::Editor(){
    scintilla.setParent(this);
    //configure margins
    turbo::call(scintilla, SCI_SETMARGINS, 0, 0);

    // Indentation
    turbo::call(scintilla, SCI_SETUSETABS, false, 0U);
    turbo::call(scintilla, SCI_SETINDENT, 4, 0U);
    turbo::call(scintilla, SCI_SETTABWIDTH, 4, 0U);
    turbo::call(scintilla, SCI_SETTABINDENTS, true, 0U);
    turbo::call(scintilla, SCI_SETBACKSPACEUNINDENTS, true, 0U);

    //Style configuration
    TColorAttr textColor = {0xFFFFFF, 0x000000};
    turbo::setStyleColor(scintilla, STYLE_DEFAULT, textColor);
    turbo::call(scintilla, SCI_STYLECLEARALL, 0, 0);

    TColorAttr selectionColor = {0xFFFFFF, 0x444466};
    turbo::setSelectionColor(scintilla, selectionColor);
}

void Editor::setSize(const TPoint& size){
    this->size = size;
}

void Editor::paint(TDrawSurface& surface, TRect area){
    turbo::paint(scintilla, surface, area);
}

void Editor::handleEvent(TEvent& event){
    if(event.what == evCommand){
        if(event.message.command == cmSave){
            std::cerr << "Save\n";
            saveFile();
        }
    }else if(event.what == evKeyboard){
        turbo::handleKeyDown(scintilla, event.keyDown);
    }
    //TODO: handle mouse events
}

TPoint Editor::pointMainCaret(){
    return turbo::pointMainCaret(scintilla);
}

void Editor::addObserver(EditorObserver* observer){
    observers.insert(observer);
}

void Editor::removeObserver(EditorObserver* observer){
    observers.erase(observer);
}

TPoint Editor::getEditorSize() noexcept{
    return size;
}

void Editor::invalidate(TRect area) noexcept{
    // may be useful later for understanding scintilla events
    // std::cerr << "invalidate " << area.a.x << " " << area.a.y << " " << area.b.x << " " << area.b.y << "\n";
    updateAll();
}

void Editor::handleNotification(const SCNotification &scn){
    // std::cerr << "handleNotification\n";
    // updateAll();
}

void Editor::setVerticalScrollPos(int delta, int limit) noexcept{
    // std::cerr << "setVerticalScrollPos " << delta << " " << limit << "\n";
    updateAll();
}

void Editor::setHorizontalScrollPos(int delta, int limit) noexcept{
    // std::cerr << "setHorizontalScrollPos " << delta << " " << limit << "\n";
    updateAll();
}

static char buffer alignas(4*1024) [128*1024 + 1024];
long buffSize = 128*1024;

void Editor::openFile(const std::string& path){
    this->path = path;
}

void Editor::readFile(){
    turbo::call(scintilla, SCI_CLEARALL, 0, 0);
    std::ifstream f(path, std::ios::in | std::ios::binary);
    //move input position indicator to end of file
    f.seekg(0, std::ios_base::end);
    long totalBytes = f.tellg(); 
    long allocBytes = totalBytes + 1000; //extra 1000 bytes like SciTE and turbo
    turbo::call(scintilla, SCI_ALLOCATE, allocBytes, 0);
    
    long remainingBytes = totalBytes;
    f.seekg(0, std::ios_base::beg);
    while(remainingBytes > 0){
        int readBytes = std::min(remainingBytes, buffSize); 
        f.read(buffer, readBytes);
        turbo::call(scintilla, SCI_APPENDTEXT, readBytes, (sptr_t)buffer);
        remainingBytes -= readBytes;
    }
    turbo::call(scintilla, SCI_SETSAVEPOINT, 0, 0);
    updateAll();
}

void Editor::saveFile(){
    std::ofstream f(path, std::ios::out | std::ios::binary);
    long totalBytes = turbo::call(scintilla, SCI_GETLENGTH, 0, 0);
    long pos = 0;
    while(pos < totalBytes){
        long endpos = std::min(totalBytes, pos + buffSize);
        Sci_TextRange range{Sci_CharacterRange{pos, endpos}, buffer};
        turbo::call(scintilla, SCI_GETTEXTRANGE, 0, (sptr_t)&range);
        long bytesRead = endpos - pos;
        f.write(buffer, bytesRead);
        pos += bytesRead;
    }
    turbo::call(scintilla, SCI_SETSAVEPOINT, 0, 0);
}

void Editor::updateAll(){
    for(auto observer : observers)observer->editorUpdate();
}