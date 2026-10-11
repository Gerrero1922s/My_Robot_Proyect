#include <Servo.h>

#include <WiFiS3.h>
#include <UnoR4WiFi_WebServer.h>

const char SSID[] = "Desousa";
const char PASSWORD[] = "sebas132?";
int status = WL_IDLE_STATUS;

UnoR4WiFi_WebServer server;
UnoR4WiFi_WebSocket* webSocket;

Servo myservo;
int input1 = 8; //Pin 1
int input2 = 7; // Pin 0
int input3 = 11; // PWN
int speed = 0;

void onStartSocket(net::WebSocket& ws){
  Serial.print("New WebSocket Connection");
}

void onMessageSocket(net::WebSocket& ws, const net::WebSocket::DataType dataType, const char* message, uint16_t length){
  Serial.print("[WebSocket] Received (");
  Serial.print(length);
  Serial.print(" bytes): ");
  Serial.println(message);


  String msgStr = String(message);
  String response = "";
  
  // Command processing
  if (msgStr.equalsIgnoreCase("ping")) {
    response = "pong";
  } else {
    response = "Echo: " + msgStr;
  }

  //Movimiento hacia los lados
  String velocidad = msgStr.substring(msgStr.indexOf("V") + 2, msgStr.indexOf("R"));
  String rotation = msgStr.substring(msgStr.indexOf("R") + 2);
  
  MotorSis(velocidad.toInt(),rotation.toInt());

  if (webSocket != nullptr) {
    webSocket->broadcastTXT(response);
  }
}

void onClientClose(net::WebSocket& ws, const net::WebSocket::CloseCode code, const char* reason, uint16_t length){
   Serial.print("WebSocket client disconnected by: ");
   Serial.println(reason);
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  myservo.attach(10);

  pinMode(input1,OUTPUT);
  pinMode(input2,OUTPUT);
  pinMode(input3,OUTPUT);

  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.print("Modulo No Dectetados");
    Serial.println("O esta fallando");
    while(true);
  }

  while (status != WL_CONNECTED) {
    Serial.print("Conectandose a: ");
    Serial.println(SSID);
    status = WiFi.begin(SSID, PASSWORD);
    delay(4500);
  } 
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  server.begin();
  webSocket = server.enableWebSocket(81);

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

  digitalWrite(input1, 0);
  digitalWrite(input2, 1);

}

void loop() {
  // put your main code here, to run repeatedly:

  analogWrite(input3, speed);
  server.handleWebSocket();
  delay(30);
}

void MotorSis(int V, int R) {

  if (V < 0){
    speed = abs(V);
    speed = (speed > 200) ? 200 : speed; 
    digitalWrite(input1, 1);
    digitalWrite(input2, 0);

  }else {
    speed = (V > 200) ? 200 : V;
    digitalWrite(input1, 0);
    digitalWrite(input2,1);
  }

  R = abs(R);
  if ( R > 180){ 
    R = 180;
  }

  myservo.write(R);
  
}