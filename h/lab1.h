#define RANDOMSCHED 1
#define LINUXSCHED 2

extern int schedclass;
extern int getschedclass();
extern void setschedclass(int type);
