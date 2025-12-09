#include <text-editor/editorWindow.hpp>

EditorWindow::EditorWindow(const TRect& bounds, TStringView aTitle, short aNumber) noexcept
    : TWindowInit(&EditorWindow::initFrame), TWindow(bounds, aTitle, aNumber), view(viewBounds(bounds)){
    insert(&view);
    view.attachEditor(&editor);
    editor.openFile(aTitle);
    editor.readFile();
}

void EditorWindow::handleEvent(TEvent &event){
    editor.handleEvent(event);
    TWindow::handleEvent(event);
}

void EditorWindow::changeBounds(const TRect& bounds){
    view.resize(viewBounds(bounds));
    TWindow::changeBounds(bounds);
}

TRect EditorWindow::viewBounds(const TRect& bounds){
    return TRect(TPoint{1, 1}, bounds.b - bounds.a - TPoint{1, 1});
}