/* projet fin cycle 2020/2021
 *  HABOUSSI Zineddine
 *  Difli Zakaria
 *  Mouacher Abderazak
 *  USDB L3 ELEC
 *  réalisation d'un drone
 *  code du commande de drone a travers une page web
 */
#include <WiFi.h> 
#include <WebServer.h>
/* pour connecter au wifi du ESP32 */ 
const char* ssid = "ESP32"; // ssid
const char* password = "12345678"; //mot de passe
/* adresse IP pour page web */ 
IPAddress local_ip(192,168,1,1); 
IPAddress gateway(192,168,1,1); 
IPAddress subnet(255,255,255,0);
WebServer server(80);
/* déclarer les moteurs (leurs esc ) */ 
int esc1 = 0; 
int esc2 = 0; 
int esc3 = 0; 
int esc4 = 0; 
int esc_min = 400; 
int esc_inc = 10;
/* déclarer la commande (stop ou start ...ect) */ 
String cmd="";
uint8_t LED1pin = 23; 
uint8_t LED2pin = 22; 
uint8_t LED3pin = 21; 
uint8_t LED4pin = 19; 
uint8_t ledcSetup; // proprietes PWM  
const int freq = 60; 
const int resolution = 12; //Resolution 8, 10, 12, 15 
const int led1Channel = 0; 
const int led2Channel = 1; 
const int led3Channel = 2; 
const int led4Channel = 3; 
void setup() {
  Serial.begin(115200);
  pinMode(LED1pin, OUTPUT); 
  pinMode(LED2pin, OUTPUT); 
  pinMode(LED3pin, OUTPUT); 
  pinMode(LED4pin, OUTPUT);
  // configurer led pwm 
  ledcSetup(led1Channel, freq, resolution); 
  ledcSetup(led2Channel, freq, resolution); 
  ledcSetup(led3Channel, freq, resolution); 
  ledcSetup(led4Channel, freq, resolution);
  // attacher la chaine GPIO2 pour controller 
  ledcAttachPin(LED1pin, led1Channel);
  ledcAttachPin(LED2pin, led2Channel); 
  ledcAttachPin(LED3pin, led3Channel); 
  ledcAttachPin(LED4pin, led4Channel);
  Serial.println("\n[*] Creating AP"); 
  WiFi.mode(WIFI_AP); 
  WiFi.softAP(ssid, password); 
  WiFi.softAPConfig(local_ip, gateway, subnet); 
  Serial.print("[+] AP Created with IP Gateway "); 
  Serial.println(WiFi.softAPIP()); 
  delay(100); 
  server.on("/", handle_OnConnect);
  server.on("/get", handle_get);
  server.onNotFound(handle_NotFound); 
  server.begin(); 
  Serial.println("HTTP server a commencé");
}
void loop() { 
  server.handleClient();
  delay(10); 
}
void handle_OnConnect() { 
  Serial.println("GPIO4 Status: OFF | GPIO5 Status: OFF"); 
  server.send(200, "text/html", SendHTML());
}
void handle_get() { 
  int arg_count = server.args(); 
  //sval = server.argName(i); 
  esc1 = server.arg(0).toInt(); 
  esc2 = server.arg(1).toInt(); 
  esc3 = server.arg(2).toInt(); 
  esc4 = server.arg(3).toInt(); 
  cmd = server.arg(4);
  hardware_cmd();
  String message = "";
  message += "\nArguments: "; message += server.args(); message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) { message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  } 
  Serial.println(message); server.send(200, "text/html", SendHTML()); }
void handle_NotFound(){ 
  server.send(404, "text/plain", "Not found");
}
void hardware_cmd(){ 
  if(cmd == "Start"){ esc1 = esc_min; esc2 = esc_min; esc3 = esc_min; esc4 = esc_min;
  }
  else if(cmd == "Stop"){ esc1 = 0; 
    esc2 = 0; 
    esc3 = 0;
    esc4 = 0;
   } 
  else if(cmd == "Update"){ 
    
   }
  else if(cmd == "Speed up"){ 
    esc1 += esc_inc; 
    esc2 += esc_inc; 
    esc3 += esc_inc; 
    esc4 += esc_inc;
  }
else if(cmd == "Speed down"){ esc1 -= esc_inc; esc2 -= esc_inc; esc3 -= esc_inc; esc4 -= esc_inc;
}
ledcWrite(led1Channel, esc1); 
ledcWrite(led2Channel, esc2); ledcWrite(led3Channel, esc3); ledcWrite(led4Channel, esc4);
} // partie html pour la page web String SendHTML(){
String ptr = "<!DOCTYPE html> <html>\n";
ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n"; ptr +="<title>LED Control</title>\n"; ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-
align: center;}\n"; ptr +="body{margin-top: 50px; background:#444444;} h1 {color: white;margin: 50px auto
30px;} h3 {color: #444444;margin-bottom: 50px;}\n"; ptr +=".button {display: block;width: 80px;background-color: #3498db;border: none;color:
white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
ptr +=".button-on {background-color: #3498db;}\n"; ptr +=".wx{display:inline-block;margin:30px}"; ptr +="span{color:#ffffff;display:block}"; ptr +=".xc{margin: 5px 30px;width:70px;height:30px;border-radius:3px;border:0;}"; ptr +=".bt{padding: 13px 30px;height:47px;border-radius:
5px;margin:15px;padding;13px;marginleft:0;background:#3498db;color:white;border:none;cursor: pointer;font-size: 18px}";
ptr +=".wx{display:inline-block;}\n"; ptr +="</style>\n"; ptr +="</head>\n"; ptr +="<body>\n"; ptr +="<h1>ESP32 drone centre</h1>\n"; ptr +="<form action=\"/get\">\n"; ptr +="<div class=\"wx\"><span>ESC1</span><input class=\"xc\"
value=\""+String(esc1)+"\" type=\"text\" name=\"esc1\"> </div>"; ptr +="<div class=\"wx\"><span>ESC2</span><input class=\"xc\"
value=\""+String(esc2)+"\" type=\"text\" name=\"esc2\"> </div>"; ptr +="<div class=\"wx\"><span>ESC3</span><input class=\"xc\"
value=\""+String(esc3)+"\" type=\"text\" name=\"esc3\"> </div>";
ptr +="<div class=\"wx\"><span>ESC4</span><input class=\"xc\" value=\""+String(esc4)+"\" type=\"text\" name=\"esc4\"> </div>"; ptr +="<div class=\"cv\">"; ptr +="<input class=\"bt\" type=\"submit\" value=\"Start\" name='cmd'>\n"; ptr +="<input class=\"bt\" type=\"submit\" value=\"Stop\" name='cmd'>\n"; ptr +="<input class=\"bt\" type=\"submit\" value=\"Update\" name='cmd'>\n"; ptr +="<input class=\"bt\" type=\"submit\" value=\"Speed up\" name='cmd'>\n"; ptr +="<input class=\"bt\" type=\"submit\" value=\"Speed down\" name='cmd'>\n"; ptr +="</div>"; ptr +="</form></body>\n"; ptr +="</html>\n";
return ptr;
