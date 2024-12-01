#include <filesystem>
#include <string_view>
#include <turbo/app.hpp>
#include <turbo/explorerWindow.hpp>
#include <tvision/tv.h>

t_explorer_outline::t_explorer_outline(const TRect &bounds, TScrollBar *hsb,
                                       TScrollBar *vsb, const std::string_view &path)
    : TOutline(bounds, hsb, vsb, nullptr) {
  root = get_root(path);
  update();
}

auto t_explorer_outline::adjust(TNode *node, bool expand) -> void {
  generate_children();
  TOutline::adjust(node, expand);
  update();
}
auto t_explorer_outline::selected(int idx) -> void {
  TOutline::selected(idx);
  auto *node = get_current_node();
  adjust(node, not node->expanded);
}
auto t_explorer_outline::get_root(const std::string_view &path) -> TNode * {
  TNode *root = nullptr;
  TNode *node = nullptr;
  for (const auto &entry : std::filesystem::directory_iterator(path)) {
    auto *tmp = new TNode(entry.path().filename().string());
    tmp->expanded = false;
    if (entry.is_directory()) {
      tmp->childList = tmp;
    }
    m_node_paths[tmp] = entry.path().string();
    if (root == nullptr) {
      root = tmp;
      node = root;
    } else {
      node->next = tmp;
      node = tmp;
    }
  }
  return root;
}
auto t_explorer_outline::get_current_node() -> TNode * { return getNode(foc); }
auto t_explorer_outline::generate_children() -> void {
  auto *node = get_current_node();
  if (m_node_paths.contains(node) and node->childList == node) {
    auto path = m_node_paths[node];
    node->childList = get_root(path);
  }
}

t_explorer_window::t_explorer_window(const TRect &bounds, const std::string_view &path)
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