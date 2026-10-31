{_______________________________________________________________________________
	INIT0610.H
			Analog Devices INC. DSP Division
			One Technology Way, Norwood MA, 02062
			DSP Applications: (617) 461-3672

	This file holds the initial values of constants found in program 
	memory for the encoder and decoder (GSM spec. 06.10).  Since
	all these values are in program memory, they can be put into ROM.

		Release History:
	__Date___  _Ver_  ___Author___  _______________Comments_______________
	20-Jun-89    1    G Sterling    Add release history
	21-Aug-89   57    J Cuthbert    Add Comfort Noise variables
	12-Sep-89   59    J Cuthbert    Reallocating some vars from DM to PM
	03-Oct-89   61    J Cuthbert    Move all tables to PM
	14-Nov-89   63    S Cox         Remove all inits except constant tables
_______________________________________________________________________________}

	{Since tables are stored in PM data, they must be 
	 initialized as 24 bits.  The decimal equilavents are 
	 shown below for each case.}


.INIT	table_a : H#500000, H#500000, H#500000, H#500000,
		  H#368C00, H#3C0000, H#215600, H#234C00;
 
                  {20480, 20480, 20480, 20480, 13964, 15360, 8534, 9036;}

.INIT	table_b : H#000000, H#000000, H#080000, H#F60000, H#005E00, H#F90000, 
		  H#FEAB00, H#FB8800;

		  {0, 0, 2048, -2560, 94, -1792, -341, -1144;}

.INIT	table_mic : H#FFE000, H#FFE000, H#FFF000, H#FFF000, H#FFF800, 
		    H#FFF800, H#FFFC00, H#FFFC00;

		    {-32, -32, -16, -16, -8, -8, -4, -4;}

.INIT	table_mac : H#001F00, H#001F00, H#000F00, H#000F00, H#000700,
		    H#000700, H#000300, H#000300;

		    {31, 31, 15, 15, 7, 7, 3, 3;}

.INIT	table_inva : H#333300, H#333300, H#333300, H#333300,
		     H#4B1700, H#444400, H#7ADE00, H#740C00;

                     {13107, 13107, 13107, 13107, 19223, 17476, 31454, 29708;}

.INIT	table_dlb : H#199A00, H#400000, H#666600, H#7FFF00;

		    {6554, 16384, 26214, 32767;}

.INIT	table_qlb : H#0CCD00, H#2CCD00, H#533300, H#7FFF00;

		    {3277, 11469, 21299, 32767;}

.INIT	table_nrfac : H#71C800, H#666700, H#5D1800, H#555600, H#4EC500,
		      H#492500, H#444400, H#400000;

		      {29128, 26215, 23832, 21846, 20165, 18725, 17476, 16384;}

.INIT	table_fac : H#47FF00, H#4FFF00, H#57FF00, H#5FFF00, H#67FF00,
		    H#6FFF00, H#77FF00, H#7FFF00;

		    {18431, 20479, 22527, 24575, 26623, 28671, 30719, 32767;}

.INIT	h : H#FF7A00, H#FE8A00, H#000000, H#080600, H#166D00, H#200000,
	    H#166D00, H#080600, H#000000, H#FE8A00, H#FF7A00;

	   {-134, -374, 0, 2054, 5741, 8192, 5741, 2054, 0, -374, -134;}

