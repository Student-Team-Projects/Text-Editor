#pragma once
#include <text-editor/app.hpp>
#include <text-editor/editorView.hpp>
#include <text-editor/editor.hpp>

struct EditorWindow : TWindow{

public:
  EditorWindow(const TRect& bounds, TStringView aTitle, short aNumber)noexcept;

  void handleEvent(TEvent &event) override;
  
  void close() override;

  virtual void changeBounds(const TRect& bounds);

private:
  TRect viewBounds(const TRect& bounds);

public:
  time_t open_time;
private:
  EditorView& view;
  Editor editor; //potentially multiple editors in the future
};