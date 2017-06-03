#include <stdint.h>
#include<stdio.h>
#include<stdlib.h>    //strlen

int main(int argc, char** argv) {
    
    if (argc != 3) {
        printf("Wrong number of argumens, expected 2 arguments.");
        return -1;
    }
    
    
    uint8_t x1 = (uint8_t) atoi(argv[1]);
    uint8_t x2 = (uint8_t) atoi(argv[2]);
    
    int16_t value_int_rec = (int16_t) (((x1 & 0x00FF) << 8) | (x2 & 0x00FF));
    
    printf("%d", value_int_rec);
}