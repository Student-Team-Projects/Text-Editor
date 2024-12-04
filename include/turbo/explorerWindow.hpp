#pragma once

#define Uses_TApplication
#define Uses_TOutline
#define Uses_TWindow
#include <map>
#include <turbo-core/fileWatcher.hpp>
#include <tvision/tv.h>

class t_explorer_outline : public TOutline {
  public:
  t_explorer_outline(const TRect &bounds, TScrollBar *hsb, TScrollBar *vsb,
                     const std::string &path);
  auto adjust(TNode *node, bool expand) -> void override;
  auto selected(int idx) -> void override;

  private:
  auto node_to_path(TNode *node) -> std::string;
  auto path_to_node(const std::string &path) -> TNode *;
  auto file_add(const std::string &dir, const std::string &filename) -> void;
  auto file_delete(const std::string &dir, const std::string &filename) -> void;
  auto file_move(const std::string &dir, const std::string &filename,
                 const std::string &old_filename) -> void;
  auto get_current_node() -> TNode *;
  auto populate_directory(const std::string &path) -> void;
  auto generate_children() -> void;
  std::map<TNode *, TNode *> m_parent_nodes;
  std::map<TNode *, size_t> m_watch_ids;
  std::string m_root_path;
  tc_file_watcher m_watcher;
};

class t_explorer_window : public TWindow {
  public:
  t_explorer_window(const TRect &bounds, const std::string &path);
  auto close() -> void override;
  auto toggle_tree() -> void;

  private:
  t_explorer_outline *m_outline;
  bool m_tree_visible = true;
};