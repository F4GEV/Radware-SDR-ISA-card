/* These are some general external labels that everyone uses       	*/

.EXTERNAL	___lib_save_small_frame;
.EXTERNAL 	___lib_restore_small_frame;

.EXTERNAL       ___lib_frame_swap;
.EXTERNAL       ___lib_frame_restore;

.EXTERNAL       ___lib_setup_stack_read;

.EXTERNAL	___lib_div_core;

#define stack_ptr I4
#define frame_ptr M4

#define dm_ptr I1
#define dm_mdf M3
#define dm_lnt L1

#define pm_ptr I6
#define pm_mdf M5
#define pm_lnt L6

#define dm_1 M1
#define dm_0 M2

#define pm_0 M6

#define NULL 0

#ifdef DMSTACK

#define put(a,b) DM(I4,b)=a
#define get(a,b) a=DM(I6,b)

#define reads(a,b,c) a=DM(b,c)
#define writes(a,b,c) DM(b,c)=a
#define mem(i,m) DM(i,m)
#endif

#ifdef PMSTACK

#define put(a,b) DM(I4,b)=a
#define get(a,b) a=DM(I6,b)

#define reads(a,b,c) a=PM(b,c)
#define writes(a,b,c) PM(b,c)=a
#define mem(i,m) PM(i,m)
#endif


#define F_EXP_BIAS 127

