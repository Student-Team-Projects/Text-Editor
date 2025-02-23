
// Copyright (c) 2025 Piotr Białek
// Copyright (c) 2025 Mateusz Rajs
// Copyright (c) 2025 Mikołaj Rams
// Copyright (c) 2025 Antoni Długosz
//
// Licensed under the MIT license

#include <ctime>
#include <sys/stat.h>
#include <sys/types.h>
#include <text-editor-core/timeUtils.hpp>

time_t timeUtils::getSystemTime() { return time(0); }

time_t timeUtils::getFileLastModifTime(char *path) {
  struct stat attr;
  stat(path, &attr);
  return attr.st_mtime;
}

