#include "internals.h"
using namespace Scintilla;
using namespace Scintilla::Internal;
#include <memory>

namespace Scintilla {
/////////////////////////////////////
/// FONT
std::shared_ptr<Font> Font::Allocate(const FontParameters &fp) {
  return std::shared_ptr<Font>();
}

/////////////////////////////////////
/// SURFACE
std::unique_ptr<Surface> Surface::Allocate(Scintilla::Technology technology) {
  return std::unique_ptr<Surface>();
}
void Surface::Init(WindowID wid) {}
void Surface::Init(SurfaceID mode_, WindowID wid_) {}
std::unique_ptr<Surface> Surface::AllocatePixMap(int width, int height) {
  return std::unique_ptr<Surface>();
}
void Surface::SetMode(SurfaceMode mode_) {}
void Surface::Release() noexcept {}
int Surface::SupportsFeature(Scintilla::Supports feature) noexcept {
  return 0;
}
bool Surface::Initialised() {
  return true;
}
int Surface::LogPixelsY() {
  return 0;
}
int Surface::PixelDivisions() {
  return 0;
}
int Surface::DeviceHeightFont(int points) {
  return 1;
}
void Surface::LineDraw(Point start, Point end, Stroke stroke) {}
void Surface::PolyLine(const Point *pts, size_t npts, Stroke stroke) {}
void Surface::Polygon(const Point *pts, size_t npts, FillStroke fillStroke) {}
void Surface::RectangleDraw(PRectangle rc, FillStroke fillStroke) {}
void Surface::RectangleFrame(PRectangle rc, Stroke stroke) {}
void Surface::FillRectangle(PRectangle rc, Fill fill) {}
void Surface::FillRectangleAligned(PRectangle rc, Fill fill) {}
void Surface::FillRectangle(PRectangle rc, Surface &surfacePattern) {}
void Surface::RoundedRectangle(PRectangle rc, FillStroke fillStroke) {}
void Surface::AlphaRectangle(PRectangle rc, XYPOSITION cornerSize, FillStroke fillStroke) {}
void Surface::GradientRectangle(PRectangle rc, const std::vector<ColourStop> &stops,
                                GradientOptions options) {}
void Surface::DrawRGBAImage(PRectangle rc, int width, int height,
                            const unsigned char *pixelsImage) {}
void Surface::Ellipse(PRectangle rc, FillStroke fillStroke) {}
void Surface::Stadium(PRectangle rc, FillStroke fillStroke, Surface::Ends ends) {}
void Surface::Copy(PRectangle rc, Point from, Surface &surfaceSource) {}
std::unique_ptr<IScreenLineLayout> Surface::Layout(const IScreenLine *screenLine) {
  return std::unique_ptr<IScreenLineLayout>();
}
void Surface::DrawTextNoClip(PRectangle rc, const Font *font_, XYPOSITION ybase,
                             std::string_view text, ColourRGBA fore, ColourRGBA back) {}
void Surface::DrawTextClipped(PRectangle rc, const Font *font_, XYPOSITION ybase,
                              std::string_view text, ColourRGBA fore, ColourRGBA back) {}
void Surface::DrawTextTransparent(PRectangle rc, const Font *font_, XYPOSITION ybase,
                                  std::string_view text, ColourRGBA fore) {}
void Surface::MeasureWidths(const Font *font_, std::string_view text,
                            XYPOSITION *positions) {}
XYPOSITION Surface::WidthText(const Font *font_, std::string_view text) {
  return 0;
}
void Surface::DrawTextNoClipUTF8(PRectangle rc, const Font *font_, XYPOSITION ybase,
                                 std::string_view text, ColourRGBA fore, ColourRGBA back) {}
void Surface::DrawTextClippedUTF8(PRectangle rc, const Font *font_, XYPOSITION ybase,
                                  std::string_view text, ColourRGBA fore, ColourRGBA back) {
}
void Surface::MeasureWidthsUTF8(const Font *font_, std::string_view text,
                                XYPOSITION *positions) {}
XYPOSITION Surface::WidthTextUTF8(const Font *font_, std::string_view text) {
  return 0;
}
XYPOSITION Surface::Ascent(const Font *font_) {
  return 0;
}
XYPOSITION Surface::Descent(const Font *font_) {
  return 0;
}
XYPOSITION Surface::InternalLeading(const Font *font_) {
  return 0;
}
XYPOSITION Surface::Height(const Font *font_) {
  return 0;
}
XYPOSITION Surface::AverageCharWidth(const Font *font_) {
  return 0;
}
void Surface::SetClip(PRectangle rc) {}
void Surface::PopClip() {}
void Surface::FlushCachedState() {}
void Surface::FlushDrawing() {}


/////////////////////////////////////
/// WINDOW
Window::~Window() noexcept {}
void Window::Destroy() noexcept {}
PRectangle Window::GetPosition() const {
  return PRectangle();
}
void Window::SetPosition(PRectangle rc) {}
void Window::SetPositionRelative(PRectangle rc, const Window *relativeTo) {}
PRectangle Window::GetClientPosition() const {
  return PRectangle();
}
void Window::Show(bool show) {}
void Window::InvalidateAll() {}
void Window::InvalidateRectangle(PRectangle rc) {}
void Window::SetCursor(Cursor curs) {}
PRectangle Window::GetMonitorRect(Point pt) {
  return PRectangle();
}

/////////////////////////////////////
/// LISTBOX

ListBox::ListBox() noexcept {}
ListBox::~ListBox() noexcept {}
std::unique_ptr<ListBox> ListBox::Allocate() {
  return std::unique_ptr<ListBox>();
}
void ListBox::SetFont(const Font *font) {}
void ListBox::Create(Window &parent, int ctrlID, Point location, int lineHeight,
                     bool unicodeMode, Technology technology) {}
void ListBox::SetAverageCharWidth(int width) {}
void ListBox::SetVisibleRows(int rows) {}
int ListBox::GetVisibleRows() const {
  return 0;
}
PRectangle ListBox::GetDesiredRect() {
  return PRectangle();
}
int ListBox::CaretFromEdge() {
  return 0;
}
void ListBox::Clear() noexcept {}
void ListBox::Append(char *s, int type) {}
int ListBox::Length() {
  return 0;
}
void ListBox::Select(int n) {}
int ListBox::GetSelection() {
  return 0;
}
int ListBox::Find(const char *prefix) {
  return 0;
}
std::string ListBox::GetValue(int n) {
  return std::string();
}
void ListBox::RegisterImage(int type, const char *xpmData) {}
void ListBox::RegisterRGBAImage(int type, int width, int height,
                                const unsigned char *pixelsImage) {}
void ListBox::ClearRegisteredImages() {}
void ListBox::SetDelegate(IListBoxDelegate *lbDelegate) {}
void ListBox::SetList(const char *list, char separator, char typesep) {}
void ListBox::SetOptions(ListOptions options_) {}


/////////////////////////////////////
/// MENU
Menu::Menu() noexcept {}
void Menu::CreatePopUp() {}
void Menu::Destroy() noexcept {}
void Menu::Show(Point pt, const Window &w) {}


/////////////////////////////////////
/// PLATFORM
ColourRGBA Internal::Platform::Chrome() {
  return ColourRGBA();
}
ColourRGBA Internal::Platform::ChromeHighlight() {
  return ColourRGBA();
}
const char *Internal::Platform::DefaultFont() {
  return "";
}
int Internal::Platform::DefaultFontSize() {
  return 1;
}
unsigned int Internal::Platform::DoubleClickTime() {
  return 300;
}
void Internal::Platform::DebugPrintf(const char *s, ...) noexcept {}

void Internal::Platform::Assert(const char *c, const char *file, int line) noexcept {}


} // namespace Scintilla
