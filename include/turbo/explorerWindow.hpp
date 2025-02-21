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
  auto change_dir(const std::string &path) -> void;

  private:
  auto get_watcher(TNode *node)
      -> std::function<void(efsw::WatchID, const std::string &, const std::string &,
                            efsw::Action, const std::string &)>;
  auto set_watcher(TNode *node) -> void;
  auto erase_watcher(TNode *node) -> void;
  auto node_to_path(TNode *node) -> std::string;
  static auto insert_node(TNode *node, TNode *new_node) -> void;
  static auto erase_node(TNode *node, const std::string &filename) -> TNode *;
  auto file_add(TNode *node, const std::string &filename) -> void;
  auto file_delete(TNode *node, const std::string &filename) -> void;
  auto file_move(TNode *node, const std::string &filename, const std::string &old_filename)
      -> void;
  auto get_focused() -> TNode *;
  auto populate_directory(TNode *node) -> void;
  auto generate_children() -> void;
  std::map<TNode *, TNode *> m_parent_nodes;
  std::map<TNode *, efsw::WatchID> m_watch_ids;
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