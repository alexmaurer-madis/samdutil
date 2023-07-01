#include <Arduino.h>
#include <samdutil.h>

/**
 * @brief Setup watchdog for 1.024s timeout
 *
 */
void SAMDUtil::setupWatchdog(void) {
  // Set divisor=4 for generic Clock Generator ID 2
  REG_GCLK_GENDIV = GCLK_GENDIV_DIV(4) | GCLK_GENDIV_ID(2);

  while (GCLK->STATUS.bit.SYNCBUSY)
    ;

  // Enable generic Clock Generator Control ID 2 with clock source OSCULP32K
  // Divide selection 2^(GENDIV.DIV+1) = 2^(4+1) = 32
  // Duty cycle 50/50
  REG_GCLK_GENCTRL = GCLK_GENCTRL_DIVSEL | GCLK_GENCTRL_IDC |
                     GCLK_GENCTRL_GENEN | GCLK_GENCTRL_SRC_OSCULP32K |
                     GCLK_GENCTRL_ID(2);

  while (GCLK->STATUS.bit.SYNCBUSY)
    ;

  // Enable Generic Clock Control ID 2 to WDT
  REG_GCLK_CLKCTRL =
      GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK2 | GCLK_CLKCTRL_ID_WDT;

  while (GCLK->STATUS.bit.SYNCBUSY)
    ;

  // Configure watchdog timeout period of 1024 clock cycles
  // 1.024s
  REG_WDT_CONFIG = WDT_CONFIG_PER_1K;
  while (WDT->STATUS.bit.SYNCBUSY)
    ;

  // Enable watchdog
  REG_WDT_CTRL = WDT_CTRL_ENABLE;

  while (WDT->STATUS.bit.SYNCBUSY)
    ;
}

/**
 * @brief Clear the watchdog timer
 *
 */
void SAMDUtil::clearWatchdog(void) {
  if (!WDT->STATUS.bit.SYNCBUSY) // Check if the WDT registers are synchronized
  {
    REG_WDT_CLEAR = WDT_CLEAR_CLEAR_KEY; // Clear the watchdog timer
  }
}

uint8_t SAMDUtil::getResetCause(void) { return REG_PM_RCAUSE; }

string SAMDUtil::resetCauseToString(uint8_t reg) {
  if (reg & PM_RCAUSE_POR) {
    return string("Reset cause Power On Reset");
  } else if (reg & PM_RCAUSE_WDT) {
    return string("Reset cause watchdog");
  } else if (reg & PM_RCAUSE_SYST) {
    return string("Reset cause system");
  } else {
    return string("Reset cause : " + reg);
  }
}