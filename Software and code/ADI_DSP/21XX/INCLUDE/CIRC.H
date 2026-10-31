/* @(#)circ.h 1.0 11/1/93  1 */

/*      This is the include file for the circular/autobuffer routines*/


#ifndef __CIRCBUF_DEFINED
#define __CIRCBUF_DEFINED


void *circ_setup(int dagreg, void *buffer, int length);
void *circ_swap(int dagreg, void *buffer);
void *circ_modify(void *address, int length, int offset);


#define circ_swap(a,b) ({int __temp;\
		       if(a==2)\
			asm("%0=I2;\n"\
			    "I2=%1;": "=c" (__temp) : "e" (b));\
		       else if (a==3)\
				asm("%0=I3;\n"\
			    	"I3=%1;": "=c" (__temp) : "e" (b));\
			    else\
				__temp=-1;\
			__temp;})


#define circ_modify(a,b,c) ({int __temp;\
			     asm("I6=%1;\n"\
				 "L6=%2;\n"\
				 "M5=%3;\n"\
				 "MODIFY(I6,M5);\n"\
				 "%0=I6;\n"\
				 "L6=0;\n": "=c" (__temp): "e" (a), "e" (b), "e" (c) : "i6");\
			      __temp;})


#define circ_setup(a,b,c) ({int __temp;\
			    if(a==2)\
			      asm("I2=%2;\n"\
				  "L2=%3;\n"\
				  "%0=%1;" : "=c" (__temp) : "e" (a), "e" (b), "e" (c));\
			    else if (a==3)\
			           asm("I3=%2;\n"\
				       "L3=%3;"\
				       "%0=%1;" : "=c" (__temp) : "e" (a), "e" (b), "e" (c));\
			         else\
				   __temp=-1;\
			    __temp;})


#endif

