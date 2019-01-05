

/*
 dado         |   variavel de controle

 latitude     |   0
 longitude    |   1
   idade info   |   2
   dia          |   3
   mês          |   4
   ano          |   5
   hora         |   6
   minuto       |   7
   segundo      |   8
   altitude     |   9
   luminosidade |   10
   FAZER PROGRAMAS DE GRAVAÇÃO PRA CADA VARIAVEL
 */
#include "DHT.h"
#define DHTPIN 4    // what digital pin we're connected t
#define DHTTYPE DHT22
#include<SD.h>
#include <SPI.h>
#include <SoftwareSerial.h>
#include <TinyGPS.h>
DHT dht(DHTPIN, DHTTYPE);

SoftwareSerial serial1(6, 7); // RX, TX
TinyGPS gps1;
File myFile;
const int MPU=0x68;  


void setup(){
   serial1.begin(9600);
   dht.begin();
   Serial.begin(9600);
   Serial.print("testando"); 
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
   if (!SD.begin(10)) {
    Serial.println("initialization failed!");
    return;
  }
}


void loop() {
gps();
}


void gps(){
  
  for(int ik=0; ik<10000; ik++){
  for(int s=0; s<10000; s++);
  }
  
  bool recebido = false;
  while (serial1.available()) {
     char cIn = serial1.read();
     recebido = gps1.encode(cIn);
  }
if (recebido) {
    Serial.println("----------------------------------------");
     
    //Latitude e Longitude
    long latitude, longitude;
    float lat, lon;
    unsigned long idadeInfo;
    gps1.get_position(&latitude, &longitude, &idadeInfo);     

     if (latitude != TinyGPS::GPS_INVALID_F_ANGLE) {
        lat = float(latitude)/100000;
        Serial.print("latitude: ");
     Serial.println(lat, 6); 
        passar_GMT(lat);
     }

     if (longitude != TinyGPS::GPS_INVALID_F_ANGLE) {
       lon = float(longitude) / 100000;
     Serial.print("Longitude: ");
     Serial.println(lon, 6); 
       passar_GMT(lon);
        }

     if (idadeInfo != TinyGPS::GPS_INVALID_AGE) {
       Serial.print("Idade da Informacao (ms): ");
       Serial.println(idadeInfo);
       gravar_idadeInfo(idadeInfo);
     }


     //Dia e Hora
     int ano;
     byte mes, dia, hora, minuto, segundo, centesimo;
     gps1.crack_datetime(&ano, &mes, &dia, &hora, &minuto, &segundo, &centesimo, &idadeInfo);
      Serial.print("Data (GMT): ");
      Serial.print(dia);
      gravar_dia(dia);
      Serial.print("/");
      Serial.print(mes);
      gravar_mes(mes);
     // Serial.print("/");
      gravar_ano(ano);
      //Serial.println(ano);
      //Serial.print("Horario (GMT): ");
      //Serial.print(hora);
      gravar_hora(hora);
      //Serial.print(":");
      //Serial.print(minuto);
      gravar_minuto(minuto);
      //Serial.print(":");
      //Serial.println(segundo);
      gravar_segundos(segundo);
 
      //altitude
      float altitudeGPS;
      altitudeGPS = gps1.f_altitude();

     if ((altitudeGPS != TinyGPS::GPS_INVALID_ALTITUDE) && (altitudeGPS != 1000000)) {
//       Serial.print("Altitude (cm): ");
//       Serial.println(altitudeGPS);
       gravar_Altitude(altitudeGPS);
     }


  dht_read();
  luz();
}
}
void luz(){
   float lumi, lumipor, luminosidade;
  lumi=analogRead(A0);
  if (lumi==0){
   luminosidade = 0; 
  }
  else{
 lumipor= 1000/lumi;
  if(lumipor>100){
   luminosidade = 100; 
    } 
    else{
   luminosidade = lumipor;
}

}
//Serial.print("luminosidade: ");
//Serial.print(luminosidade);
//Serial.println("%");
//Serial.println("--------------------------------");
gravar_luminosidade(luminosidade);
}


  
void passar_GMT(float coord){
  float seg, mi = coord - int(coord);
        mi = mi * 60;
        seg = mi - int (mi);
        seg = seg * 60;
    gravar_GMT(seg, mi, coord);
}

