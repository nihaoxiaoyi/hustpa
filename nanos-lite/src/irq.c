#include "common.h"

/* Start*/

_Context* do_syscall(_Context *c);

/* End */

static _Context* do_event(_Event e, _Context* c) {
  switch (e.event) {
    /* Start*/

    case _EVENT_YIELD: {
      Log("self trapping event");
      break;
      }
    case _EVENT_SYSCALL: {
      do_syscall(c);        
      break;
      }
      
    /* End */
    default: panic("Unhandled event ID = %d", e.event);
  }

  return NULL;
}

void init_irq(void) {
  Log("Initializing interrupt/exception handler...");
  _cte_init(do_event);
}
