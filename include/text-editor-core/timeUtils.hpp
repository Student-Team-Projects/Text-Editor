
// Copyright (c) 2025 Piotr Białek
// Copyright (c) 2025 Mateusz Rajs
// Copyright (c) 2025 Mikołaj Rams
// Copyright (c) 2025 Antoni Długosz
//
// Licensed under the MIT license

#include <ctime>

namespace timeUtils {

// returns the current system time
time_t getSystemTime();

// returns the time at which the file under `path` was last modified
time_t getFileLastModifTime(char *path);

} // namespace timeUtils

