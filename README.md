# Monitor de Batimentos Cardíacos com ESP32

Este projeto implementa um **monitor cardíaco** usando o **ESP32 DevKit C V4**, permitindo acompanhar os batimentos em tempo real. As informações são exibidas em um **OLED 128x64** e alertas visuais e sonoros são acionados automaticamente conforme a frequência detectada. Além disso, os dados podem ser enviados via **MQTT** para monitoramento remoto em dashboards ou aplicativos.

![Protótipo](../Imagens/Prototipo.png)

---

## Componentes do Projeto

- **Microcontrolador:** ESP32 DevKit C V4  
- **Sensor:** Pulse Sensor (conectado ao pino analógico 35)  
- **Display:** OLED 128x64 com interface I²C  
- **Indicadores visuais:**
- **Alerta sonoro:** Buzzer  
- **Comunicação:** Wi-Fi integrado do ESP32 e broker MQTT (`test.mosquitto.org`)

---

## Como Funciona?

1. **Captura do pulso:** o ESP32 lê continuamente o sinal analógico do sensor e converte em BPM (batimentos por minuto).  
2. **Classificação da frequência cardíaca:**
   - BPM < 50 → Ritmo abaixo 
   - BPM > 120 → Ritmo elevado 
3. **Alertas automáticos:**
   - Acionamento do LED  
   - Buzzer dispara para valores fora da faixa considerada normal  
4. **Exibição no OLED:** mostra em tempo real o BPM e a situação do ritmo cardíaco  
5. **Transmissão via MQTT:** os dados são publicados nos tópicos:
   - `monitor/cardiaco/valor`  
   - `monitor/cardiaco/estado`

---

## Resultados dos Testes

- **Leitura do sensor:** valores consistentes e confiáveis de BPM  
- **LED:** acende corretamente conforme o número do BPM  
- **Buzzer:** alerta sonoro funciona para BPM fora do normal  
- **Display OLED:** informações exibidas de forma clara e atualizadas em tempo real  
- **MQTT:** conexão estável e envio correto dos dados para monitoramento remoto

---

## Referências

- [Biblioteca Adafruit SSD1306 para OLED](https://github.com/adafruit/Adafruit_SSD1306)  
- [PubSubClient MQTT Library para Arduino](https://pubsubclient.knolleary.net/)  
- [ESP32 DevKit-C V4 – Documentação Técnica](https://www.espressif.com/)  
- [HiveMQ – Documentação do Broker MQTT](https://www.hivemq.com/)
