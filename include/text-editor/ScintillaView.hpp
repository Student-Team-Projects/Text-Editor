#define Uses_TSurfaceView

#include <text-editor/app.hpp>
#include <tvision/tv.h>
#include <tvision/surface.h>
#include <tvision/views.h>

struct ScintillaView : TSurfaceView
{
    ScintillaView(const TRect &bounds, const TDrawSurface _FAR *aSurface=0)noexcept;

    void handleEvent(TEvent &event)override;

    public:
    int x = 0;
    int y = 0;
    ushort color = 0;
};
