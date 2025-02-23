
// Copyright (c) 2025 Piotr Białek
// Copyright (c) 2025 Mateusz Rajs
// Copyright (c) 2025 Mikołaj Rams
// Copyright (c) 2025 Antoni Długosz
//
// Licensed under the MIT license

#define Uses_TDialog
#include <tvision/tv.h>

namespace dialogs {

// Helper function to show a `dialog` to the screen
ushort execDialog(TDialog *d, void *data);

// Show a dialog referenced with `dialog` and return the dialog result
auto spawnDialog(int dialog, ...) -> ushort;

} // namespace dialogs

