#include "common.h"
#include <amdev.h>
#include <stdio.h>

size_t serial_write(const void *buf, size_t offset, size_t len) {
  /* Start */

  len = len>0 ? len : 0;
  for(int i=0; i<len; i++) {
    _putc(((char *)buf)[i]);
  }
  return len;

  /* End */
}

#define NAME(key) \
  [_KEY_##key] = #key,

static const char *keyname[256] __attribute__((used)) = {
  [_KEY_NONE] = "NONE",
  _KEYS(NAME)
};

/* Start */

#define KEYDOWN_MASK 0x8000

/* End */

size_t events_read(void *buf, size_t offset, size_t len) {
  /* Start */
  
  int key = read_key();
  bool down = false;
  if( key & KEYDOWN_MASK ){
    key ^= 0x8000;
    down = true;
  }
  if(key){
    len = sprintf(buf,"%s %s\n", down ?"kd":"ku", keyname[key]);
  }
  else{
    len = sprintf(buf,"t %d\n", (int)uptime());
  }
  return len;

  /* End */
}

static char dispinfo[128] __attribute__((used)) = {};

size_t dispinfo_read(void *buf, size_t offset, size_t len) {
  /* Start */

  return sprintf(buf, dispinfo+offset);

  /* End */
}

size_t fb_write(const void *buf, size_t offset, size_t len) {
  /* Start */
  
  int x = (offset/4) % screen_width();
  int y = (offset/4) / screen_width();
  draw_rect((uint32_t*)buf, x, y, len / 4, 1);
  return len;

  /* End */
}

size_t fbsync_write(const void *buf, size_t offset, size_t len) {
  /* Start */
  
  draw_sync();
  return len;

  /* End */
}

void init_device() {
  Log("Initializing devices...");
  _ioe_init();

  // TODO: print the string to array `dispinfo` with the format
  // described in the Navy-apps convention
  /* Start */

  sprintf(dispinfo, "WIDTH:%d\nHEIGHT:%d\n", screen_width(), screen_height() );

  /* End */
}
