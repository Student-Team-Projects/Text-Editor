#include <text-editor/scintillaEditorWindow.hpp>
#include <iostream>
#include <scintilla/src/EditModel.h>
#include "../text-editor-core/platform/surface.h"

//experimental class. Tried to figure out how things work - Kacper

TPalette ScintillaEditWindow::myPalette(cpBlueWindow, sizeof(cpBlueWindow) - 1);
ScintillaEditWindow::ScintillaEditWindow(const TRect &rect, TStringView aTitle, short aNumber)noexcept
    : TWindowInit(&TWindow::initFrame), TWindow(rect, aTitle, aNumber), scintilla(turbo::createScintilla()){
    // TColorAttr arr[256];
    // for(int i = 0; i < 256; i++)arr[i] = TColorAttr(i);
    // myPalette = TPalette(arr);
    TPoint point;
    point.x = size.x - 2;
    point.y = size.y - 2;
    surface = new TDrawSurface(point);
    setChar(surface->at(0, 0), 'A');
    setChar(surface->at(size.y-3, size.x-3), 'B');
    TColorAttr a = {'\x07', 0x7F00BB, slBold | slItalic};
    // setAttr(surface->at(1, 1), a);
    view = new ScintillaView(TRect(1, 1, size.x-1, size.y-1), surface);
    insert(view);

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

// TPalette& ScintillaEditWindow::getPalette() const{
//     std::cerr << "getPalette called \n";
//     return myPalette;
// }

void ScintillaEditWindow::handleEvent(TEvent& event){
    if(event.what != evBroadcast){
        std::cerr << "Event " << event.what << "\n";
    }
    if(event.what == evKeyboard){
        KeyDownEvent keyEvent = event.keyDown;
        // this is an alternative way to handle keyboard input. Probably worse
        // scintilla.InsertCharacter(std::string_view(keyEvent.text), Scintilla::EditModel::CharacterSource::directInput);
        turbo::handleKeyDown(scintilla, keyEvent); //seems better
    }else{
        // this doesnt seem to work. Probably mouse needs to be implemented manually.
        // turbo::handleMouse(scintilla, event.what, event.mouse);
    }
    TWindow::handleEvent(event);
}

TPoint ScintillaEditWindow::getEditorSize(){
    std::cerr << "getEditorSize\n";
    return view->size;
}

void ScintillaEditWindow::invalidate(TRect area){
    std::cerr << "invalidate " << area.a.x << " " << area.a.y << " " << area.b.x << " " << area.b.y << "\n";
    turbo::paint(scintilla, *surface, TRect(0, 0, view->size.x, view->size.y));
    // Scintilla::TScintillaSurface s;
    // s.surface = surface;
    // s.defaultTextAttr;
    // scintilla.Paint(&s, Scintilla::PRectangle::FromInts(0, 1, 20, 30));
    view->draw();
    // std::cerr << scintilla.
}

void ScintillaEditWindow::handleNotification(const SCNotification &scn){
    std::cerr << "handleNotification\n";
}

void ScintillaEditWindow::setVerticalScrollPos(int delta, int limit)noexcept{
    std::cerr << "setVerticalScrollPos " << delta << " " << limit << "\n";
}

void ScintillaEditWindow::setHorizontalScrollPos(int delta, int limit) noexcept{
    std::cerr << "setHorizontalScrollPos " << delta << " " << limit << "\n";
}