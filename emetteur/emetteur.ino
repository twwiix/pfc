/* projet fin cycle 2020/2021
 *  HABOUSSI Zineddine
 *  Difli Zakaria
 *  Mouacher Abderazak
 *  USDB L3 ELEC
 *  réalisation d'un drone
 *  code du commande de drone a travers le module NRF32 (émetteur )
 */
#include <SPI.h> // bibliotheque qui permet de communiquer aces les périphétique SPI 
#include <nRF24L01.h> // bibliotheque du module NRF24L01 
#include <RF24.h> // bibliotheue du RAFIO FREQUENCY 
const uint64_t pipeOut = 0xE9E8F0F0E1LL; //l'adresse pour communiquer avec récepteur ( il faut que soit la meme dans les deux ) 
RF24 radio(9, 10); // selectionner les pins 
struct Signal  { // caractréstiques du sigal 
  byte throttle; 
  byte pitch; 
  byte roll; 
  byte yaw; 
  byte aux1; 
  byte aux2;
};
Signal data; // déclarer data comme signal 
void ResetData() // fonction pour initialliser les valeurs des dimensions
{
  data.throttle = 12; // stoper le moteur 
  data.pitch = 127; 
  data.roll = 127; 
  data.yaw = 127;
  // centre // centre // centre
} 
void setup() {
  //commencer
  radio.begin(); 
  radio.openWritingPipe(pipeOut); 
  radio.setAutoAck(false); 
  radio.setDataRate(RF24_250KBPS); 
  radio.setPALevel(RF24_PA_HIGH); 
  radio.stopListening(); //commencer la communication du radio pour émetteur ResetData(); // appeler la fonction resetdata
  }
  // centre du manette 
  int mapJoystickValues(int val, int lower, int middle, int upper, bool reverse)
  {
  val = constrain(val, lower, upper); 
  if ( val < middle ) 
    val = map(val, lower, middle, 0, 128); 
  else val = map(val, middle, upper, 128, 255); 
  return ( reverse ? 255 - val : val );
  } 
void loop() {
  // Étalonnage du manche de commande // Un réglage peut être nécessaire pour les valeurs correctes des leviers de commande 
  data.throttle = mapJoystickValues( analogRead(A0), 12, 524, 1020, true ); // "true" ou "false" pour direction du signal 
  data.roll = mapJoystickValues( analogRead(A3), 12, 524, 1020, true ); // "true" ou "false" pour direction servo 21
  
  data.pitch = mapJoystickValues( analogRead(A2), 12, 524, 1020, false ); // "true" ou "false" pour direction du servo 
  data.yaw = mapJoystickValues( analogRead(A1), 12, 524, 1020, false );// "false" pour direction servo 
  data.aux1 = mapJoystickValues( analogRead(A6), 12, 524, 1020, true );
  "false" pour direction du servo 
  data.aux2 = mapJoystickValues( analogRead(A7), 12, 524, 1020, true ); //"false" pour direction du servo 
  radio.write(&data, sizeof(Signal));
}
