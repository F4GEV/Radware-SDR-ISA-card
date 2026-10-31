/* This module will be located in the CRTL.  
   If you want to change the heap size, copy this file into your working
   directory and assemble and link it in explicitly.
*/

.MODULE		Heap_Declaration;

.VAR/DM/RAM	____heap[128];
.GLOBAL		____heap;

.endmod;


