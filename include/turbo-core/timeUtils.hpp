

#include <ctime>

namespace timeUtils {

// returns the current system time
time_t getSystemTime();

// returns the time at which the file under `path` was last modified
time_t getFileLastModifTime(char *path);

} // namespace timeUtils

