/* A mbed library for pixy camera
 * Author: Shawn Yang
 */

/*I'm going to use I2C to communicate between sensor and mbed*/
#ifndef PixyH
#define PixyH

#include "mbed.h"

#define SLAVE_ADD  0X54 << 1
#define PIXY_LED_SYNC    0xfd


class Pixy {
	public:
		/*I2c communicate will be generated*/
		Pixy(PinName SDA, PinName SCL);

		/* Read From Sensor to private data*/
		/* Read specific bytes*/
		char* read(int num_byte);

		/* Check if the pixy is connected to the mbed*/
		void checkconnected(int r, int g, int b);

		uint16_t getWord(void);

		/* set x axis*/
		void set_x_width(void);

		uint16_t ret_x(void);

		uint16_t ret_width(void);

	protected:
		uint16_t x;
		I2C i2c;
		uint16_t width;
		//uint16_t height;
//		DigitalOut clk;
//		DigitalOut data;
		

};

#endif