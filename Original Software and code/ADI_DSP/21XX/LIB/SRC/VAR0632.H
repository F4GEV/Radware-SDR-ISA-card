{________________________________________________________________________
	VAR0632.H

	This file holds the variable declarations for the Voice Activity
	Detection (GSM 06.32) routine.  It is NOT used when the assembler
	switch "-Dalias" is specified.

________________________________________________________________________}

{_________Variables that need to be saved for successive frames__________}
.VAR/DM/RAM      rvad[9];         {3.1,3.6}

.VAR/DM/RAM      normrvad;        {3.1,3.6}
.VAR/DM/RAM/CIRC L_sacf[54];      {3.2}
.VAR/DM/RAM/CIRC L_sav0[72];      {3.2}

.VAR/DM/RAM      pt_sacf;         {3.2}
.VAR/DM/RAM      pt_sav0;         {3.2}

.VAR/DM/RAM      L_lastdm[2],     {3.4}         { must be declared in sequence }
                 oldlagcount,     {3.5,3.9}     { for reset routine }
                 veryoldlagcount, {3.5,3.9}
                 adaptcount,      {3.6}
                 burstcount;      {3.8}

.VAR/DM/RAM      e_thvad;         {3.6}
.VAR/DM/RAM      m_thvad;         {3.6}

.VAR/DM/RAM      hangcount;       {3.8}

.VAR/DM/RAM      oldlag;          {3.9}

{_________Variables that are only used within vad_routine__________}
.VAR/DM/RAM      r_a_av1[9];      {aav1:3.3, rav1:3.3,3.4,3.6}
.VAR/DM/RAM      sav0[9];         {3.4}
.VAR/DM/RAM      sacf[9];         {3.1,3.3}
.VAR/DM/RAM      vpar[8];         {3.3}
.VAR/DM/RAM      e_acf0;          {3.1,3.6}
.VAR/DM/RAM      m_acf0;          {3.1,3.6}
.VAR/DM/RAM      L_coef[18];      {3.3}
.VAR/DM/RAM      e_pvad;          {3.1,3.6}
.VAR/DM/RAM      m_pvad;          {3.1,3.6}
.VAR/DM/RAM      L_work[18];      {3.3}
.VAR/DM/RAM      L_av0[18];       {3.2,3.4}
.VAR/DM/RAM      L_av1[18];       {3.2,3.3}

.VAR/DM/RAM      normrav1;        {3.3,3.4,3.6}
.VAR/DM/RAM      ptch;            {3.5,3.6}
.VAR/DM/RAM      stat;            {3.4,3.6}

{_________Variable that is the result of vad_routine__________}
.VAR/DM/RAM      vad;             {3.8}

{_________Variable that is only used within update_periodicity__________}
.VAR/DM/RAM      lags[4];         {3.9}

