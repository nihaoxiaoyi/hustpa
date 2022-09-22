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
/* Start */

/**
  * create a watchpoint
  */
WP* new_wp(uint32_t value, char *expr){
  WP* new_wp = free_;
  if(new_wp==NULL){
    return NULL;
  }
  free_ = free_->next;
  new_wp->next = head;
  head = new_wp;
  new_wp->value = value;
  strcpy(new_wp->expr,expr);
  return new_wp;
}

/**
  * delete a watchpoint
  */
WP* free_wp(int NO){
  if( NO<0 || NO>=32 || head==NULL){
    return NULL;
  }
  WP wp_head;
  WP *pre = &wp_head;
  pre->next = head;
  WP *p = pre->next;
  while(p!=NULL){
    if(p->NO == NO){
      pre->next = p->next;
      p->next = free_;
      free_->next = p;
      if(wp_head.next==NULL){
        head = NULL;
      }
      return p;
    }
    pre = p;
    p = p->next;
  }
  return NULL;
}

/**
  * update watchpoint
  */
bool wp_update(){
  WP *p = head;
  bool success = true;
  bool pause = false;
  uint32_t temp_value;
  while(p!=NULL){
    temp_value = expr(p->expr,&success);
    if(success && temp_value != p->value){
      p->value = temp_value;
      printf("%-4d %-32s %-12d 0x%-8x\n", p->NO,p->expr,p->value,p->value);
      pause = true;
    }
    p = p->next;
  }
  if(pause){
    printf("watchpoint changed\n");
  }
  return pause;
}

/**
  * display watchpoint
  */
void wp_dispaly(){
  if(head!=NULL){
    printf("%-4s %-32s %-12s %-8s\n", "NO", "EXPR", "DECIMAL", "HEX");
    for(WP *node=head; node!=NULL;node=node->next){
      printf("%-4d %-32s %-12d 0x%-8x\n", node->NO,node->expr,node->value,node->value);
    }
  }else{
    printf("No watchpoints were added.\n");
  }
}

/* End */