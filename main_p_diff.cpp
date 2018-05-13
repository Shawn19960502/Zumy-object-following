/*
 * A sample colorized object following program for the Zumy board. (With P controler differential driving)
 * Author: Shawn Yang
 *
 * Behavior:
 * Zumy follows the "learned" colorized object until the robot is closet to the object.
 * Object center position is recorded every frame as an 16 bit unsigned int. The information 
 * of the robot motion is showed by the 4 LEDS.
 */

#include "mbed.h"
#include "Pixy.h"

/***** Constants ******/
#define GRAPH_CENTER 155

#define SDA p28
#define SCL p27

// Motors -- As labelled on the Zumy mbed board
PwmOut m1_fwd(p21);
PwmOut m1_back(p22);

PwmOut m2_fwd(p23);
PwmOut m2_back(p24);

// These constants define the base pwm across the motors and how much the controller
#define SPEED_PWM 0.20
//#define SPEED_PWM_DIRCTION_FOR 0.10
//#define SPPED_PWN_DIRECTION_BAC 0.10
#define Right_ThreHold 190
#define Left_ThreHold 100
// LEDs
DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);
DigitalOut led4(LED4);

// USB serial


//PID Parameter (Not used in this example, can be implemented if you
//are insterested)//
/*******************************
uint16_t previous_err_direction;
uint16_t previous_err_distance;
uint16_t integral_direction;
uint16_t integral_distance;
**************************/
#define Kp_dir 0.0010 //P controler Hyperparameter


/***** Helper functions *****/
// Sets the 4 LEDS on the mbed board to the four given binary values
void setLEDs(uint8_t a, uint8_t b, uint8_t c, uint8_t d);

//P controler for direction
float PControl_dir(uint16_t temp_x);

//PID controler for distance
int PID_dis(uint16_t width);

//function to change the speed of steer
void steerstupid(uint16_t temp_x, uint16_t temp_width);

Pixy temp(SDA,SCL);

int main() {

    setLEDs(1,1,1,1);
    wait(3);
    setLEDs(0,0,0,0);
    wait(2);
    //check the mbed is working
    while(1) {
        uint16_t temp_x = 0;
        uint16_t temp_width = 0;
       // setLEDs(1,1,1,1);
       // wait(1);
        temp.set_x_width();
       // setLEDs(1,1,1,0);
       // wait(1);
        temp_x = temp.ret_x();
      //  setLEDs(1,1,0,0);
       // wait(1);
        temp_width = temp.ret_width();
       // setLEDs(1,0,0,0);
       // wait(1);
        if(temp_x == 0 && temp_width == 0) {
            setLEDs(1,0,1,0);
            wait(1);
            setLEDs(0,0,0,0);
            wait(1);
        } else {
            /************************************
            This part is for debug.
            setLEDs(1,0,1,0);                   
            wait(1);
            setLEDs(0,0,0,0);
            pc.printf("center:%d\n",temp_x);
            **************************************/
            steerstupid(temp_x, temp_width);
        }
    }
}

//set four leds on Zumy
void setLEDs(uint8_t a, uint8_t b, uint8_t c, uint8_t d)
{
    led1 = a;
    led2 = b;
    led3 = c;
    led4 = d;
}

//Given the x position, output the desired PWM using p controler
float PControl_dir(uint16_t temp_x) {
    if(temp_x > GRAPH_CENTER) {
        float temp_pwm = (temp_x - GRAPH_CENTER) * Kp_dir;
        if (temp_pwm > 0.25) {
            return 0.25;
        } else {
            return temp_pwm;
        }
    } else {
        float temp_pwm = (GRAPH_CENTER - temp_x) * Kp_dir;
        if (temp_pwm > 0.25) {
            return 0.25;
        } else {
            return temp_pwm;
        }
    }
}

void steerstupid(uint16_t temp_x, uint16_t temp_width) {
    float pwm = PControl_dir(temp_x);

    if(temp_x > Left_ThreHold && temp_x < Right_ThreHold) {
        //go straight
       // pc.printf("%d ",temp_x);
        setLEDs(0,1,0,0);
        wait(1);
        setLEDs(0,0,0,0);
        m1_fwd.write(0.2);
        m1_back.write(0);
        m2_fwd.write(0.2);
        m2_back.write(0);
    }
    else if(temp_x <= Left_ThreHold ) {
        //trun left
      //  pc.printf("%d ",temp_x);
        setLEDs(0,0,1,0);
        wait(1);
        setLEDs(0,0,0,0);
        m2_fwd.write(pwm);
        m2_back.write(0);
        m1_back.write(pwm);
        m1_fwd.write(0);
    }
    else if(temp_x >= Right_ThreHold) {
        //turn right;
      //  pc.printf("%d ",temp_x);
        setLEDs(0,0,0,1);
        wait(1);
        setLEDs(0,0,0,0);
        m1_fwd.write(pwm);
        m1_back.write(0);
        m2_fwd.write(0);
        m2_back.write(pwm);
        
    }
} 


