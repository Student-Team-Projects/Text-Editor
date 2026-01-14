#pragma once
#include <text-editor/app.hpp>
#include <tvision/tv.h>
#include <scintilla.h>
#include <scintilla/tscintilla.h>
#include <set>
#include <string>

struct EditorObserver{
public:
    virtual void editorUpdate() = 0;
};

struct Editor : turbo::TScintillaParent{

public:
    Editor();
    void setSize(const TPoint& size);
    void paint(TDrawSurface& surface, TRect area);
    void handleEvent(TEvent &event);
    TPoint pointMainCaret();
    //observers
    void addObserver(EditorObserver* observer);
    void removeObserver(EditorObserver* observer);

    //interface TScintillaParent
    virtual TPoint getEditorSize() noexcept;
    virtual void invalidate(TRect area) noexcept;
    virtual void handleNotification(const SCNotification &scn);
    virtual void setVerticalScrollPos(int delta, int limit) noexcept;
    virtual void setHorizontalScrollPos(int delta, int limit) noexcept;

    //file manipulation
    void openFile(const std::string& path);
    void readFile();
    void saveFile();
    bool isModified();

private:
    void updateAll();
    void configureStyling(Scintilla::ILexer5* lexer);

private:
    std::string path;
    turbo::TScintilla scintilla;
    TPoint size = {0, 0};
    std::set<EditorObserver*> observers;
};