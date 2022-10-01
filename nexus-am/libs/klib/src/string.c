#include "klib.h"

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

/* My code */
/* Start */

size_t strlen(const char *s) {
  assert(s!=NULL);
  size_t len = 0;
  while ( s[len] != '\0' ) len++;
  return len;
}

char *strcpy(char* dst, const char* src) {
  assert(dst!=NULL && src!=NULL);
  size_t i = 0;
  while (src[i] != '\0') {
    dst[i] = src[i];
    i++;
  }
  dst[i] = '\0';
  return dst;
}

char* strncpy(char* dst, const char* src, size_t n) {
  assert(dst!=NULL && src!=NULL);
  size_t i = 0;
  while (i < n && src[i] != '\0'){
    dst[i] = src[i];
    i++;
  }
  dst[i] = '\0';
  return dst;
}

char* strcat(char* dst, const char* src) {
  assert(dst!=NULL && src!=NULL);
  size_t dst_len = strlen(dst);
  size_t i = 0;
  while (src[i] != '\0'){
    dst[dst_len + i] = src[i];
    i++;
  }
  dst[dst_len + i] = '\0';
  return dst;
}

int strcmp(const char* s1, const char* s2) {
  assert(s1!=NULL && s2!=NULL);
  size_t i = 0;
  while (s1[i]==s2[i]){
    if(s1[i] == '\0'){
      return 0;
    }
    i++;
  }
  int ret = s1[i]-s2[i];
  return ret>0 ? 1 : -1;
}

int strncmp(const char* s1, const char* s2, size_t n) {
  assert(s1!=NULL && s2!=NULL);
  if(n < 1) return 0;
  size_t i = 0;
  while (i<n && s1[i]==s2[i]){
    if(s1[i] == '\0' || i == n-1){
      return 0;
    }
    i++;
  }
  int ret = s1[i]-s2[i];
  return ret>0 ? 1 : -1;
}

void* memset(void* v,int c,size_t n) {
  assert(v!=NULL);
  char *p = (char* )v;
  for (size_t i = 0; i < n; i++){
    p[i]=(char)c;
  }
  return p;
}

void* memcpy(void* out, const void* in, size_t n) {
  assert(out!=NULL && in!=NULL);
  char* p = (char *)out;
  char* q = (char *)in;
  while (n-->0){
    *p++ = *q++;
  }
  return out;
}

int memcmp(const void* s1, const void* s2, size_t n){
  assert(s1!=NULL && s2!=NULL);
  if(n < 1) return 0;
  char* p = (char *)s1;
  char* q = (char *)s2;
  while (n-->0 && *p==*q){
    if(n == 0){
      return 0;
    }
    p++;
    q++; 
  }
  return (*p-*q)>0 ? 1 : -1;
}

/* End */

#endif
