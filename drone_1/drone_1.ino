/* projet fin cycle 2020/2021
 *  HABOUSSI Zineddine
 *  Difli Zakaria
 *  Mouacher Abderazak
 *  USDB L3 ELEC
 *  réalisation d'un drone
 *  code alternative pour commander les moteurs
 */

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

int ch_width_1 = 0;
int ch_width_2 = 0;
int ch_width_3 = 0;
int ch_width_4 = 0;
int ch_width_5 = 0;
int ch_width_6 = 0;

Servo ch1;
Servo ch2;
Servo ch3;
Servo ch4;
Servo ch5;
Servo ch6;

struct Signal  {
byte throttle;      
byte pitch;
byte roll;
byte yaw;
byte aux1;
byte aux2;
};

Signal data;

const uint64_t pipeIn = 0xE9E8F0F0E1LL;
RF24 radio(8, 7); 

void ResetData()
{

data.roll = 127;   
data.pitch = 127;   
data.throttle = 12;
data.yaw = 127;   

}

void setup()
{
 
  ch1.attach(2);
  ch2.attach(3);
  ch3.attach(4);
  ch4.attach(5);

  
  ResetData();
  radio.begin();
  radio.openReadingPipe(1,pipeIn);
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_HIGH);
  radio.startListening(); 
  pinMode(6,OUTPUT);

}

unsigned long lastRecvTime = 0;

void recvData()
{
while ( radio.available() ) {
radio.read(&data, sizeof(Signal));
lastRecvTime = millis();   
}
}

void loop()
{
recvData();
unsigned long now = millis();
if ( now - lastRecvTime > 1000 ) {
ResetData(); 
}

ch_width_4 = map(data.yaw,      0, 255, 1000, 2000);     // pin D5 (PWM signal)
ch_width_2 = map(data.pitch,    0, 255, 1000, 2000);     // pin D3 (PWM signal)
ch_width_3 = map(data.throttle, 0, 255, 1000, 2000);     // pin D4 (PWM signal)
ch_width_1 = map(data.roll,     0, 255, 1000, 2000);     // pin D2 (PWM signal)

// Write the PWM signal 
ch1.writeMicroseconds(ch_width_1);
ch2.writeMicroseconds(ch_width_2);
ch3.writeMicroseconds(ch_width_3);
ch4.writeMicroseconds(ch_width_4);

}
