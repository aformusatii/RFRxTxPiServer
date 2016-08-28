#include<stdio.h>
#include<string.h>    //strlen
#include<stdlib.h>    //strlen
#include<unistd.h>    //write
#include <errno.h>
#include<pthread.h> //for threading , link with lpthread
#include <ctype.h>

#include <fcntl.h>
#include <stropts.h>
#include <asm/termios.h>
#include <unistd.h> 

class SerialPort {
    public:
    	SerialPort();
        void addDataListner(void *(*)(char[]));
        void writeCommand(const char[]);
};
