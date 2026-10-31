#define APIFUNC far pascal _export

extern "C" LONG APIFUNC dspGetNumDevs (void);

extern "C" LONG APIFUNC dspLoadAlg (DWORD dwDeviceID,
												DWORD dwInstance,
												LPHALG lphAlg,
												LPSTR lpAlgSection,
												DWORD dwCallback,
												DWORD dwFlags);

extern "C" LONG APIFUNC dspReset(DWORD dwDeviceID);

#define DSPCB_MSGPENDING     1

