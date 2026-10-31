void DSPGetMsg();
long DSPGetNumDevs();
long DSPReset();
long DSPLoadAlg(DWORD dwDeviceID,DWORD dwInstance,
					 LPSTR lpAlgSection,HWND Test_Window);
long DSPSendData (LPVOID lpMsgBody,DWORD dwSize);
long DSPSendMsg (LONG lMsgID,LPVOID lpMsgBody,DWORD dwSize);
long DSPUnloadAlg();
long DSPGetData(DWORD dwSize);
DSPSendDataNull(int);

void Simulation_Mode();

#define msg_ECHO_TOGGLE 501
#define msg_PLOT_TOGGLE 502
#define msg_LOG_TOGGLE  505
#define msg_LPF_TOGGLE  507
#define msg_PLOT_SEND   509

#define msg_MORSE_CHAR  1501
#define msg_START_MORSE 1502
#define msg_STOP_MORSE  1503
#define msg_RECIEVE     1504
#define msg_TRANSMIT    1505
#define msg_PLOT_LSB_S  1511
#define msg_PLOT_USB_S  1512
#define msg_PLOT_LSB_T  1513
#define msg_PLOT_USB_T  1514
#define msg_MUTE        1515
#define msg_MORSE_SPEED 1516
#define msg_SMETER      1517


