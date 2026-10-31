/* This module will be located in the CRTL.  
   If you want to change the stack size, copy this file into your home
   directory and assemble and link it in explicitly.
*/

.MODULE		Stack_Declaration;

.VAR/DM/RAM	stack[1024], ____top_of_stack;
.GLOBAL		____top_of_stack;

.endmod;


