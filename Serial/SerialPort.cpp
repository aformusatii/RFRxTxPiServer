
#include "SerialPort.h"

volatile bool listen;
volatile int PORT;

void *(*data_handler)(char[]);
void *usart_handler(void *);

SerialPort::SerialPort() {
    
    PORT = open("/dev/ttyUSB0", O_RDWR|O_NOCTTY);

    if (PORT < 0) {
        fprintf (stderr, "Unable to open serial device: %s\n", strerror(errno));
        return;
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
        return;
    }
    
    // Create listening thread
    pthread_t listner_thread;
    if( pthread_create(&listner_thread, NULL, usart_handler, NULL) < 0)
    {
        fprintf (stderr, "Unable to create pthread: %s\n", strerror(errno));
        return;
    }
}

void *usart_handler(void *context) {
    char buff[8];
    int rc;
    char data[1024];
    int r_count = 0;
    
    listen = true;
    
    while (listen) {
        rc = read(PORT, buff, 8);
        
        for (int i = 0; i < rc; i++) {
            if ((buff[i] != 0) && (buff[i] != 127) && (buff[i] != 8)) {
                data[r_count++] = buff[i];
                printf("%c", buff[i]);
            }

            if (buff[i] == 13) {
                
                data[r_count] = 0;
                
                fflush(stdout);
                
                char sub_data[r_count + 1];
                
                memcpy(sub_data, data, r_count - 1);
                
                sub_data[r_count] = 0;
                
                data_handler(sub_data);
                r_count = 0;
            }
        }
    }
    
    close(PORT);

	return 0;
}

void SerialPort::addDataListner(void *(*d_handler)(char[])) {
	data_handler = d_handler;
}

void SerialPort::writeCommand(const char serial_cmd[]) {
    write(PORT, serial_cmd, strlen(serial_cmd));
    //printf("{%d}", strlen(serial_cmd));
    char end[] = {13};
    write(PORT, end, strlen(end));
}