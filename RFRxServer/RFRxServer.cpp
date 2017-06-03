
#include "../Serial/SerialPort.h"

void *data_handler(char[]);

SerialPort serialPort;

int main ()
{
    serialPort.addDataListner(data_handler);
    
    sleep(2);
    
    serialPort.writeCommand("setRxChannel 110");

    sleep(2);

    serialPort.writeCommand("setRxChannel 110");
    
    while (1) {
      sleep(5000);
    }
    
    return 0;
}

void *data_handler(char data[]) {
    char* rf_data_bytes[16];
    int rf_data_count = 0;
    char *state;
	
    if (strstr(data, "INITIALIZED 000001") != NULL) {
        printf("INITIALIZED 000001!\n");
        serialPort.writeCommand("setRxChannel 110");
    }
    
    if ((strstr(data, "DATA,") != NULL) && (strstr(data, ",DATA") != NULL)) {
        char* pch = strtok_r(data, ",", &state);
        
        if (pch != NULL) {
            rf_data_bytes[rf_data_count++] = pch;
        }
    
    	while (pch != NULL) {
    	    pch = strtok_r(NULL, ",", &state);
    		rf_data_bytes[rf_data_count++] = pch;
    	}
        
        //========================================================
        char cmd[255];
        sprintf(cmd, "./rules.sh %s %s %s %s %s %s %s %s &", 
            rf_data_bytes[1],
            rf_data_bytes[2],
            rf_data_bytes[3],
            rf_data_bytes[4],
            rf_data_bytes[5],
            rf_data_bytes[6],
            rf_data_bytes[7],
            rf_data_bytes[8]);
            \
        system(cmd);
        
        //printf("cmd=%s\n", cmd);
    } else {
        printf("%s", data);
    }
}
