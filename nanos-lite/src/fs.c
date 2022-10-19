#include "fs.h"

typedef size_t (*ReadFn) (void *buf, size_t offset, size_t len);
typedef size_t (*WriteFn) (const void *buf, size_t offset, size_t len);

/* Start */

// in ramdisk.c
extern size_t ramdisk_read(void*, size_t, size_t);
extern size_t ramdisk_write(const void*, size_t, size_t);
// in device.c
size_t events_read(void *buf, size_t offset, size_t len);
size_t dispinfo_read(void *buf, size_t offset, size_t len);
size_t fb_write(const void *buf, size_t offset, size_t len);
size_t fbsync_write(const void *buf, size_t offset, size_t len);
size_t serial_write(const void *buf, size_t offser, size_t len);

/* End */

typedef struct {
  char *name;
  size_t size;
  size_t disk_offset;
  /* Start */

  size_t open_offset;

  /* End */
  ReadFn read;
  WriteFn write;
} Finfo;

enum {FD_STDIN, FD_STDOUT, FD_STDERR};

size_t invalid_read(void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

size_t invalid_write(const void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

/* This is the information about all files in disk. */
static Finfo file_table[] __attribute__((used)) = {
  /* Start */
  
  {"stdin"         , 0  , 0, 0, (ReadFn)invalid_read , (WriteFn)invalid_write},
  {"stdout"        , 0  , 0, 0, (ReadFn)invalid_read , (WriteFn)serial_write},
  {"stderr"        , 0  , 0, 0, (ReadFn)invalid_read , (WriteFn)serial_write},
  {"/dev/fb"       , 0  , 0, 0, (ReadFn)invalid_read , (WriteFn)fb_write     },
  {"/dev/events"   , 0  , 0, 0, (ReadFn)events_read  , (WriteFn)invalid_write},
  {"/dev/fbsync"   , 0  , 0, 0, (ReadFn)invalid_read , (WriteFn)fbsync_write },
  {"/proc/dispinfo", 128, 0, 0, (ReadFn)dispinfo_read, (WriteFn)invalid_write},
  {"/dev/tty"      , 0  , 0, 0, (ReadFn)invalid_read , (WriteFn)serial_write},

  /* End */
#include "files.h"
};

#define NR_FILES (sizeof(file_table) / sizeof(file_table[0]))

/* Start */

int fs_open(const char *pathname, int flags, int mode){
  for(int i=3; i < NR_FILES; i++){
    if(strcmp(pathname, file_table[i].name) == 0){
      return i;
    }
  }
  printf("%s open failed\n", pathname);
  assert(0);
  return -1;
}

size_t fs_read(int fd, void *buf, size_t len){
  if(file_table[fd].open_offset+len >= file_table[fd].size) {
    size_t value = file_table[fd].size - file_table[fd].open_offset;
    len = value > 0 ? value : 0;
  }
  if(file_table[fd].read == NULL) {
    len = ramdisk_read(buf, file_table[fd].disk_offset+file_table[fd].open_offset, len);
  }
  else{
    len = file_table[fd].read(buf, file_table[fd].disk_offset+file_table[fd].open_offset, len);
  }
  file_table[fd].open_offset += len;
  return len;
}

size_t fs_write(int fd, const void *buf, size_t len){

  if(fd == 0) {
    return 0;
  }

  if( fd==1 || fd==2 ){ // stdout stderr
    len = file_table[fd].write(buf, 0, len);
    return len;
  }

  if(file_table[fd].open_offset+len >= file_table[fd].size){
    size_t value  = file_table[fd].size - file_table[fd].open_offset;
    len = value > 0 ? value : 0;
  }
  
  if(file_table[fd].write == NULL){
    len = ramdisk_write(buf, file_table[fd].disk_offset + file_table[fd].open_offset, len);
  }
  else{
    len = file_table[fd].write(buf, file_table[fd].disk_offset + file_table[fd].open_offset, len);
  }
  file_table[fd].open_offset += len;
  return len;
}

size_t fs_lseek(int fd, size_t offset, int whence){
  switch(whence){
    case SEEK_SET: {
      file_table[fd].open_offset = offset;
      break;
    }  
    case SEEK_CUR: {
      file_table[fd].open_offset += offset;
      break;
    }
    case SEEK_END: {
      file_table[fd].open_offset = file_table[fd].size+offset;
      break;
    }
  }
  return file_table[fd].open_offset;
}

int fs_close(int fd){
  file_table[fd].open_offset = 0;
  return 0;
}

/* End */

void init_fs() {
  // TODO: initialize the size of /dev/fb
  /* Start */
  
  int fd = fs_open("/dev/fb", 0, 0);
  file_table[fd].size = screen_width() * screen_height() * 4;

  /* End */
}
