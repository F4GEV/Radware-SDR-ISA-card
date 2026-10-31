.MODULE/IMAGE	Setup_Heap_at_Startup;


#include	<lib_glob.h>;

.ENTRY		___lib_setup_heap;

.EXTERNAL	____heap;     

.VAR/DM/RAM	    __mem_heap_space[2];
.GLOBAL		    __mem_heap_space;

___lib_setup_heap:
		    I6=^____heap;
		    M5=1;
		    DM(I6,M5)=%____heap;
		    DM(I6,M5)=NULL;
		    AY1=0;			/*Set length to zero*/
		    DM(__mem_heap_space)=AY1;	/*In heap space marker*/
		    AY1=^____heap;	    	/*Set next pointer to*/
		    DM(__mem_heap_space+1)=AY1;	/*top of heap*/
		   
		    RTS;

.ENDMOD;





