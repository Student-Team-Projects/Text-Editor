#include <text-editor/ScintillaView.hpp>
#include <iostream>

ScintillaView::ScintillaView(const TRect &bounds, const TDrawSurface _FAR *aSurface) :
    TSurfaceView(bounds, aSurface) {

}

void ScintillaView::handleEvent(TEvent &event){
    // if(event.what != evBroadcast){
    //     std::cerr << "Event " << event.what << "\n";
    // }
    // if(event.what == evKeyboard){
    //     KeyDownEvent keyEvent = event.keyDown;
    //     if(keyEvent.keyCode == kbRight)x++;
    //     if(keyEvent.keyCode == kbLeft)x--;
    //     if(keyEvent.keyCode == kbDown)y++;
    //     if(keyEvent.keyCode == kbUp)y--;
    //     if(keyEvent.keyCode == kbEnter){
    //         color++;
    //         color %= 256;
    //     }
    //     std::cerr << "Key " << keyEvent.keyCode << " " << keyEvent.text << "\n";
    //     std::cerr << "x, y, color " << x << " " << y << " " << color << "\n";
    //     this->writeChar(x, y, 'C', color, 1);
    // }
    TSurfaceView::handleEvent(event);
}