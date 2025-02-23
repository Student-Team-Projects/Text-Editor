#pragma once

#define Uses_TRect
#define Uses_TApplication
#include <tvision/tv.h>
#include <tvterm/termctrl.h>
#include <tvterm/vtermemu.h>
#include <tvterm/termwnd.h>

const ushort cmCheckTerminalUpdates = 203;
const ushort cmTerminalUpdated      = 204;
const ushort cmGrabInput            = 205;
const ushort cmReleaseInput         = 206;
const ushort hcInputGrabbed         = 207;


void term_idle(TApplication *app);
TRect default_bounds(TRect deskTop_extent);

class t_terminal : public tvterm::BasicTerminalWindow {

public:
    explicit t_terminal(const TRect &bounds);
    void handleEvent(TEvent &ev) override;
protected:
    bool closing = false;
};

