
#include "../Serial/SerialPort.h"

#define BUFSIZE 255

void *data_handler(char[]);

void *handler_command(void *);

char * extract_between(const char *str, const char *p1, const char *p2);

char *trimwhitespace(char *str);

SerialPort serialPort;

int main ()
{
    serialPort.addDataListner(data_handler);
    
    sleep(2);
    
    serialPort.writeCommand("setRxChannel 110");
    
    while (1) {
      sleep(5000);
    }
    
    return 0;
}

void *data_handler(char data[]) {
    // Copy from local array to our memory location for the thread
    int n = strlen(data);
    char *bred = (char *) malloc(n + 1);
    strcpy(bred, data);
    
    pthread_t handler_thread;
    if( pthread_create(&handler_thread, NULL, handler_command, (void*) bred) < 0 )
    {
        fprintf (stderr, "Unable to create pthread: %s\n", strerror(errno));
    }
    
    //pthread_join( listner_thread, NULL);
}

void *handler_command(void *data) {
    //printf("%s", reinterpret_cast<char *>(data));
    //printf("%s", (char*) data);
    //printf("2->%p\n", data);
    
    //printf("[%s]", (char*) data);   
    
    char *rf_data = extract_between((char*)data, "<DATA>", "</DATA>");
    
    if (rf_data != NULL) {
        
        char* rf_data_bytes[10];
        int rf_data_count = 0;
        char *state;
        
        char* pch = strtok_r(rf_data, ",", &state);
        if (pch != NULL) {
            rf_data_bytes[rf_data_count++] = pch;
        }

		while (pch != NULL) {
		    pch = strtok_r(NULL, ",", &state);
			rf_data_bytes[rf_data_count++] = pch;
		}
        
        //========================================================
        
        char pr_buf[BUFSIZE];
        FILE *pr_fp;
        
        char cmd[255];
        sprintf(cmd, "./rules.sh %s %s %s %s %s %s %s %s", 
            rf_data_bytes[0], 
            rf_data_bytes[1],
            rf_data_bytes[2],
            rf_data_bytes[3],
            rf_data_bytes[4],
            rf_data_bytes[5],
            rf_data_bytes[6],
            rf_data_bytes[7]);
        
        if ((pr_fp = popen(cmd, "r")) == NULL) {
            printf("Error opening pipe!\n");
        }
    
        while (fgets(pr_buf, BUFSIZE, pr_fp) != NULL) {
            serialPort.writeCommand(pr_buf);
        }
        
        if(pclose(pr_fp))  {
            printf("Command not found or exited with error status\n");
        }
        
        //========================================================

        free(rf_data);
    } else {
    }
    
    fflush(stdout);
    free(data);
}

char * extract_between(const char *str, const char *p1, const char *p2)
{
  const char *i1 = strstr(str, p1);
  if(i1 != NULL)
  {
    const size_t pl1 = strlen(p1);
    const char *i2 = strstr(i1 + pl1, p2);
    if(p2 != NULL)
    {
     /* Found both markers, extract text. */
     const size_t mlen = i2 - (i1 + pl1);
     char *ret = (char *) malloc(mlen + 1);
     if(ret != NULL)
     {
       memcpy(ret, i1 + pl1, mlen);
       ret[mlen] = '\0';
       return ret;
     }
    }
  }
  
  return NULL;
}

char *trimwhitespace(char *str)
{
  char *end;

  // Trim leading space
  while(isspace(*str)) str++;

  if(*str == 0)  // All spaces?
    return str;

  // Trim trailing space
  end = str + strlen(str) - 1;
  while(end > str && isspace(*end)) end--;

  // Write new null terminator
  *(end+1) = 0;

  return str;
}