void gravar_GMT(float seg, float mi, float coord){
  myFile = SD.open("dados.txt", FILE_WRITE);
  myFile.print("Latitude: ");
  myFile.print(int(coord));
  myFile.print("°");
  myFile.print(int(mi)*-1);
  myFile.print(" ' ");
  myFile.print(seg*-1);
    myFile.println(" '' ");
  myFile.close();
}
void gravar_lon(float lon){
  myFile = SD.open("dados.txt", FILE_WRITE);
  myFile.print("Longitude: ");
  myFile.println(lon, 6);
  myFile.close();
}
void gravar_idadeInfo(unsigned long idadeInfo){
  myFile = SD.open("dados.txt", FILE_WRITE);
  myFile.print("Idade da Infomacao: ");
  myFile.println(idadeInfo);
  myFile.close();
}
void gravar_dia(byte dia){
  myFile = SD.open("dados.txt", FILE_WRITE);
  myFile.print("DATA (GMT): ");
  myFile.print(dia);
  myFile.print("/");
  myFile.close();
}
void gravar_mes(byte mes){
  myFile = SD.open("dados.txt", FILE_WRITE);
  myFile.print(mes);
  myFile.print("/");
  myFile.close();
}
void gravar_ano(int ano){
  myFile = SD.open("dados.txt", FILE_WRITE);
  myFile.println(ano);
  myFile.close();
}
void gravar_hora(byte hora){
  myFile = SD.open("dados.txt", FILE_WRITE);
  myFile.print("HORARIO (GMT): ");
  hora=hora-3;
  myFile.print(hora);
  myFile.print(":");
  myFile.close();
}
void gravar_minuto(byte minuto){
  myFile = SD.open("dados.txt", FILE_WRITE);
  myFile.print(minuto);
  myFile.print(":");
  myFile.close();
}
void gravar_segundos(byte segundos){
  myFile = SD.open("dados.txt", FILE_WRITE);
  myFile.println(segundos);
  myFile.close();
}
void gravar_Altitude(float Altitude){
  myFile = SD.open("dados.txt", FILE_WRITE);
  myFile.print("Altitude (cm): ");
  myFile.println(Altitude);
  myFile.close();
}
void gravar_luminosidade(float luminosidade){
    myFile = SD.open("dados.txt", FILE_WRITE);
    myFile.print("Luminosidade: ");
    myFile.print(luminosidade);
    myFile.println("%");
    myFile.println("----------------------------------------");
    myFile.close();
}
void gravar_temperatura(float temperatura){
  myFile = SD.open("dados.txt", FILE_WRITE);
  myFile.print("Temperatura: ");
  myFile.print(temperatura);
  myFile.println(" °C"); 
  myFile.close();
}
void gravar_humidade(float humidade){
  myFile = SD.open("dados.txt", FILE_WRITE);
  myFile.print("Humidade: ");
  myFile.print(humidade);
  myFile.println("%");
  myFile.close();
}


void dht_read(){
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  //Serial.print("Humidity: ");
  //Serial.print(h);
  //Serial.print(" %\t");
  gravar_humidade(h);
  //Serial.print("Temperature: ");
  //Serial.print(t);
  //Serial.print(" *C ");
  gravar_temperatura(t);
  //Serial.print(f);
  //Serial.print(" *F\t");
  //Serial.print("Heat index: ");
  //Serial.print(hic);
  //Serial.print(" *C ");
  //Serial.print(hif);
  //Serial.println(" *F");
}

