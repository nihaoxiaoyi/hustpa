#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 32

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = &wp_pool[i + 1];
  }
  wp_pool[NR_WP - 1].next = NULL;

  head = NULL;
  free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */

/**
  * create a watchpoint
  */
WP* create_wp(){
  WP* new_wp = free_;
  free_ = free_->next;
  new_wp->next = head;
  head = new_wp;
  return new_wp;
}

/**
  * display watchpoint
  */
void wp_dispaly(){
  if(head!=NULL){
    printf("%-4s %-34s %-32s\n", "NO", "EXPR", "VALUE");
    for(WP *node=head; node!=NULL;node=node->next){
      printf("%-4d %-34s %-32d\n", node->NO,node->expr,node->value);
    }
  }else{
    printf("No watchpoints were added.\n");
  }
}

