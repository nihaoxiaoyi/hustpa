#include <am.h>
#include <amdev.h>
#include <nemu.h>

/* my global variable*/
/* Start */

static uint32_t start_time;
static uint32_t cur_time;
/* End */

size_t __am_timer_read(uintptr_t reg, void *buf, size_t size) {
  switch (reg) {
    case _DEVREG_TIMER_UPTIME: {
      _DEV_TIMER_UPTIME_t *uptime = (_DEV_TIMER_UPTIME_t *)buf;
      uptime->hi = 0;
      uptime->lo = 0;
      /* Start */

      // cur_time = inl(RTC_ADDR);
      // uptime->lo = cur_time - start_time;

      /* End */
      return sizeof(_DEV_TIMER_UPTIME_t);
    }
    case _DEVREG_TIMER_DATE: {
      _DEV_TIMER_DATE_t *rtc = (_DEV_TIMER_DATE_t *)buf;
      rtc->second = 0;
      rtc->minute = 0;
      rtc->hour   = 0;
      rtc->day    = 0;
      rtc->month  = 0;
      rtc->year   = 2000;
      return sizeof(_DEV_TIMER_DATE_t);
    }
  }
  return 0;
}

void __am_timer_init() {
  /* Start */

  // inl : nexus-am/am/include/riscv32.h (to load value of addr)
  // RTC_ADDR : nexus-am/am/include/nemu.h ( RTC_ADDR = 0xa1000048 )
  // start_time = inl(RTC_ADDR);
  
  /* End */
}
