#include <debug.hpp>
#include <turbo-core/fileWatcher.hpp>

tc_file_watcher::tc_file_watcher(
    const std::function<void(efsw::WatchID, const std::string &, const std::string &,
                             efsw::Action, const std::string &)> &callback)
    : m_listener(new m_update_listener(callback)), m_watch_id(0) {}

auto tc_file_watcher::add_watch(const std::string &path) -> size_t {
  // return m_watcher->addWatch(path, m_listener);
  m_watchers[m_watch_id].addWatch(path, m_listener);
  m_watchers[m_watch_id].watch();
  return m_watch_id++;
}

auto tc_file_watcher::remove_watch(const size_t &watch_id) -> void {
  m_watchers.erase(watch_id);
}