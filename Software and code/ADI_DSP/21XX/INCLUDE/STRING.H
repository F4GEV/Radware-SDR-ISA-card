#ifndef ___STRING_DEFINED
#define ___STRING_DEFINED

#ifndef NULL
#define	    NULL    (void *)0
#endif

#ifndef ___T_STUFFDEFINED
typedef unsigned int size_t;
#define ___T_STUFFDEFINED
#endif

void	*memcpy(void *s1,  const void *s2, size_t n);
void	*memset(void *s1, int c, size_t n);
int	memcmp(const void *s1, const void *s2, size_t n);
char	*strcpy(char *s1, const char *s2);
char	*strncpy(char *s1, const char *s2, size_t n);
char	*strcat(char *s1, const char *s2);
char	*strncat(char *s1, const char *s2, size_t n);
int	strcmp(const char *s1, const char *s2);
int	strncmp(const char *s1, const char *s2, size_t n);
size_t	strlen(const char *s1);

#endif


