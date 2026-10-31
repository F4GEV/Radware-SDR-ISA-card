#ifndef ___STDLIB_DEFINED
#define ___STDLIB_DEFINED

#ifndef NULL
#define	    NULL    (void *)0
#endif

#define	    EXIT_FAILURE 1
#define	    EXIT_SUCCESS 0
#define	    RAND_MAX 32767
#define	    MB_CUR_MAX 1

#ifndef ___T_STUFFDEFINED
typedef unsigned int	size_t;
typedef	int		wchar_t;
#define ___T_STUFFDEFINED
#endif 

typedef struct {int quot; int rem;} div_t;
typedef struct {long int quot; long int rem;} ldiv_t;

int	rand(void);
void	srand(unsigned int seed);
void	free(void *ptr);
void	*malloc(size_t size);
void	exit(int status);
int	abs(int j);
long int    labs(long int j);
double  fabs(double);
#endif

