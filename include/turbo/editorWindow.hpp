#include <turbo/app.hpp>
#include <tvision/tv.h>

struct EditWindow : TEditWindow {

  EditWindow(const TRect &rect, TStringView view, int val) noexcept;

  void handleEvent(TEvent &event) override;

  private:
  // last modification this editor made to the file
  time_t open_time;
};

