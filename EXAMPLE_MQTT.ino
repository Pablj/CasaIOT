#include "EspMQTTClient.h"

const int trigPin = 12;  // Pin de disparo del sensor ultrasónico
const int echoPin = 13;  // Pin de eco del sensor ultrasónico
EspMQTTClient client(
  "RedAbC30_2.4Gnormal",
  "Ximena55",
  "test.mosquitto.org",  // Dirección IP del servidor MQTT (broker)
  "",                  // Puede omitirse si no se necesita
  "",                  // Puede omitirse si no se necesita
  "TestClient",        // Nombre único para identificar tu dispositivo
  1883                 // Puerto MQTT, por defecto es 1883. Esta línea puede omitirse
);

void setup()
{
  Serial.begin(115200);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  // Funcionalidades opcionales de EspMQTTClient
  client.enableDebuggingMessages(); // Habilitar mensajes de depuración enviados por el puerto serie
  client.enableHTTPWebUpdater(); // Habilitar el actualizador web. Usuario y contraseña por defecto son los valores de MQTTUsername y MQTTPassword. Estos valores pueden ser modificados con enableHTTPWebUpdater("usuario", "contraseña").
  client.enableOTA(); // Habilitar actualizaciones OTA (Over The Air). La contraseña por defecto es MQTTPassword. El puerto es el puerto OTA por defecto. Puede ser modificado con enableOTA("contraseña", puerto).
  client.enableLastWillMessage("TestClient/lastwill", "Me desconecto");  // Puedes activar la bandera retain estableciendo el tercer parámetro en true
}

// Esta función se llama una vez que todo está conectado (Wifi y MQTT)
// ADVERTENCIA: DEBES IMPLEMENTARLA SI USAS EspMQTTClient
void onConnectionEstablished()
{
  // Suscribirse a "nombre_del_topico/test" y mostrar el mensaje recibido en el puerto serie
  client.subscribe("topico_pub", [](const String & payload) {
    Serial.println(payload);
  });
}

void loop()
{
  // Publicar un mensaje en "nombre_del_topico/test"
    // Obtener la distancia del sensor ultrasónico
  float distancia = sensorUltrasonico();
  char buffer[10];
  
  dtostrf(distancia, 4, 2, buffer);
  
  client.publish("topico_pub", buffer); // Puedes activar la bandera retain estableciendo el tercer parámetro en true

  client.loop();
  
  delay(1000);  // Publicar cada 5 segundos
}


float sensorUltrasonico() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Medir la duración del eco
  long duration = pulseIn(echoPin, HIGH);

  // Calcular la distancia en centímetros
  float distance = duration * 0.034 / 2;
   
  return distance;
}
