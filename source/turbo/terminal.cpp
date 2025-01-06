#include <turbo/terminal.hpp>
#include <tvterm/termctrl.h>
#include <tvterm/vtermemu.h>
#include <tvterm/termwnd.h>
#include <tvterm/consts.h>
#include <turbo/app.hpp>

static void onTermError(const char *reason) {
    exit(1);
    //unimplemented!
}


const tvterm::TVTermConstants term_coms =
{
    cmCheckTerminalUpdates,
    cmTerminalUpdated,
    cmGrabInput,
    cmReleaseInput,
    7//hcInputGrabbed,
};

t_terminal::t_terminal(const TRect &bound, TApplication *app) {
    using namespace tvterm;
    VTermEmulatorFactory factory;
    auto *term_ctrl = TerminalController::create(BasicTerminalWindow::viewSize(bound),
                                                 factory, onTermError);

    t_term_wnd = new BasicTerminalWindow(bound, *term_ctrl, term_coms);
    //t_term_wnd -> flags &= ~wfZoom;
    //t_term_wnd -> growMode = gfGrowLoX | gfGrowHiX | gfGrowHiY;
    //t_term_wnd -> setState(sfShadow, /*enable=*/true);
    if (nullptr != term_ctrl) {
        app->insert(t_term_wnd);
        //t_term_wnd->execute();
    }
}

tvterm::BasicTerminalWindow t_terminal::new_term(const TRect &bounds) {

}
