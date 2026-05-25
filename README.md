# 😺 DeskBuddy LARI - Assistente de Mesa Inteligente

## 📄 Introdução
O **DeskBuddy** é um assistente de mesa inteligente desenvolvido para a **Liga Acadêmica de Robótica (LARI)** da **Unifor**. Com um design personalizado no formato de um gato, este dispositivo serve como um companheiro dinâmico para o ambiente de trabalho ou estudos. 

O projeto integra hardware e software para oferecer as seguintes funcionalidades centrais:
* **Relógio Analógico em Tempo Real:** Atualizado automaticamente via internet (NTP).
* **Monitor Climático:** Exibe a temperatura e umidade local consumindo dados em tempo real da API OpenWeatherMap.
* **Temporizador Pomodoro:** (Idealizado) Focado em auxiliar na produtividade e gestão de tempo do usuário.

---

## 🗂️ Estrutura dos Arquivos

O código do projeto foi modularizado para manter a organização e facilitar manutenções futuras. A estrutura está dividida em:

### 1. `Main.cpp`
É o núcleo do firmware. Ele gerencia:
* A inicialização dos periféricos e a conexão Wi-Fi.
* A sincronização do horário oficial através de servidores NTP (`configTime`).
* O consumo periódico (a cada 10 minutos) dos dados meteorológicos da API OpenWeatherMap usando requisições HTTP e tratamento de dados em JSON.
* Os loops temporizadores (cronômetros) que atualizam o relógio a cada 1 segundo sem bloquear o processamento (utilizando `millis()`).

### 2. `Tela.cpp`
Contém toda a lógica gráfica e funções de renderização geométrica na tela TFT ILI9341:
* **Design do Gato:** Desenha os olhos com efeito de gradiente circular, nariz, boca e detalhes faciais.
* **Interface de Clima:** Renderiza os textos e valores de temperatura e umidade recebidos da internet.
* **Interface do Relógio:** Realiza cálculos trigonométricos baseados em radianos para mover os ponteiros de horas, minutos e segundos, além de gerenciar a memória de pixels para apagar os ponteiros antigos (evitando rastro na tela).

### 3. `Tela.h`
Arquivo de cabeçalho (header). Define os pinos de comunicação SPI entre o ESP32 e o display TFT (como `TFT_CS`, `TFT_DC`, `TFT_RST`, etc.) e declara as funções da tela para que possam ser chamadas no arquivo principal (`Main.cpp`).

---

## 📚 Dependências e Bibliotecas

Para compilar este projeto com sucesso, certifique-se de ter as seguintes bibliotecas instaladas na sua IDE (Arduino IDE ou PlatformIO):

| Biblioteca | Função |
| :--- | :--- |
| **Adafruit GFX Library** | Biblioteca base para renderização de gráficos, formas e texto. |
| **Adafruit ILI9341** | Driver específico para o controle do display TFT ILI9341. |
| **ArduinoJson** | Essencial para fazer o *parse* (leitura) dos dados climáticos que a API retorna em formato JSON. |
| **WiFi** (Nativa do ESP32) | Gerencia a conexão com a rede de internet local. |
| **HTTPClient** (Nativa) | Permite efuar requisições HTTP GET para buscar os dados do OpenWeatherMap. |
| **time.h** (Nativa) | Controla a estrutura de tempo interna e a sincronização com o servidor NTP. |

> ⚠️ **Nota Importante:** O projeto requer um arquivo adicional chamado `secrets.h` (não incluso no código principal por motivos de segurança) contendo as diretivas `#define WIFI_SSID`, `#define WIFI_PASSWORD` e `#define OPENWEATHER_API_KEY`.

---

## 📸 Galeria do Projeto


### Render do Design / Case do Gato

![Design do Case do Gato](img/case.jpg)

### Interface do Display TFT

![Interface com Rosto de Gato](img/imagem_2026-05-25_165541235.png)

---

## 🛠️ Como Executar o Projeto

1. Clone este repositório no seu computador.
2. Crie o arquivo `secrets.h` na raiz do projeto e configure suas credenciais de Wi-Fi e chave da API do OpenWeatherMap.
3. Certifique-se de que as conexões físicas do display batem com os pinos definidos em `Tela.h`.
4. Faça o upload do código para o seu ESP32.

---
<p align="center">Desenvolvido pela Equipe Edgerunners - Unifor</p>
