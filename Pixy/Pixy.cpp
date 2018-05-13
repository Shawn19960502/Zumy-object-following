/*Author Shawn Yang*/
/*Use I2c to communicate between mbed and pixy*/
#include "Pixy.h"

Serial pc(USBTX, USBRX);

Pixy::Pixy(PinName SDA, PinName SCL):i2c(SDA,SCL)
{
    i2c.frequency(200000); 
}

char* Pixy::read(int num_byte) {
    char * temp = (char *) malloc(num_byte);
    i2c.read(SLAVE_ADD,temp,num_byte,false);
    return temp;
    
}
void Pixy::checkconnected(int r, int g, int b) {
    char outBuf[5];
    outBuf[0] = 0x00;
    outBuf[1] = PIXY_LED_SYNC;
    outBuf[2] = r;
    outBuf[3] = g;
    outBuf[4] = b;
    i2c.write(SLAVE_ADD, outBuf,5,false);
    wait(5);
}

uint16_t Pixy::getWord(void)
{
  // this routine assumes little endian 
    char * c = read(2);
    //char * w = read(1);
    uint16_t temp = 0;
    temp = (c[1] << 8) | c[0];
    return temp;
}

void Pixy::set_x_width() {
    while(1) {
        uint16_t first = getWord();
        pc.printf("first:%d ",first);
        if(first == 0xaa55) {
            uint16_t second = getWord();
            pc.printf("second:%d ",second);
            if(second == 0xaa55) {
                char * temp = read(12);
                x = (uint16_t)(temp[5] << 8 | temp[4]);
                pc.printf("x:%d ",x);
                width = (uint16_t)(temp[9] << 8 | temp[8]);
                pc.printf("width:%d",width);
                break;
            }
        }
    }
}


uint16_t Pixy::ret_x() {
    return x;
}

uint16_t Pixy::ret_width() {
    return width;
}

