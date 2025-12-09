#include <text-editor/editor.hpp>
#include <iostream>

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
}

void Editor::setSize(const TPoint& size){
    this->size = size;
}

void Editor::paint(TDrawSurface& surface, TRect area){
    turbo::paint(scintilla, surface, area);
}

void Editor::handleEvent(TEvent& event){
    if(event.what == evKeyboard){
        turbo::handleKeyDown(scintilla, event.keyDown);
    }
    //TODO: handle mouse events
}

void Editor::addObserver(EditorObserver* observer){
    observers.insert(observer);
}

void Editor::removeObserver(EditorObserver* observer){
    observers.erase(observer);
}

TPoint Editor::getEditorSize(){
    return size;
}

void Editor::invalidate(TRect area){
    // may be useful later for understanding scintilla events
    std::cerr << "invalidate " << area.a.x << " " << area.a.y << " " << area.b.x << " " << area.b.y << "\n";
    updateAll();
}

void Editor::handleNotification(const SCNotification &scn){
    std::cerr << "handleNotification\n";
    // updateAll();
}

void Editor::setVerticalScrollPos(int delta, int limit){
    std::cerr << "setVerticalScrollPos " << delta << " " << limit << "\n";
    updateAll();
}

void Editor::setHorizontalScrollPos(int delta, int limit){
    std::cerr << "setHorizontalScrollPos " << delta << " " << limit << "\n";
    updateAll();
}

void Editor::updateAll(){
    for(auto observer : observers)observer->editorUpdate();
}