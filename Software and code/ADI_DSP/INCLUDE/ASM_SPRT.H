{   
    This include file holds several macros that are helpful in developing
    assembly code for use with a C program
}

.MACRO save_machine_state(%0);    

	SI=M4;					    {Hold old Frame Pointer}
	M4=I4;					    {Set Frame = Stack}
	%0(I4,M7)=SI;				    {Save old Frame on stack} 
	%0(I4,M7)=AR;				    {Save data registers}
	%0(I4,M7)=AX0; %0(I4,M7)=AX1; 
	%0(I4,M7)=AY0; %0(I4,M7)=AY1; 
	%0(I4,M7)=MR0; %0(I4,M7)=MR1; 
	%0(I4,M7)=MX0; %0(I4,M7)=MX1; 
	%0(I4,M7)=MY0; %0(I4,M7)=MY1; 
	%0(I4,M7)=SR0; %0(I4,M7)=SR1;
	SI=I0; %0(I4,M7)=SI; SI=M0; %0(I4,M7)=SI;   {Save DAG registers}
	SI=I1; %0(I4,M7)=SI; SI=M1; %0(I4,M7)=SI; 
	SI=I2; %0(I4,M7)=SI; SI=M2; %0(I4,M7)=SI; 
	SI=I3; %0(I4,M7)=SI; SI=M3; %0(I4,M7)=SI; 
	SI=I5; %0(I4,M7)=SI; SI=I7; %0(I4,M7)=SI;

.ENDMACRO;

.MACRO save_selected_registers(%0);    

	SI=M4;					    {Hold old Frame Pointer}
	M4=I4;					    {Set Frame = Stack}
	%0(I4,M7)=SI;				    {Save old Frame on stack} 
	%0(I4,M7)=AR;				    {Save data registers}
	%0(I4,M7)=AY0; %0(I4,M7)=AY1; 
	%0(I4,M7)=MR0; %0(I4,M7)=MR1; 
	%0(I4,M7)=MX0; %0(I4,M7)=MX1; 
	%0(I4,M7)=MY0; %0(I4,M7)=MY1; 
	%0(I4,M7)=SR0; %0(I4,M7)=SR1;

.ENDMACRO;

.MACRO	putfirst(%0,%1);	{Save first register one C Stack}
	SI=M4;			{Hold old Frame pointer in SI}
	M4=I4;			{Set Frame = Stack pointer}
	%1(I4,M7)=SI;		{Save old Frame on stack}
	%1(I4,M7)=%0;		{Save register on stack}
.ENDMACRO;

.MACRO	put(%0,%1);		{Save a register on stack}
	%1(I4,M7)=%0;	
.ENDMACRO;

.MACRO	putdag(%0,%1);		{Save a DAG register on stack}
	SI=%0;			{Copy DAG into SI}
	%1(I4,M7)=SI;
.ENDMACRO;

.MACRO restore_machine_state(%0);

	MODIFY(I4,M5);				    {Point to valid data on stack} 
	SI=%0(I4,M5); I7=SI; SI=%0(I4,M5); I5=SI;   {Restore DAG registers}
	SI=%0(I4,M5); M3=SI; SI=%0(I4,M5); I3=SI; 
	SI=%0(I4,M5); M2=SI; SI=%0(I4,M5); I2=SI; 
	SI=%0(I4,M5); M1=SI; SI=%0(I4,M5); I1=SI; 
	SI=%0(I4,M5); M0=SI; SI=%0(I4,M5); I0=SI; 
	SR1=%0(I4,M5); SR0=%0(I4,M5);		    {Restore data registers}
	MY1=%0(I4,M5); MY0=%0(I4,M5); 
	MX1=%0(I4,M5); MX0=%0(I4,M5); 
	MR1=%0(I4,M5); MR0=%0(I4,M5); 
	AY1=%0(I4,M5); AY0=%0(I4,M5); 
	AX1=%0(I4,M5); AX0=%0(I4,M5); 
	AR=%0(I4,M5); 
	SI=%0(I4,M5);				    {Fetch old Frame}
	I4=M4;					    {Set Stack = Frame}
	M4=SI;					    {Restore previous Frame}

.ENDMACRO;

.MACRO restore_selected_registers(%0);

	MODIFY(I4,M5);				    {Point to valid data on stack} 
	SR1=%0(I4,M5); SR0=%0(I4,M5);		    {Restore data registers}
	MY1=%0(I4,M5); MY0=%0(I4,M5); 
	MX1=%0(I4,M5); MX0=%0(I4,M5); 
	MR1=%0(I4,M5); MR0=%0(I4,M5); 
	AY1=%0(I4,M5); AY0=%0(I4,M5); 
	AR=%0(I4,M5); 
	SI=%0(I4,M5);				    {Fetch old Frame}
	I4=M4;					    {Set Stack = Frame}
	M4=SI;					    {Restore previous Frame}

.ENDMACRO;

.MACRO	getfirst(%0,%1);	{Get the first register from the stack}
	MODIFY(I4,M5);		{Modify Stack pointer to point to valid data}
	%0=%1(I4,M5);		{Fetch register from stack}
.ENDMACRO;

.MACRO	get(%0,%1);		{Get the next registers from the stack}
	%0=%1(I4,M5);		{Fetch the register from the stack}
.ENDMACRO;

.MACRO	getdag(%0,%1);		{Get a DAG register from the stack}
	SI=%1(I4,M5);		{Fetch from stack into SI}
	%0=SI;			{Copy SI into DAG register}
.ENDMACRO;

.MACRO	getlast(%0,%1);		{Get the last register from the stack}
	%0=%1(I4,M5);		{Fetch register}
	SI=%1(I4,M5);		{Fetch old Frame Pointer}
	I4=M4;			{Set Stack = Frame}
	M4=SI;			{Restore old Frame pointer}
.ENDMACRO;

.MACRO	readstack(%0,%1,%2);	{Read an argument into a register}
	I6=%1;			{Set I6 equal to offset}
	MODIFY(I6,M4);		{Point to argument}
	%0=%2(I6,M5);		{Read the argument into register}
.ENDMACRO;

