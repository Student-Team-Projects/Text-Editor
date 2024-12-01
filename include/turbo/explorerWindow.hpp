#pragma once

#define Uses_TApplication
#define Uses_TOutline
#define Uses_TWindow
#include <map>
#include <tvision/tv.h>

class t_explorer_outline : public TOutline {
  public:
  t_explorer_outline(const TRect &bounds, TScrollBar *hsb, TScrollBar *vsb,
                     const std::string_view &path);
  auto focused(int idx) -> void override;

  private:
  auto get_current_node() -> TNode *;
  auto get_root(const std::string_view &path) -> TNode *;
  auto generate_children() -> void;
  std::map<TNode *, std::string> m_node_paths;
};

class t_explorer_window : public TWindow {
  public:
  t_explorer_window(const TRect &bounds, const std::string_view &path);

  private:
  t_explorer_outline *m_outline;
};