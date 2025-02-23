#include <ctime>
#include <sys/stat.h>
#include <sys/types.h>
#include <turbo-core/timeUtils.hpp>

time_t timeUtils::getSystemTime() { return time(0); }

time_t timeUtils::getFileLastModifTime(char *path) {
  struct stat attr;
  stat(path, &attr);
  return attr.st_mtime;
}

