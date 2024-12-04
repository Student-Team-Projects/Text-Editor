#include <debug.hpp>
#include <turbo-core/fileWatcher.hpp>

tc_file_watcher::tc_file_watcher(
    const std::function<void(efsw::WatchID, const std::string &, const std::string &,
                             efsw::Action, const std::string &)> &callback)
    : m_listener(new m_update_listener(callback)), m_watcher(new efsw::FileWatcher) {
  m_watcher->watch();
}

auto tc_file_watcher::add_watch(const std::string &path) -> efsw::WatchID {
  return m_watcher->addWatch(path, m_listener);
}

auto tc_file_watcher::remove_watch(const efsw::WatchID &watch_id) -> void {
  m_watcher->removeWatch(watch_id);
}