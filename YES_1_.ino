#include <CTBot.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#define pinled 4
#define pinbuzzer 14

//sediakan object untuk CTbot
CTBot myBot ;

int pinsensor = A0;
float outputSensor;
float sensorAsap;
//rule
float asap [4];
float rule0,rule1,rule2,rule3;
float rule [4];
float defuz, pwm;
int defuzzy;
float temp;
//konfigurasi WiFi
String ssid = "OTANG HOUSE";
String pass = "12345678";

int sensorr = 0;
//KONFIGURASI TELEGRAM
String token = "2133911040:AAGqMPNX3jbSXYYDPtPzZ8rerSpzIqSsbn0";
const int id = 1780598208 ;

void setup() {
  Serial.begin(9600);
  pinMode(pinsensor, INPUT);
  pinMode(pinled, OUTPUT);
  pinMode(pinbuzzer, OUTPUT);

  //koneksikan nodemcu ke telegram melalui WiFi
  myBot.wifiConnect(ssid, pass);
  myBot.setTelegramToken(token);

  //uji apakah berhasil terkoneksi atau tidak
  
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, pass);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    digitalWrite(pinled,LOW  );
  }
  Serial.println("");
  Serial.println("WiFi connected");
  digitalWrite(pinled,HIGH);
 
  
  
}

void loop() {
  float sensorAsap = outputSensor;
  
  //fuzzyfikasi
  FuzzyAsap(sensorAsap);

  //defuzzyfikasi
  Defuzzy();
  outputSensor= analogRead(pinsensor);
  outputSensor = map(outputSensor,0,1023,0,250);
  delay(6000);

  Serial.println(sensorAsap);
  Serial.println(rule0);
  Serial.println(rule1);
  Serial.println(rule2);
  Serial.println(rule3);
  Serial.println(defuzzy);

  
}

void FuzzyAsap(float sensorAsap){
  //untuk kondisi mati
  if (sensorAsap >= 99)
  {
    asap [0] = 0;
  }
  else if (sensorAsap <= 99)
  {
    asap [0] = 1;
  }
  //untuk terdeteksi rendah
  if (sensorAsap >= 150)
  {
    asap [1] = 0;
  }
  else if (sensorAsap >= 100 && sensorAsap <= 150)
  {
    asap [1] = (150-sensorAsap)/(150-100);
    
  }
  //untuk terdeteksi sedang
  if (sensorAsap <= 140 || sensorAsap >= 200)
  {
    asap [2] = 0;
  }
  else if (sensorAsap >= 140 && sensorAsap <= 170)
  {
    asap [2] = (sensorAsap-140)/(170-140);
  }
  else if (sensorAsap >= 170 && sensorAsap <= 200)
  {
    asap [2] = (200-sensorAsap)/(200-170);
  }
  //untuk terdeteksi tinggi
  if(sensorAsap <= 190)
  {
    asap [3] = 0;
  }
  else if (sensorAsap >= 190 && sensorAsap <= 250)
  {
    asap [3] = (sensorAsap-190)/(250-190);
  }
  
  else if (sensorAsap >=251)
  {
    asap [2] = 1;
  }
  
}
void Rule(){
  int i;
    for ( i=0; i<=3; i=i+1)
    {
        temp =(asap[i]);
        rule [i]= temp; 
    }
    rule0 = rule[0]; //mati
    rule1 = rule[1]; //rendah
    rule2 = rule[2]; //sedang
    rule3 = rule[3]; //tinggi
}
void Defuzzy(){
  Rule();
  pwm = (rule0*1)+(rule1*2)+(rule2*3)+(rule3*4);

  defuz = 0;
  int i;
  for ( i=0; i<=3; i=i+1)
  {
    defuz = defuz + rule [i];
  }
  defuzzy = pwm / defuz;
  if (defuzzy == 1){
    digitalWrite(pinbuzzer, LOW);
    myBot.sendMessage(id, "Ruangan Bersih");
    String temp = "Kondisi Udara : ";
    temp += int(outputSensor);
    temp += " PPM";
    myBot.sendMessage(id, temp);
    digitalWrite(pinbuzzer, LOW);
 }
  else if (defuzzy == 2){
    myBot.sendMessage(id, "Hati Hati Terdeteksi Asap Di Ruangan Kadar Rendah");
    String temp = "Kondisi Udara : ";
    temp += int(outputSensor);
    temp += " PPM";
    myBot.sendMessage(id, temp);
    digitalWrite(pinbuzzer, HIGH);
    delay(1000);
    digitalWrite(pinbuzzer, LOW);
    delay(500);
    digitalWrite(pinbuzzer, HIGH);
    delay(1000);
    digitalWrite(pinbuzzer, LOW);
    
    
 }
  else if (defuzzy == 3){
    myBot.sendMessage(id, "Hati Hati Terdeteksi Asap Di Ruangan Kadar Sedang");
    String temp = "Kondisi Udara : ";
    temp += int(outputSensor);
    temp += " PPM";
    myBot.sendMessage(id, temp);
    digitalWrite(pinbuzzer, HIGH);
    delay(1000);
    digitalWrite(pinbuzzer, LOW);
    delay(500);
    digitalWrite(pinbuzzer, HIGH);
    delay(1000);
    digitalWrite(pinbuzzer, LOW);
    delay(500);
    digitalWrite(pinbuzzer, HIGH);
    delay(1000);
    digitalWrite(pinbuzzer, LOW);
    
 }
  else if (defuzzy == 4){
    myBot.sendMessage(id, "Hati Hati Terdeteksi Asap Di Ruangan Kadar Tinggi");
    String temp = "Kondisi Udara : ";
    temp += int(outputSensor);
    temp += " PPM";
    myBot.sendMessage(id, temp);
    digitalWrite(pinbuzzer, HIGH);
    delay(1000);
    digitalWrite(pinbuzzer, LOW);
    delay(500);
    digitalWrite(pinbuzzer, HIGH);
    delay(1000);
    digitalWrite(pinbuzzer, LOW);
    delay(500);
    digitalWrite(pinbuzzer, HIGH);
    delay(1000);
    digitalWrite(pinbuzzer, LOW);
    delay(500);
    digitalWrite(pinbuzzer, HIGH);
    delay(1000);
    digitalWrite(pinbuzzer, LOW);
    
 }
}
