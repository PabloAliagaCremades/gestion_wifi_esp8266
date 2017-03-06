# gestion_wifi_esp8266


#Módulo de gestion WIFI ESP8266 con genuino

Después de muchas versiones sobre la gestión del módulo wifi ESP8266, he tratado de hacer una librería lo más simple y sencilla posible. 
Está basada en un objeto **wifi** que gestiona la conexión y el envío de mensajes.
Esta primera versión solo es un sistema cliente, que envía los datos a un servidor.



> USO: **Creación del objeto**
> - Funciona con la librería **SoftwareSerial**
> - Creación del objeto :wifi(SoftwareSerial *wifiSerial,uint8_t rstPin, long timeout,long BPS);
> - **wifiSerial** Se necesita un puntero a la dirección del **SoftwareSerial** creado
> - **rstPin** Pin al que va conectado el reset
> - **timeout** Tiempo de espera para dar un error por timeout
> - **BPS** velocidad en BPS con la que se trabaja, de normal 9600;


> USO: **Conexión al punto de acceso**
> - uint8_t configurarWifi(String SSID,String pass);
> - **SSID**: Cadena de texto con el identificador de la red
> - **pass** : Cadena de texto con la password de la red
> - **return** : Devuelve un entero con el código del resultado. 


> USO: **envio de un mensaje**
> - uint8_t enviarStr(String HOST, int PORT, String mensaje)
> - **HOST** : Cadena de texto con la "IP" del servidor
> - **PORT** : Entero para indicar el puerto 
> - **mensaje** : Mensaje a enviar. 
> - **return** : Devuelve un entero con el código del resultado. 


> USO: **Control de error**
> - Los errores están codificados de la siguiente manera
> - #define OK_WIFI 0 
> - #define ERR_TIMEOUT 1
> - #define ERR_FAIL_AP 2
> - #define ERR_TCP 3
> - #define ERR_TCP_ENVIO 4
> - #define ERR_TCP_SEND 5
