#include <am.h>
#include <amdev.h>
#include <nemu.h>

#define KEYDOWN_MASK 0x8000

size_t __am_input_read(uintptr_t reg, void *buf, size_t size) {
  switch (reg) {
    case _DEVREG_INPUT_KBD: {
      _DEV_INPUT_KBD_t *kbd = (_DEV_INPUT_KBD_t *)buf;
      kbd->keydown = 0;
      kbd->keycode = _KEY_NONE;
      /* Start */

      // inl : nexus-am/am/include/riscv32.h (to load value of addr)
      // KBD_ADDR : nexus-am/am/include/nemu.h ( KBD_ADDR = 0xa1000060 )
      // KEYDOWN_MASK = 0x8000
      kbd->keycode = inl(KBD_ADDR);
      kbd->keydown = kbd->keycode&KEYDOWN_MASK ? 1 : 0;

      /* End */
      return sizeof(_DEV_INPUT_KBD_t);
    }
  }
  return 0;
}
