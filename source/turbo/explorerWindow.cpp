#include <algorithm>
#include <efsw/efsw.hpp>
#include <filesystem>
#include <string>
#include <turbo/app.hpp>
#include <turbo/explorerWindow.hpp>
#include <tvision/tv.h>
#include <tvision/util.h>
#include <vector>

t_explorer_outline::t_explorer_outline(const TRect &bounds, TScrollBar *hsb,
                                       TScrollBar *vsb, const std::string &path)
    : TOutline(bounds, hsb, vsb, nullptr), m_root_path(path),
      m_watcher([this](efsw::WatchID /*watchid*/, const std::string &dir,
                       const std::string &filename, efsw::Action action,
                       const std::string &old_filename) -> void {
        switch (action) {
        case efsw::Actions::Add:
          file_add(dir, filename);
          break;
        case efsw::Actions::Delete:
          file_delete(dir, filename);
          break;
        case efsw::Actions::Moved:
          file_move(dir, filename, old_filename);
          break;
        default:
          return;
        }

        update();
        drawView();
      }) {
  root = new TNode(path);
  populate_directory(path);
  root->expanded = true;
  m_parent_nodes[root] = nullptr;
  m_watch_ids[root] = m_watcher.add_watch(path);

  update();
}

auto t_explorer_outline::adjust(TNode *node, bool expand) -> void {
  generate_children();
  TOutline::adjust(node, expand);
  if (not node->expanded and node->childList == nullptr) {
    node->childList = node;
  }
  update();
}
auto t_explorer_outline::selected(int idx) -> void {
  TOutline::selected(idx);
  auto *node = get_current_node();
  adjust(node, not node->expanded);
}
auto t_explorer_outline::populate_directory(const std::string &path) -> void {
  std::vector<std::filesystem::directory_entry> entries;
  for (const auto &entry : std::filesystem::directory_iterator(path)) {
    entries.emplace_back(entry);
  }
  std::ranges::sort(entries, [](const auto &lhs, const auto &rhs) {
    return lhs.path().filename().string() < rhs.path().filename().string();
  });
  for (const auto &entry : entries) {
    file_add(path, entry.path().filename().string());
  }
}
auto t_explorer_outline::get_current_node() -> TNode * { return getNode(foc); }
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
auto t_explorer_outline::path_to_node(const std::string &path) -> TNode * {
  auto *node = root;
  auto work_path = path;
  work_path = work_path.substr(m_root_path.size());
  while (not empty(work_path)) {
    auto pos = work_path.find('/');
    auto part = work_path.substr(0, pos);
    work_path = work_path.substr(pos + 1);
    node = node->childList;
    while (node != nullptr and node->text != part) {
      node = node->next;
    }
    if (node == nullptr) {
      return node;
    }
  }
  return node;
}
auto t_explorer_outline::file_add(const std::string &dir, const std::string &filename)
    -> void {
  auto *dir_node = path_to_node(dir);
  if (dir_node == nullptr or path_to_node(dir + filename + '/') != nullptr) {
    return;
  }
  auto *node = new TNode(filename);
  node->expanded = false;
  if (std::filesystem::is_directory(dir + filename)) {
    node->childList = node;
  }
  m_parent_nodes[node] = dir_node;

  auto *tmp = dir_node->childList;
  if (tmp == dir_node or tmp == nullptr) {
    dir_node->childList = node;
  } else {
    while (tmp->next != nullptr && tmp->next->text < filename) {
      tmp = tmp->next;
    }
    node->next = tmp->next;
    tmp->next = node;
  }
}
auto t_explorer_outline::file_delete(const std::string &dir, const std::string &filename)
    -> void {
  auto *dir_node = path_to_node(dir);
  if (dir_node == nullptr) {
    return;
  }
  auto *my_node = path_to_node(dir + filename + '/');
  if (my_node == nullptr) {
    return;
  }
  while (my_node->childList != nullptr) {
    auto *child = my_node->childList;
    if (child == my_node) {
      break;
    }
    file_delete(dir + filename + '/', child->text);
  }
  auto *tmp = dir_node->childList;
  if (tmp == my_node) {
    dir_node->childList = my_node->next;
  } else {
    while (tmp->next != my_node) {
      tmp = tmp->next;
    }
    tmp->next = my_node->next;
  }

  if (m_watch_ids.contains(my_node)) {
    auto watch_id = m_watch_ids[my_node];
    m_watcher.remove_watch(watch_id);
    m_watch_ids.erase(my_node);
  }
  m_parent_nodes.erase(my_node);
  delete my_node;
}
auto t_explorer_outline::file_move(const std::string &dir, const std::string &filename,
                                   const std::string &old_filename) -> void {
  file_delete(dir, old_filename);
  file_add(dir, filename);
}
auto t_explorer_outline::generate_children() -> void {
  auto *node = get_current_node();
  if (node->childList == node) {
    auto path = node_to_path(node);
    node->childList = nullptr;
    populate_directory(path);
    auto watch_id = m_watcher.add_watch(path);
    m_watch_ids[node] = watch_id;
  }
}

t_explorer_window::t_explorer_window(const TRect &bounds, const std::string &path)
    : TWindowInit(&t_explorer_window::initFrame), TWindow(bounds, "Explorer", wnNoNumber) {
  auto *hsb = standardScrollBar(sbHorizontal);
  auto *vsb = standardScrollBar(sbVertical);
  m_outline = new t_explorer_outline(getExtent().grow(-1, -1), hsb, vsb, path);
  insert(m_outline);
}
auto t_explorer_window::close() -> void {
  message(t_hello_app::app, evCommand, cm_toggle_tree, nullptr);
}
auto t_explorer_window::toggle_tree() -> void {
  m_tree_visible = !m_tree_visible;
  if (m_tree_visible) {
    show();
  } else {
    hide();
  }
}
