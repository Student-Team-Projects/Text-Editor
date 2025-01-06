#pragma once

#define Uses_TRect
#define Uses_TApplication
#include <tvision/tv.h>
#include <tvterm/termctrl.h>
#include <tvterm/vtermemu.h>
#include <tvterm/termwnd.h>


class t_terminal {
    public:
    t_terminal(const TRect &bounds, TApplication *app);
    static tvterm::BasicTerminalWindow new_term(const TRect &bounds);
    
    private:
    tvterm::BasicTerminalWindow *t_term_wnd;
};
