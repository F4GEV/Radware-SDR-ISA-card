#ifndef _stdarg_h
#define _stdarg_h
typedef char *va_list;

#define va_start(ap,v) ap = (va_list)&v + sizeof(v)
#define va_arg(ap,t) ((t*)(ap += sizeof(t)))[-1]
#define va_end(ap) ap = NULL
#endif
