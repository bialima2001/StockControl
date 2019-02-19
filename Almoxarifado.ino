#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <MFRC522.h>
#include <SPI.h>
#include <Wire.h>

#define Pin_SDA 4
#define Pin_Reset 5

HTTPClient http;

const char* SSID = "iPhone de Bianca";
const char* PASS = "bianca123";


const String LOCATION = "https://stokcontrol.herokuapp.com/upload";
const String UPLOAD = "upload";
const String DOWNLOAD = "download";


const String CADASTRO = "\"CADASTRO\":";
const String TAG = "\"TAG\":";
const String DISPO = "\"DISPO\":";
const String NOME = "\"NOME\":";
String leitura, pacote = "";
byte caractere;

void uploadDados(String p);
void  dividir(String p);
void jsonDados(int x, String n);

MFRC522 RFID (Pin_SDA, Pin_Reset);

void setup() {

  Wire.begin();
  Serial.begin(115200);
  SPI.begin();
  RFID.PCD_Init();
   WiFi.begin(SSID, PASS);
   while (WiFi.status() != WL_CONNECTED) {
   delay(500);
   Serial.println(" Aguardando conexão");
   }
   if(WiFi.status() == WL_CONNECTED){
    Serial.println("conectado");
   }
   
}

void loop() {
  leitura = "";
  if(Serial.available() >0){
    pacote = Serial.readStringUntil('\n');
    if(pacote.startsWith("{") and pacote.endsWith("}")){
      dividir(pacote);
    }
  }else if (!RFID.PICC_IsNewCardPresent() or !RFID.PICC_ReadCardSerial()){
    
    return;
  } 
    
  for (byte i=0; i<RFID.uid.size; i++){
    leitura +=
    (RFID.uid.uidByte[i] < 0x10 ? "0" : "") +
    String(RFID.uid.uidByte[i], HEX) +
    (i!=3 ? ":" : "");
  }
  leitura.toUpperCase();
  Serial.print("{"+leitura+"}");
  delay(1000);
  
}
void uploadDados(String p){

 http.begin(LOCATION);
 http.addHeader("Content-Type", "application/json");
 http.POST(p);
 http.end();
 }
 
 void dividir(String p){
  int aux=1, pos=0;
  String temp="";
  String json="";
  for(int i=0; p[i]!='}'; i++){
    if(p[i]==','){
      temp = p.substring(aux,i);
      if(pos == 0){
        json = "{"+CADASTRO+"1,"+ NOME + temp + ",";
      }else if(pos ==1){
        json = json + TAG + temp+",";
        aux = i+1; 
    }
    pos++;
    }else if(pos == 2){
      temp = p.substring(aux);
      json = json + DISPO + temp+"}";
      uploadDados(json);
      Serial.print(json);
    }
     
    
  }
  
 }
 void jsonDados(int x, String n){
  String aux = "";
  if(x == 0){
    aux = "{"+NOME+n+"}";
  }else if(x == 1){
    aux = "{"+TAG+n+"}";
  }else if(x == 2){
    aux = "{"+DISPO+n+"}";
  }
  Serial.print(aux);
  uploadDados(aux);
 }
 

