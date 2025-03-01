
// Copyright (c) 2025 Piotr Białek
// Copyright (c) 2025 Mateusz Rajs
// Copyright (c) 2025 Mikołaj Rams
// Copyright (c) 2025 Antoni Długosz
//
// Licensed under the MIT license

#include <algorithm>
#include <cassert>
#include <cstring>
#include <efsw/efsw.hpp>
#include <filesystem>
#include <functional>
#include <string>
#include <text-editor/app.hpp>
#include <text-editor/explorerWindow.hpp>
#include <tvision/tv.h>
#include <tvision/util.h>
#include <vector>

t_explorer_outline::t_explorer_outline(const TRect &bounds, TScrollBar *hsb,
                                       TScrollBar *vsb, const std::string &path,
                                       const std::function<void(std::string)> &on_open)
    : TOutline(bounds, hsb, vsb, nullptr), m_root_path(path), m_on_open(on_open) {
  root = new TNode(path);
  populate_directory(root);
  root->expanded = true;
  m_parent_nodes[root] = nullptr;
  set_watcher(root);
  update();
}

auto t_explorer_outline::adjust(TNode *node, bool expand) -> void {
  generate_children();
  TOutline::adjust(node, expand);
  if (not node->expanded and node->childList == nullptr and
      m_watch_ids.contains(node) /* is a directory */) {
    erase_watcher(node);
    node->childList = node;
  }
  update();
}
auto t_explorer_outline::selected(int idx) -> void {
  TOutline::selected(idx);
  auto *node = get_focused();
  adjust(node, not node->expanded);
  if (node->childList == nullptr) {
    m_on_open(node_to_path(node).substr(0, node_to_path(node).size() - 1));
  }
}
auto t_explorer_outline::set_watcher(TNode *node) -> void {
  assert(not m_watch_ids.contains(node));
  m_watch_ids[node] = m_watcher.add_watcher(node_to_path(node), get_watcher(node));
}
auto t_explorer_outline::erase_watcher(TNode *node) -> void {
  assert(m_watch_ids.contains(node));
  m_watcher.remove_watcher(m_watch_ids[node]);
  m_watch_ids.erase(node);
}
auto t_explorer_outline::get_watcher(TNode *node)
    -> std::function<void(efsw::WatchID, const std::string &, const std::string &,
                          efsw::Action, const std::string &)> {
  return [this, node](efsw::WatchID watch_id, const std::string &dir,
                      const std::string &filename, efsw::Action action,
                      const std::string &old_filename) {
    switch (action) {
    case efsw::Actions::Add:
      file_add(node, filename);
      break;
    case efsw::Actions::Delete:
      file_delete(node, filename);
      break;
    case efsw::Actions::Moved:
      file_move(node, filename, old_filename);
      break;
    default:
      break;
    }
    update();
    drawView();
  };
}
auto t_explorer_outline::populate_directory(TNode *node) -> void {
  auto path = node_to_path(node);
  std::vector<std::filesystem::directory_entry> entries;
  std::copy(begin(std::filesystem::directory_iterator(path)),
            end(std::filesystem::directory_iterator(path)), std::back_inserter(entries));
  std::ranges::sort(entries, [](const auto &lhs, const auto &rhs) {
    return lhs.path().filename().string() < rhs.path().filename().string();
  });
  for (const auto &entry : entries) {
    file_add(node, entry.path().filename().string());
  }
}
auto t_explorer_outline::get_focused() -> TNode * { return getNode(foc); }
auto t_explorer_outline::node_to_path(TNode *node) -> std::string {
  std::vector<std::string> path;
  while (node != nullptr) {
    path.emplace_back(node->text);
    node = m_parent_nodes[node];
  }
  std::ranges::reverse(path);
  std::string path_str;
  path[0].pop_back();
  for (const auto &part : path) {
    path_str += part + '/';
  }
  return path_str;
}
auto t_explorer_outline::insert_node(TNode *node, TNode *new_node) -> void {
  if (node->childList == nullptr or node->childList == node) {
    node->childList = new_node;
  } else {
    node = node->childList;
    while (node->next != nullptr and strcmp(node->next->text, new_node->text) < 0) {
      node = node->next;
    }
    new_node->next = node->next;
    node->next = new_node;
  }
}
auto t_explorer_outline::erase_node(TNode *node, const std::string &filename) -> TNode * {
  if (node->childList == nullptr or
      node->childList == node) { // can happen due to weird behaviour of moving
    return nullptr;
  }
  TNode *erase_node = nullptr;
  if (node->childList->text == filename) {
    erase_node = node->childList;
    node->childList = node->childList->next;
  } else {
    node = node->childList;
    while (node->next != nullptr and node->next->text != filename) {
      node = node->next;
    }
    if (node->next != nullptr) {
      erase_node = node->next;
      node->next = node->next->next;
    }
  }
  return erase_node;
}
auto t_explorer_outline::file_add(TNode *node, const std::string &filename) -> void {
  auto *new_node = new TNode(filename);
  new_node->expanded = false;
  if (std::filesystem::is_directory(node_to_path(node) + filename)) {
    new_node->childList = new_node;
  }
  m_parent_nodes[new_node] = node;
  insert_node(node, new_node);
}
auto t_explorer_outline::file_delete(TNode *node, const std::string &filename) -> void {
  auto *erased_node = erase_node(node, filename);
  if (erased_node == nullptr) { // can happen due to weird behaviour of moving
    return;
  }
  if (erased_node->childList != erased_node) {
    while (erased_node->childList != nullptr) {
      file_delete(erased_node, erased_node->childList->text);
    }
  }
  m_parent_nodes.erase(erased_node);
  if (m_watch_ids.contains(erased_node)) {
    erase_watcher(erased_node);
  }
  delete erased_node;
}
auto t_explorer_outline::file_move(TNode *node, const std::string &filename,
                                   const std::string &old_filename) -> void {
  if (old_filename == filename) {
    file_add(node, filename);
  } else {
    file_delete(node, old_filename);
    file_add(node, filename);
  }
}
auto t_explorer_outline::generate_children() -> void {
  auto *node = get_focused();
  if (node->childList == node) {
    node->childList = nullptr;
    populate_directory(node);
    set_watcher(node);
  }
}

t_explorer_window::t_explorer_window(const TRect &bounds, const std::string &path)
    : TWindowInit(&t_explorer_window::initFrame), TWindow(bounds, "Explorer", wnNoNumber) {
  auto *hsb = standardScrollBar(sbHorizontal);
  auto *vsb = standardScrollBar(sbVertical);
  m_outline = new t_explorer_outline(
      getExtent().grow(-1, -1), hsb, vsb, path,
      [this](std::string path) { message(App::app, evCommand, cm_open_guy, &path); });
  insert(m_outline);
}
auto t_explorer_window::close() -> void {
  message(App::app, evCommand, cm_toggle_tree, nullptr);
}
auto t_explorer_window::visible() -> bool { return m_tree_visible; }
auto t_explorer_window::toggle_tree() -> void {
  m_tree_visible = !m_tree_visible;
  if (m_tree_visible) {
    show();
  } else {
    hide();
  }
}
