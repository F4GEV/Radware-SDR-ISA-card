#undef pm_ptr
#undef pm_mdf
#undef pm_lnt
#undef dm_ptr
#undef dm_mdf
#undef dm_lnt

#define pm_ptr I6
#define pm_mdf M6
#define pm_lnt L6

#define dm_ptr I6
#define pm_mdf M6
#define pm_lnt L6

#undef dm_1
#define dm_1 M1


#undef dm_0
#define dm_0 M0

#undef pm_M1
#define pm_M1 M7

#undef dm_M1
#define dm_M1 M3

#undef put
#define put(a) DM(I4,M7)=a

#undef get
#define get(a) a=DM(I4,M5)

#undef reads
#define reads(a,b,c) a=DM(b,c)


#define RTLRETURN RTS

