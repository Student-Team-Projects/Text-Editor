#include <efsw/efsw.hpp>
#include <string>
#include <turbo-core/fileWatcher.hpp>

tc_file_watcher::tc_file_watcher() : m_watcher(new efsw::FileWatcher()) {
  m_watcher->watch();
}

auto tc_file_watcher::add_watcher(const std::string &path, const t_callback &callback)
    -> efsw::WatchID {
  auto *listener = new m_update_listener(callback);
  auto watch_id = m_watcher->addWatch(path, listener, /*recursive=*/false);
  m_listeners[watch_id] = listener;
  return watch_id;
}

auto tc_file_watcher::remove_watcher(const efsw::WatchID &watch_id) -> void {
  m_listeners.erase(watch_id);
}