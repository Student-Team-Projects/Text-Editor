#include <filesystem>
#include <turbo/explorerWindow.hpp>

t_explorer_outline::t_explorer_outline(const TRect &bounds, TScrollBar *hsb,
                                       TScrollBar *vsb, const std::string_view &path)
    : TOutline(bounds, hsb, vsb, get_root(path)) {}
auto t_explorer_outline::focused(int idx) -> void { foc = idx; }

auto t_explorer_outline::get_root(const std::string_view &path) -> TNode * {
  TNode *root = nullptr;
  TNode *node = nullptr;
  for (const auto &entry : std::filesystem::directory_iterator(path)) {
    auto *tmp = new TNode(entry.path().filename().string());
    // m_node_paths[tmp] = entry.path().string();
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
  auto path = m_node_paths[node];
  if (node->childList == nullptr and std::filesystem::is_directory(path)) {
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