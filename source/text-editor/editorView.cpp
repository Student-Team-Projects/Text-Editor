#include <text-editor/editorView.hpp>

EditorView::EditorView(const TRect& bounds) noexcept : TSurfaceView(bounds){
    surface = &drawSurface;
    drawSurface.resize(bounds.b - bounds.a);
}

void EditorView::resize(const TRect& bounds) noexcept{
    drawSurface.resize(bounds.b - bounds.a);
    if(editor){
        editor->setSize(bounds.b - bounds.a);
    }
    TSurfaceView::setBounds(bounds);
    paint();
    draw();
}

void EditorView::paint(){
    if(editor){
        editor->paint(drawSurface, TRect({0, 0}, size));
    }
}

void EditorView::attachEditor(Editor* newEditor){
    if(editor){
        editor->removeObserver(this);
    }
    editor = newEditor;
    editor->setSize(size);
    editor->addObserver(this);
    paint();
    draw();
}

void EditorView::editorUpdate(){
    TCommandSet s;
    s.enableCmd(cmSave);
    enableCommands(s);
    paint();
    draw();
}