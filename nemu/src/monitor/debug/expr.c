#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

/* my header file*/
/* Start */
#include<stdio.h>
#include<stdlib.h>
/* End */

/* External function */
/* End */
uint32_t isa_reg_str2val(const char *s, bool *success);
uint32_t paddr_read(paddr_t addr, int len);
/* End */

enum {
  TK_NOTYPE = 256, TK_EQ,

  /* TODO: Add more token types */
  /* Start */
  TK_INT, TK_HEX, TK_REG, TK_NOTEQ, TK_AND, TK_OR, TK_DEREFERENCE, TK_NEGATIVE
  /* End */
};

static struct rule {
  char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */

  {" +", TK_NOTYPE},    // spaces
  {"\\+", '+'},         // plus
  {"==", TK_EQ},        // equal

  /* Start */
  {"\\-", '-'},         // sub
  {"\\*", '*'},         // mul
  {"\\/", '/'},         // div
  {"\\(", '('},         // LP
  {"\\)", ')'},         // RP
  {"0[Xx][0-9A-Fa-f]+",TK_HEX},   // HEX
  {"[0-9]+", TK_INT},   // INT
  {"!=", TK_NOTEQ},     // not equal
  {"&&", TK_AND},       // and
  {"\\|\\|", TK_OR},          // or
  {"\\$(\\$0|ra|sp|gp|tp|t[0-6]|s[0-9]|a[0-7]|s10|s11))", TK_REG}        // reg
  /* End */
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  char str[32];
} Token;

static Token tokens[32] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;

static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);
        position += substr_len;

        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */
         /* Start */
        switch (rules[i].token_type) {
          case TK_NOTYPE:break;
          case '+' : tokens[nr_token++].type='+'; break;
          case '-' : tokens[nr_token++].type='-'; break;
          case '*' : tokens[nr_token++].type='*'; break;
          case '/' : tokens[nr_token++].type='/'; break;
          case '(' : tokens[nr_token++].type='('; break;
          case ')' : tokens[nr_token++].type=')'; break;
          case TK_EQ : tokens[nr_token++].type=TK_EQ; break;
          case TK_NOTEQ : tokens[nr_token++].type=TK_NOTEQ; break;
          case TK_AND : tokens[nr_token++].type=TK_AND; break;
          case TK_OR : tokens[nr_token++].type=TK_OR; break;
          case TK_INT : {
            tokens[nr_token].type=TK_INT; 
            if(substr_len>=32){
              printf("TK_INT too long\n");
              return false;
            }
            strncpy(tokens[nr_token].str,substr_start,substr_len);
            tokens[nr_token].str[substr_len]='\0';
            nr_token++;
            break;
          }
          case TK_HEX : {
            tokens[nr_token].type=TK_HEX; 
            if(substr_len>=32){
              printf("TK_HEX too long\n");
              return false;
            }
            strncpy(tokens[nr_token].str,substr_start,substr_len);
            tokens[nr_token].str[substr_len]='\0';
            nr_token++;
            break;
          }
          case TK_REG : {
            tokens[nr_token].type=TK_REG;
            strncpy(tokens[nr_token].str,substr_start+1,substr_len-1);
            tokens[nr_token].str[substr_len-1]='\0';
            nr_token++;
            break;
          }
          case TK_DEREFERENCE : tokens[nr_token++].type=TK_DEREFERENCE; break; 
          case TK_NEGATIVE : tokens[nr_token++].type=TK_NEGATIVE; break; 
        }
        /* End */
        break;
      }
    }

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }

  return true;
}

/*---my funtion to evaluate the expression---*/
/* Start */
void printTokens(int start,int end){
  int num = 0;
  for(int i=start; i<=end; i++){
    switch(tokens[i].type){
      case TK_NOTYPE:break;
      case '+' : printf("%-10c",'+'); break;
      case '-' : printf("%-10c",'-'); break;
      case '*' : printf("%-10c",'*'); break;
      case '/' : printf("%-10c",'/'); break;
      case '(' : printf("%-10c",'('); break;
      case ')' : printf("%-10c",')'); break;
      case TK_EQ : printf("%-10s","TK_EQ"); break;
      case TK_NOTEQ : printf("%-10s","TK_NOTEQ"); break;
      case TK_AND : printf("%-10s","TK_AND"); break;
      case TK_OR :  printf("%-10s","TK_OR"); break;
      case TK_INT : printf("%-10s","TK_INT"); num = atoi(tokens[i].str); printf("%-10d",num); break;
      case TK_HEX : printf("%-10s","TK_HEX"); sscanf(tokens[i].str,"%x",&num); printf("%-10d",num); break;
      case TK_REG : printf("%-10s","TK_REG"); printf("%-10s",tokens[i].str); break;
      default: printf("error\n");return;
    }
    printf("\n");
  }
}

