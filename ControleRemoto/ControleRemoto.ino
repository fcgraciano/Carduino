/*
  Mac do Carrinho a8:42:e3:45:ae:0c Proboard Vermelho
  ***  Mac do Controle a8:42:e3:48:37:50 Protoboard Verde 
*/
#include <WiFi.h>
#include <esp_wifi.h>
#include <esp_now.h>

//MAC CARRINHO
uint8_t CarrinhoMAC[] = {0xA8, 0x42, 0xE3, 0x45, 0xAE, 0x0C};

typedef struct Estrutura {
  int Acelerador;
  int Direcao;
  bool Led;
} Estrutura;

Estrutura dadosEnviados;

esp_now_peer_info_t Carrinho;




void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nStatus do ultimo pacote enviado:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Successo" : "Falha");
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
  
  Serial.print("Controle, Protoboard Verde MAC Address: ");readMacAddress();
  if (esp_now_init() != ESP_OK) {
    Serial.println("Erro na inicialização ESP-NOW");
    return;
  }
  esp_now_register_send_cb(OnDataSent);
  memcpy(Carrinho.peer_addr, CarrinhoMAC, 6);
  Carrinho.channel = 0;  
  Carrinho.encrypt = false;
  if (esp_now_add_peer(&Carrinho) != ESP_OK){
    Serial.println("Falha ao adicionar o Carrinho");
    return;
  }
}
 
void loop(){
  
  dadosEnviados.Acelerador = analogRead(0);
  dadosEnviados.Direcao = analogRead(1);
  dadosEnviados.Led = !dadosEnviados.Led;
Serial.println(analogRead(0));
Serial.println(analogRead(1));

  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(CarrinhoMAC, (uint8_t *) &dadosEnviados, sizeof(dadosEnviados));

  if (result == ESP_OK) {
    Serial.println("Enviado com sucesso");
  }
  else {
    Serial.println("Erro ao enviar");
  }
  //delay(2000);
   
}