#pragma once

#include <cassert>
#include <efsw/efsw.hpp>
#include <functional>
#include <utility>

class tc_file_watcher {
  using t_callback =
      std::function<void(efsw::WatchID, const std::string &, const std::string &,
                         efsw::Action, const std::string &)>;

  public:
  explicit tc_file_watcher(const t_callback &callback);
  auto add_watch(const std::string &path) -> efsw::WatchID;
  auto remove_watch(const efsw::WatchID &watch_id) -> void;

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
  t_callback m_callback;
  m_update_listener *m_listener;
  efsw::FileWatcher *m_watcher;
};