#include <SoftwareSerial.h>


#define RXPIN 2
#define TXPIN 3
#define RSTPIN 4


#define DBG Serial
SoftwareSerial wifiSerial(RXPIN,TXPIN);
#include "wifi.h"

wifi miWifi(&wifiSerial,RSTPIN,10000,9600);

#define runEvery(t) for (static uint32_t _lasttime;(uint32_t)((uint32_t)millis()- _lasttime) >=(t); _lasttime+=(t))


void setup() 
{
  DBG.begin(9600);
  
  if (miWifi.wifiActivo()==OK_WIFI)
  {
    miWifi.configurarWifi("DELABS","dellmacmolaano");  
    /*DBG.println(miWifi.enviarStr("192.168.2.165",2000,F("Hola mundo\n")));
    DBG.println(miWifi.enviarStr("192.168.2.165",2000,F("que tal el día\n")));
    DBG.println(miWifi.enviarStr("192.168.2.165",2000,F("ahora esto va mucho mejor")));
    */
  }
  else
  {
    DBG.println(F("Error en el wifi"));
  }

}

void loop() 
{
  runEvery(60000)
  {
    String cuerpo = "name=Pablo_wifi&value=18" ;
    String lonPost = String(cuerpo.length());
  // Serial.println (F("cuerpo_parte_1"));
  // Serial.println (cuerpo); 
  // Serial.println (lonPost);
  
 
  String post = ("POST / HTTP/1.1\r\nContent-Length:") + lonPost + "\r\n\r\n"+ cuerpo;

    if (miWifi.enviarStr("193.146.117.35",7000,post))
    //if (miWifi.enviarStr("192.168.2.165",2000,post))
    {
      DBG.println("Enviado");
    }
    else
    {
      DBG.println("NOOOO");
    }
  }

}
