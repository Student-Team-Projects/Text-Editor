#pragma once

#include <efsw/efsw.hpp>
#include <functional>
#include <map>
#include <utility>

class tc_file_watcher {
  using t_callback =
      std::function<void(efsw::WatchID, const std::string &, const std::string &,
                         efsw::Action, const std::string &)>;

  public:
  tc_file_watcher();
  auto add_watcher(const std::string &path, const t_callback &callback) -> efsw::WatchID;
  auto remove_watcher(const efsw::WatchID &watch_id) -> void;

  private:
  class m_update_listener : public efsw::FileWatchListener {
public:
    explicit m_update_listener(t_callback callback) : m_callback(std::move(callback)) {}
    void handleFileAction(efsw::WatchID watchid, const std::string &dir,
                          const std::string &filename, efsw::Action action,
                          std::string old_filename) override {
      m_callback(watchid, dir, filename, action, old_filename);
    }

private:
    t_callback m_callback;
  };
  efsw::FileWatcher *m_watcher;
  std::map<efsw::WatchID, m_update_listener *> m_listeners;
};