bool check_parentheses(int p,int q){
  int lp  = 0;
  int rp  = 0;
  if(tokens[p].type=='(' && tokens[q].type==')'){
    for(int i = p+1; i<=q-1; i++){
      if( tokens[i].type=='(' ){
        lp++;
      }
      else if(tokens[i].type==')' && lp>rp ){
        rp++;
      }
      else if(tokens[i].type==')' && rp>=lp){
        return false;
      }
    }
    return lp == rp;
  }
  return false;
}

int operator_priority(int op){
  switch(op){
    case TK_NEGATIVE: return 1;
    case TK_DEREFERENCE: return 2;
    case '*':
    case '/': return 3;
    case '+':
    case '-': return 4;
    case TK_EQ:
    case TK_NOTEQ: return 5;
    case TK_AND: return 6;
    case TK_OR: return 7;
    default: return 0;
  }
}

uint32_t eval(int p, int q, bool *success){
  // printf("\nnew level\n");
  // printTokens(p,q);
  // printf("\n");
  // getchar();
  uint32_t num = 0;
  if( p > q ){
    /* Bad expression */
    printf("Bad expression\n");
    *success = false;
    return 0;
  }
  else if( p == q ){
    /* Single token.
     * For now this token should be a number.
     * Return the value of the number.
     */
    // printf("Single token: %s\n",tokens[p].str);
    switch(tokens[p].type){
      case TK_INT: {
        num = atoi(tokens[p].str); 
        break;
      }
      case TK_HEX: {
        num = sscanf(tokens[p].str,"%x",&num); 
        break;
      }
      case TK_REG: {
        num = isa_reg_str2val(tokens[p].str,success);
        break;
      }
      default:{
        printf("Bad Single token index : %d\n",p);
        *success = false;
        break;
      } 
    }
    return num;
  }
  else if( check_parentheses(p, q) == true ){
    /* The expression is surrounded by a matched pair of parentheses.
     * If that is the case, just throw away the parentheses.
     */
    //  printf("check_parentheses\n");
     num = eval(p+1,q-1,success);
     return num;
  }
  else{
    // printf("composite eval \n");
    /* We should do more things here. */
    int op_token_index = -1;
    int op_token_priority = 0;
    int cur_op_token_priority = 0;
    int lp = 0;
    int rp = 0;
    // find the primary operator to break the expression into two expressions
    for(int i=p; i<=q; i++){
      if(tokens[i].type=='('){
        lp++;
      }else if(tokens[i].type==')' && lp>rp){
        rp++;
      }else if(tokens[i].type==')' && rp>=lp){
        printf("Bad expression\n");
        *success = false;
        return 0;
      }
      if( lp!=rp || operator_priority(tokens[i].type)==0 ){
        continue;
      }
      cur_op_token_priority = operator_priority(tokens[i].type);
      if(cur_op_token_priority>=op_token_priority){
        op_token_priority = cur_op_token_priority;
        op_token_index = i;
      }
    }
    if( op_token_index>-1 ){
      uint32_t val1 = 0;
      uint32_t val2 = 0;
      val1 = eval(p,op_token_index-1,success);
      if(*success==false){
        return 0;
      }
      val2 = eval(op_token_index+1,q,success);
      if(*success==false){
        return 0;
      }
      switch(tokens[op_token_index].type){
        case TK_NEGATIVE: num = -val2; break;
        case TK_DEREFERENCE: num = paddr_read(val2,4); break;
        case '*': num = val1*val2; break;
        case '/': num = val1/val2; break;
        case '+': num = val1+val2; break;
        case '-': num = val1-val2; break;
        case TK_EQ: num = val1==val2; break;
        case TK_NOTEQ: num = val1!=val2; break;
        case TK_AND: num = val1&&val2; break;
        case TK_OR: num = val1||val2; break;
        default: {
          // This situation does not exist
          printf("tokens[op_token_index].type error\n");
          *success = false;
          return 0;
        }
      }
      // printf("num = %d\n",num);
      return num;
    }
    // printf("op_token_index == -1\n");
    *success = false;
    return 0;
  }
}

/* End */

uint32_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }

  /* TODO: Insert codes to evaluate the expression. */
  /* Start */
  // dereference address
  for(int i=0;i<nr_token;i++){
    if(tokens[i].type=='*'&&
        (i==0||tokens[i-1].type=='+'||tokens[i-1].type=='-'||tokens[i-1].type=='*'
        ||tokens[i-1].type=='/'||tokens[i-1].type=='('||tokens[i-1].type==TK_DEREFERENCE)
        ){
      tokens[i].type=TK_DEREFERENCE;
    }
  }
  // negative number
  for(int i=0;i<nr_token;i++){
    if(tokens[i].type=='-'&&
        (i==0||tokens[i-1].type=='+'||tokens[i-1].type=='-'||tokens[i-1].type=='*'
        ||tokens[i-1].type=='/'||tokens[i-1].type=='('||tokens[i-1].type==TK_NEGATIVE)
        ){
      tokens[i].type=TK_NEGATIVE;
    }
  }
  // evaluate the expr
  return eval(0, nr_token-1, success);
  /* End */
}