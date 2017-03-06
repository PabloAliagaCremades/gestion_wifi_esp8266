#define OK_WIFI 0 
#define ERR_TIMEOUT 1
#define ERR_FAIL_AP 2
#define ERR_TCP 3
#define ERR_TCP_ENVIO 4
#define ERR_TCP_SEND 5

class wifi
{
  private:
    SoftwareSerial *_wifiSerial;
    uint8_t _rstPin;
    long _timeout=0;
    uint8_t conectar(String *ordenesAT);
    String respuestaWifi();
  
  public:
    
    wifi(SoftwareSerial *wifiSerial,uint8_t rstPin, long timeOut,long BPS);
    uint8_t wifiActivo();
    uint8_t configurarWifi(String SSID,String pass);
    void softReset();
    uint8_t enviarStr(String HOST, int PORT, String mensaje);
  
};


wifi::wifi(SoftwareSerial *wifiSerial, uint8_t rstPin, long timeout , long BPS)
{
  _wifiSerial = wifiSerial;
  _rstPin =rstPin;
  _timeout = timeout;
  _wifiSerial->begin(BPS);
}



String wifi::respuestaWifi()
{
  String s="";
  while (_wifiSerial->available())
  {
    char c= _wifiSerial->read();
    s+=c;
  }
  return s;
}




uint8_t wifi::configurarWifi(String ssid, String pass)
{
  DBG.println(F("Creación del objeto wifi"));
  uint8_t estado=OK_WIFI;
  String ordenesAT[]=
  {
  F("AT+CWMODE=1"),//modo cliente de red
  "AT+CWJAP=\"" +  ssid +"\",\"" + pass + "\"",
  F("AT+CIPMUX=0"),//Simple conexión 
  //F("AT+CIFSR"),//ip asignada 
  F("END")
  };
  estado = conectar(ordenesAT);
  return estado;
  
}


uint8_t wifi::wifiActivo()
{
  DBG.println(F("Comando AT"));
  uint8_t estado=OK_WIFI;
  String ordenesAT[]=
  {
  F("AT"),//modo cliente de red
  F("END")
  };
  estado = conectar(ordenesAT);
  return estado;
}

uint8_t wifi::enviarStr(String HOST, int PORT, String mensaje)
{
  DBG.print(F("Enviar Texto:"));
  DBG.println(mensaje);
  uint8_t estado=OK_WIFI;
  String ordenesAT[]=
  {
    "AT+CIPSTART=\"TCP\",\"" + HOST + "\"," + String (PORT),//abrir conexion
    F("END")
  };
  estado = conectar(ordenesAT);
  if (estado==OK_WIFI)
  {
    DBG.println(F("Enlazado"));
    _wifiSerial->println("AT+CIPSEND=" + String (mensaje.length()));
    //delay(1000);
    if (_wifiSerial->find('>'))
    {
      _wifiSerial->write(mensaje.c_str());
     // _wifiSerial->write("\r\n");
      DBG.println("Enviando");
      while (_wifiSerial->available())
      {
        if (_wifiSerial->find(F("SEND OK")))
        {
          DBG.println(F("Enviado al fin"));
        }
        else
        {
          DBG.println(F("Error en el envio"));
          estado=ERR_TCP_SEND;
        }
      }
    }
    else
    {
      DBG.println(F("NOOOO enviado"));
      estado=ERR_TCP_ENVIO;
    }
    
     String s=respuestaWifi();
    DBG.print(F("Respuesta::::::"));
    DBG.println(s);
    _wifiSerial->println(F("AT+CIPCLOSE=0"));
    s=respuestaWifi();
    DBG.print(F("Respuesta::close::::"));
    DBG.println(s);
    delay(1000);
   
  }
  else
  {
    DBG.println(F("NO enlazado"));
  }
  return estado; 
  
}

uint8_t  wifi::conectar(String *ordenesAT)
{
  uint8_t estado=OK_WIFI;
  uint8_t i =0;
  while (ordenesAT[i]!=F("END"))
  {
    _wifiSerial->println(ordenesAT[i].c_str());
    delay(1000);
 //   DBG.print(F("enviado:"));DBG.println(ordenesAT[i]);
    i++;
    String s=respuestaWifi();
    DBG.print(F("recibido:"));DBG.println(s);
    long flag= millis();
    while (true)
    {
      if (s.indexOf(F("OK"))!=-1){estado=OK_WIFI; break;}
      if (s.endsWith(F("AT\r\n")))break;
      if (s.endsWith(F("no change\r\n")))break;
      if (s.endsWith(F("Linked\r\n")))break;
      if (s.endsWith(F("ALREAY CONNECT\r\n")))break;
      if (s.endsWith(F("CONNECT\r\n")))break;
      if (s.endsWith(F("busy p...\r\n"))){delay(5000);break;}
      if (s.endsWith(F("FAIL\r\n"))){estado=ERR_FAIL_AP;break;}
      if (s.indexOf(F("Unlink"))!=-1){estado=ERR_TCP;break;}
      if ( millis() - flag > _timeout){estado=ERR_TIMEOUT;break;}
      
    }//control de lo recibido y timeout

  }
  DBG.print(F("DEVOLVER ESTADO:"));
  DBG.println(estado);
  return estado;
}

void wifi::softReset()
{
  _wifiSerial->println(F("AT+RST"));
}

