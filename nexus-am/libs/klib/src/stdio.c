// #include "klib.h"
// #include <stdarg.h>

// #if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

// /* Start */

// static char buf[1000];

// int printf(const char *fmt, ...) {
//   va_list ap;
//   va_start(ap, fmt);
//   int len = vsprintf(buf, fmt, ap);
//   va_end(ap);
//   int i = 0;
//   while(buf[i] != '\0'){
//     _putc(buf[i++]);
//   }
//   return len;
// }

// int vsprintf(char *out, const char *fmt, va_list ap) {
//   char *p = out;
//   while((*fmt)!='\0'){
//     if(*fmt!='%'){
//       *p++=*fmt++;
//     }
//     else{
//       fmt++;
//       switch( *fmt ){
//         case 's':{
//           char *str = va_arg(ap, char*);
//           while(*str!='\0'){
//             *p++ = *str++;
//           }
//           break;
//         }
//         case 'd':{
//           char num_buf[11];
//           int num = va_arg(ap, int);
//           if(num < 0){
//             *p++ = '-';
//           }
//           int i = 0;
//           do{
//             num_buf[i++] = num%10+'0';
//             num = num/10;
//           }while(num != 0);
//           while (--i >= 0){
//             *p++ = num_buf[i];
//           }
//           break;
//         }
//         case 'x':{
//           char buf[12];
//           uint32_t hex_num = va_arg(ap, uint32_t);
//           *p++='0';
//           *p++='x';
//           int i=0;
//           do{
//             int tmp_num = hex_num % 16;
//             if(tmp_num < 10){
//               buf[i++]=tmp_num+'0';
//             }  
//             else{
//               buf[i++] = tmp_num-10+'a';
//             }
//             hex_num = hex_num/16;
//           }while(hex_num!=0);
//           while (--i>=0){
//             *p++=buf[i];
//           }
//           break;
//         }
//       }
//       fmt++;
//     }
//   }
//   *p = '\0';
//   return p-out;
// }

// int sprintf(char *out, const char *fmt, ...) {
//   va_list ap;
//   va_start(ap,fmt);
//   int len=vsprintf(out,fmt,ap);
//   va_end(ap);
//   return len;
// }

// int snprintf(char *out, size_t n, const char *fmt, ...) {
//   return 0;
// }

// /* End */

// #endif
#include "klib.h"
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

int printf(const char *fmt, ...) {
  char buf[1000];
  va_list ap;
  va_start(ap,fmt);
  int length=vsprintf(buf,fmt,ap);
  va_end(ap);
  int i=0;
  while(buf[i]!='\0'){
    _putc(buf[i]);
    i++;
  }
  return length;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  char *temp=out;
  while(*fmt!='\0'){
    if(*fmt!='%'){
      *temp++=*fmt++;
    }
    else{
      fmt++;
      switch(*fmt){
        case 's':{
          char *str=va_arg(ap,char*);
          while(*str!='\0'){
            *temp++=*str++;
          }
          break;
        }
        case 'd':{
          int n=va_arg(ap,int);
          if(n==0){
            *temp++='0';
            break;
          }
          if(n<0){
            *temp++='-';
            n=-n;
          }
          char buf[11];
          int i=0;
          while(n!=0){
            buf[i++]=n%10+'0';
            n=n/10;
          }
          for(int j=i-1;j>=0;j--){
            *temp++=buf[j];
          }
          break;
        }
        case 'x':{
          int n=va_arg(ap,int);
          if(n==0){
            *temp++='0';
            *temp++='x';
            *temp++='0';
            break;
          }
          if(n<0){
            *temp++='-';
            n=-n;
          }
          char buf[12];
          int i=0;
          while(n!=0){
            int a=n%16;
            if(a<10)
              buf[i++]=a+'0';
            else
              buf[i++]=a-10+'a';
            n=n/16;
          }
          *temp++='0';
          *temp++='x';
          for(int j=i-1;j>=0;j--){
            *temp++=buf[j];
          }
          break;
        }
      }
      fmt++;
    }
  }
  *temp='\0';
  return temp-out;
}

int sprintf(char *out, const char *fmt, ...) {
  va_list ap;
  va_start(ap,fmt);
  int length=vsprintf(out,fmt,ap);
  va_end(ap);
  return length;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  return 0;
}

#endif
