#include<stdio.h>
#include<string.h>    //strlen
#include<stdlib.h>    //strlen
#include<unistd.h>    //write
#include <errno.h>
#include<pthread.h> //for threading , link with lpthread
#include <ctype.h>

#include <math.h>
#include <time.h>
#include <inttypes.h>

#include <fcntl.h>
#include <stropts.h>
#include <asm/termios.h>
#include <unistd.h> 

volatile int PORT;

int main ()
{
    
    PORT = open("/dev/ttyUSB0", O_RDWR|O_NOCTTY);

    if (PORT < 0) {
        fprintf (stderr, "Unable to open serial device: %s\n", strerror(errno));
        return 1;
    }

    fcntl(PORT, F_SETFL, 0);

    int speed = 750000;

    struct termios2 tio;
    ioctl(PORT, TCGETS2, &tio);
    
    tio.c_ispeed = speed;
    tio.c_ospeed = speed;
    
    tio.c_cflag &= ~CBAUD;
    tio.c_cflag |= BOTHER;
    
    tio.c_cflag |= (CLOCAL | CREAD);    // Enable the receiver and set local mode
    tio.c_cflag |= PARENB;              // Parity enable
	tio.c_cflag |= PARODD;              // Enable odd parity 
    tio.c_cflag |= CSTOPB;              // Two stop bits
    tio.c_cflag &= ~CSIZE;              // Mask data size
    tio.c_cflag |=  CS8;                // Select 8 data bits
    tio.c_cflag &= ~CRTSCTS;            // Disable hardware flow control
    
    //options.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl
    
    
    /* setup for non-canonical mode */
    tio.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
    tio.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
    tio.c_oflag &= ~OPOST;

    /* fetch bytes as they become available */
    tio.c_cc[VMIN] = 0;
    tio.c_cc[VTIME] = 5;
    
    int r = ioctl(PORT, TCSETS2, &tio);

    if (r == 0) {
        printf("Serial device was successfully opened.\n");
    } else {
        fprintf (stderr, "Unable to configure serial device: %s\n", strerror(errno));
        return 1;
    }
    
    long            ms; // Milliseconds
    time_t          s;  // Seconds
    struct timespec spec_start;
    struct timespec spec_end;
    
    // Start time
    clock_gettime(CLOCK_REALTIME, &spec_start);

    char end[] = {13};
    
    int maxToRead = 100000;
    char data[maxToRead + 125];
    
    char buff[64];
    int rc = 0;
    int r_count = 0;
    
    // Send start ADC command
    write(PORT, "adc 1", strlen("adc 1"));
    write(PORT, end, strlen(end));
    
    while (r_count < maxToRead) {
        rc = read(PORT, buff, 64);
        
        for (int i = 0; i < rc; i++) {
            data[r_count++] = buff[i];
            printf("%c", buff[i]);
        }
    }
    
    // Start time
    clock_gettime(CLOCK_REALTIME, &spec_end);

    s  = spec_start.tv_sec;
    ms = round(spec_start.tv_nsec / 1.0e6); // Convert nanoseconds to milliseconds    
    printf("START:%"PRIdMAX"%03ld\n", (intmax_t)s, ms);
    

    
    for (int i = 0; i < r_count; i++) {
        //printf("%c", data[i]);
    }
    
    s  = spec_end.tv_sec;
    ms = round(spec_end.tv_nsec / 1.0e6); // Convert nanoseconds to milliseconds    
    printf("\nEND:%"PRIdMAX"%03ld\n", (intmax_t)s, ms);
    
    // Send stop ADC command
    write(PORT, "adc 0", strlen("adc 0"));
    write(PORT, end, strlen(end));
    
    close(PORT);
    
    
    return 0;
}