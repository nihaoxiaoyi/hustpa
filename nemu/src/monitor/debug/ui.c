#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

/***Mycode:External function***/
/*Start*/

// (1) CPU execution
void cpu_exec(uint64_t);

// (2) display ISA reg
void isa_reg_display();

// (3) print watchpoints
void print_wp();

// (4) Clear the NO-th Watchpoint
void free_wp(int NO);

// (5) Create watchpoint
WP* new_wp();

/*End*/

/* We use the `readline' library to provide more flexibility to read from stdin. */
static char* rl_gets() {
  static char *line_read = NULL;

  if (line_read) {
    free(line_read);
    line_read = NULL;
  }

  line_read = readline("(nemu) ");

  if (line_read && *line_read) {
    add_history(line_read);
  }

  return line_read;
}

static int cmd_c(char *args) {
  cpu_exec(-1);
  return 0;
}

static int cmd_q(char *args) {
  return -1;
}

static int cmd_help(char *args);

/* Mycode: Add more commands code*/
/*Start*/

/**
  * let programe execute N instructions and then stop
  * When N is not given, it defaults to 1
  * si [N]
  * eg: si 10
  */ 
static int cmd_si(char *args){
  int n = 1;  // defaults 1
  if(args!=NULL){
    n = atoi(args);
  }
  cpu_exec(n);
  return 0;
}

/**
  * 1、Print register status
  * 2、Print Watchpoint Information
  * info SUBCMD
  * eg: info r or info w
  */ 
  static int cmd_info(char *args){
    if(args!=NULL){
      switch(args[0]){
        case 'r':isa_reg_display();break;
        case 'w':print_wp();break;
        default:break;
      }
    }
    return 0;
  }


/*End*/

static struct {
  char *name;
  char *description;
  int (*handler) (char *);
} cmd_table [] = {
  { "help", "Display informations about all supported commands", cmd_help },
  { "c", "Continue the execution of the program", cmd_c },
  { "q", "Exit NEMU", cmd_q },

  /* TODO: Add more commands */
  /*Start*/
  { "si [N]", "Single step execution", cmd_si},
  { "info r||w", "Print details of registers or watchpoints", cmd_info},
  /*End*/
};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i;

  if (arg == NULL) {
    /* no argument given */
    for (i = 0; i < NR_CMD; i ++) {
      printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
    }
  }
  else {
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(arg, cmd_table[i].name) == 0) {
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        return 0;
      }
    }
    printf("Unknown command '%s'\n", arg);
  }
  return 0;
}

void ui_mainloop(int is_batch_mode) {
  if (is_batch_mode) {
    cmd_c(NULL);
    return;
  }

  for (char *str; (str = rl_gets()) != NULL; ) {
    char *str_end = str + strlen(str);

    /* extract the first token as the command */
    char *cmd = strtok(str, " ");
    if (cmd == NULL) { continue; }

    /* treat the remaining string as the arguments,
     * which may need further parsing
     */
    char *args = cmd + strlen(cmd) + 1;
    if (args >= str_end) {
      args = NULL;
    }

#ifdef HAS_IOE
    extern void sdl_clear_event_queue(void);
    sdl_clear_event_queue();
#endif

    int i;
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(cmd, cmd_table[i].name) == 0) {
        if (cmd_table[i].handler(args) < 0) { return; }
        break;
      }
    }

    if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
  }
}
