/* projet fin cycle 2020/2021
 *  HABOUSSI Zineddine
 *  Difli Zakaria
 *  Mouacher Abderazak
 *  USDB L3 ELEC
 *  réalisation d'un drone
 *  code du test les motors brushless
 */
#include <Servo.h> //c’est la bibliotheque servo 
Servo esc; //déclarer esc comme servo 
 void setup() { 
  esc.attach(8); //specifier le pin du signal d’esc 
  esc.writeMicroseconds(1000); //initialiser le signal a 1000 
  Serial.begin(9600); }
 }
void loop() { 
  int val; //créer variable val 
  val= analogRead(A0); //lire input de pin analog et stocker val
  val= map(val, 0, 1023,1000,2000); //changer la valeur du val de minimuim a max avec un potentiometer 
  esc.writeMicroseconds(val); //utitliser val comme signal using val as the signal to esc
}
