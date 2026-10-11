#include <Servo.h>

#include <WiFiS3.h>
#include <UnoR4WiFi_WebServer.h>
#include "web_html.h"

const char SSID[] = "moto";
const char PASSWORD[] = "123456789";
int status = WL_IDLE_STATUS;

UnoR4WiFi_WebServer server(80);
UnoR4WiFi_WebSocket* webSocket;

Servo myservo;
int direcion1 = 8; //Pin 1
int direcion2 = 7; // Pin 0

int speed = 0;
int rota = 0;

bool motion = false;
int lastMessageTime = 0;
const int time = 200;


int servoM = 10;

void onStartSocket(net::WebSocket& ws){
  Serial.print("New WebSocket Connection");
}

void onMessageSocket(net::WebSocket& ws, const net::WebSocket::DataType dataType, const char* message, uint16_t length){

  motion = true;
  lastMessageTime = millis();
  String msgStr = String(message);
  String response = "";
  
  //Movimiento hacia los lados
  
  String velocidad = msgStr.substring(msgStr.indexOf("V") + 2, msgStr.indexOf("R"));
  String rotation = msgStr.substring(msgStr.indexOf("R") + 2);
  
  speed = velocidad.toInt();
  rota = rotation.toInt();


}

void onClientClose(net::WebSocket& ws, const net::WebSocket::CloseCode code, const char* reason, uint16_t length){
   Serial.print("WebSocket client disconnected by: ");
   Serial.println(reason);
}

void handleHome(WiFiClient& client, const String& method, const String& request, const QueryParams& params, const String& jsonData) {
  server.sendResponse(client, htmlPage);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  while(!Serial);

  myservo.attach(servoM);

  pinMode(direcion1,OUTPUT);
  pinMode(direcion2,OUTPUT);
  //  pinMode(speed3,OUTPUT);

  status = WiFi.beginAP(SSID, PASSWORD);

  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.print("Modulo No Dectetados");
    Serial.println("O esta fallando");
    while(true);
  }

  while (status != WL_AP_LISTENING) {
    Serial.println("Failt to create the conextion");
    status = WiFi.status();
    delay(4500);

  } 
  delay(1000);
  
  server.addRoute("/", handleHome);

  webSocket = server.enableWebSocket(81);

  server.begin();


  if (webSocket != nullptr) {
    // Set up WebSocket event handlers
    webSocket->onOpen(onStartSocket);
    webSocket->onMessage(onMessageSocket);
    webSocket->onClose(onClientClose);
    
    Serial.println("WebSocket server started on port 81");
  } else {
    Serial.println("Failed to start WebSocket server");
    while(true);
  }

  Serial.print("WebSocket: ws://");
  Serial.print(WiFi.localIP());
  Serial.println(":81");

}

void loop() {

  sist();
  while (motion){
    sist();
    if ( millis() - lastMessageTime > time){
      motion = false;
    }
    MotorSis(speed, rota);
    delay(30);
  }
  MotorSis();
  delay(30);
}

void sist(){
    server.handleWebSocket();
    server.handleClient();
}

void MotorSis(int V = 0, int R = 90) { // V = Velocidad, R = Rotacion o grado de Servo 

  if (V < 0){
 
    digitalWrite(direcion1, 1);
    digitalWrite(direcion2, 0);

  }else if (V > 0){
   
    digitalWrite(direcion1, 0);
    digitalWrite(direcion2,1);
  }else{
    digitalWrite(direcion1, 0);
    digitalWrite(direcion2, 0);
  }

  R = abs(R);
  if ( R > 180){ 
    R = 180;
  }

  myservo.write(R);
}