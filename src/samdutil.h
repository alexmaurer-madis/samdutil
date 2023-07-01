#ifndef __SAMD_UTIL_H
#define __SAMD_UTIL_H

#include <string>

using std::string;

class SAMDUtil {
public:
  static void setupWatchdog(void);
  static void clearWatchdog(void);
  static uint8_t getResetCause(void);
  static string resetCauseToString(uint8_t reg);
};

#endif