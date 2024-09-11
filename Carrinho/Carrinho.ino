/*
  ***** Mac do Carrinho a8:42:e3:45:ae:0c Proboard Vermelho
  Mac do Controle a8:42:e3:48:37:50 Protoboard Verde 
*/
#include <WiFi.h>
#include <esp_wifi.h>
#include <esp_now.h>
#include <ESP32Servo.h>

typedef struct Estrutura {
  int Acelerador;
  int Direcao;
  bool Led;
} Estrutura;

Estrutura dadosRecebidos;
Servo esc;

const int PinoControleMotor = 33;
const int freq = 60;
const int escChannel = 0;
const int resolution = 12;
int velocidad = 245;


void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&dadosRecebidos, incomingData, sizeof(dadosRecebidos));
  Serial.print("Dados Recebidos: ");
  Serial.println(len);
  Serial.print("Aceleração: ");
  Serial.println(dadosRecebidos.Acelerador);
  Serial.print("Direção: ");
  Serial.println(dadosRecebidos.Direcao);
  Serial.print("Led ");
  Serial.println(dadosRecebidos.Led);
  if(dadosRecebidos.Led)
  {
    digitalWrite(12, HIGH);
  }else{
    digitalWrite(12, LOW);
  }
 
  int mmotorSpeed =  map(dadosRecebidos.Acelerador, 1700, 4095, 0, 50);
  esc.write(mmotorSpeed);
  Serial.println(mmotorSpeed);
  Serial.println();
}

void readMacAddress(){
  uint8_t baseMac[6];
  esp_err_t ret = esp_wifi_get_mac(WIFI_IF_STA, baseMac);
  if (ret == ESP_OK) {
    Serial.printf("%02x:%02x:%02x:%02x:%02x:%02x\n",
                  baseMac[0], baseMac[1], baseMac[2],
                  baseMac[3], baseMac[4], baseMac[5]);
  } else {
    Serial.println("Failed to read MAC address");
  }
}

void setup(){
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.STA.begin();
  pinMode(12, OUTPUT);
  esc.attach(33,  1000, 2000);
  esc.write(0);

  analogWrite(33,15);
  Serial.print("Carrinho, Protoboard Vermelho MAC Address: ");readMacAddress();
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
}
 
void loop(){
  
}