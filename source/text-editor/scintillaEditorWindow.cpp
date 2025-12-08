#include <text-editor/scintillaEditorWindow.hpp>
#include <iostream>
#include <scintilla/src/EditModel.h>
#include "../text-editor-core/platform/surface.h"

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
    setChar(surface->at(1, 1), 'A');
    TColorAttr a = {'\x07', 0x7F00BB, slBold | slItalic};
    // setAttr(surface->at(1, 1), a);
    view = new ScintillaView(TRect(1, 1, size.x-1, size.y-1), surface);
    insert(view);

    scintilla.setParent(this);
    // scintilla.KeyDefault()
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
        // scintilla.KeyDefault(keyEvent.keyCode, 0);
        scintilla.InsertCharacter(std::string_view(keyEvent.text), Scintilla::EditModel::CharacterSource::directInput);
        // if(keyEvent.keyCode == kbRight)x++;
        // if(keyEvent.keyCode == kbLeft)x--;
        // if(keyEvent.keyCode == kbDown)y++;
        // if(keyEvent.keyCode == kbUp)y--;
        // if(keyEvent.keyCode == kbEnter){
        //     color++;
        //     color %= 256;
        // }
        // std::cerr << "Key " << keyEvent.keyCode << " " << keyEvent.text << "\n";
        // std::cerr << "x, y, color " << x << " " << y << " " << color << "\n";
        // this->writeChar(x, y, 'C', color, 1);
    }
    TWindow::handleEvent(event);
}

TPoint ScintillaEditWindow::getEditorSize(){
    std::cerr << "getEditorSize\n";
    return size;
}

void ScintillaEditWindow::invalidate(TRect area){
    std::cerr << "invalidate " << area.a.x << " " << area.a.y << " " << area.b.x << " " << area.b.y << "\n";
    Scintilla::TScintillaSurface s;
    s.surface = surface;
    s.defaultTextAttr;
    scintilla.Paint(&s, Scintilla::PRectangle::FromInts(0, 0, 1000, 1000));
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