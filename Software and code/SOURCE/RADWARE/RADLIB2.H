void get_1848();
long GetNumDevs(void);
void Set_Morse(int how);
char convert_from_baudot(int which);
int save_voice_memory_start(char *filename,int which);
void save_voice_memory_end();
void save_voice_memory();
int save_voice_memory_open();
void start_play();
void end_play();
void end_play_and_null();
int Set_PTT(long Value);
long Check_PTT_Time();

int send_play_message(int *string);
