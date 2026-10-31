#ifndef ___STDDEF_DEFINED
#define ___STDDEF_DEFINED

#ifndef NULL
#define	NULL	(void *)0
#endif

typedef int ptrdiff_t;
#ifndef ___T_STUFFDEFINED
typedef unsigned int size_t;
typedef int wchar_t;
#define ___T_STUFFDEFINED
#endif

#define offsetof(s,m) (size_t)&(((struct s *)(void *)0)->m)
#define segment(x) __attribute((segment = x))

#endif


