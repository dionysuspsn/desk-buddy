# 😺 DeskBuddy LARI - Assistente de Mesa Inteligente

## 📄 Introdução
O **DeskBuddy** é um assistente de mesa inteligente desenvolvido para a **Liga Acadêmica de Robótica (LARI)** da **Unifor**. Com um design personalizado no formato de um gato, este dispositivo serve como um companheiro dinâmico para o ambiente de trabalho ou estudos. 

O projeto integra hardware e software para oferecer as seguintes funcionalidades centrais:
* [cite_start]**Relógio Analógico em Tempo Real:** Atualizado automaticamente via internet (NTP)[cite: 24, 56].
* [cite_start]**Monitor Climático:** Exibe a temperatura e umidade local consumindo dados em tempo real da API OpenWeatherMap[cite: 16, 40, 47].
* **Temporizador Pomodoro:** (Idealizado) Focado em auxiliar na produtividade e gestão de tempo do usuário.

---

## 🗂️ Estrutura dos Arquivos

O código do projeto foi modularizado para manter a organização e facilitar manutenções futuras. A estrutura está dividida em:

### 1. `Main.cpp`
[cite_start]É o núcleo do firmware[cite: 40]. Ele gerencia:
* [cite_start]A inicialização dos periféricos e a conexão Wi-Fi[cite: 52, 54].
* [cite_start]A sincronização do horário oficial através de servidores NTP (`configTime`)[cite: 56].
* [cite_start]O consumo periódico (a cada 10 minutos) dos dados meteorológicos da API OpenWeatherMap usando requisições HTTP e tratamento de dados em JSON[cite: 40, 42, 45, 59].
* [cite_start]Os loops temporizadores (cronômetros) que atualizam o relógio a cada 1 segundo sem bloquear o processamento (utilizando `millis()`)[cite: 58, 60, 62].

### 2. `Tela.cpp`
[cite_start]Contém toda a lógica gráfica e funções de renderização geométrica na tela TFT ILI9341[cite: 1]:
* [cite_start]**Design do Gato:** Desenha os olhos com efeito de gradiente circular, nariz, boca e detalhes faciais[cite: 6, 8, 9, 15].
* [cite_start]**Interface de Clima:** Renderiza os textos e valores de temperatura e umidade recebidos da internet[cite: 16, 18, 20].
* [cite_start]**Interface do Relógio:** Realiza cálculos trigonométricos baseados em radianos para mover os ponteiros de horas, minutos e segundos [cite: 26, 28, 30, 32][cite_start], além de gerenciar a memória de pixels para apagar os ponteiros antigos (evitando rastro na tela)[cite: 33, 37].

### 3. `Tela.h`
Arquivo de cabeçalho (header). [cite_start]Define os pinos de comunicação SPI entre o ESP32 e o display TFT (como `TFT_CS`, `TFT_DC`, `TFT_RST`, etc.) e declara as funções da tela para que possam ser chamadas no arquivo principal (`Main.cpp`)[cite: 38].

---

## 📚 Dependências e Bibliotecas

Para compilar este projeto com sucesso, certifique-se de ter as seguintes bibliotecas instaladas na sua IDE (Arduino IDE ou PlatformIO):

| Biblioteca | Função |
| :--- | :--- |
| **Adafruit GFX Library** | [cite_start]Biblioteca base para renderização de gráficos, formas e texto[cite: 1]. |
| **Adafruit ILI9341** | [cite_start]Driver específico para o controle do display TFT ILI9341[cite: 1]. |
| **ArduinoJson** | [cite_start]Essencial para fazer o *parse* (leitura) dos dados climáticos que a API retorna em formato JSON[cite: 40, 45]. |
| **WiFi** (Nativa do ESP32) | [cite_start]Gerencia a conexão com a rede de internet local[cite: 40, 54]. |
| **HTTPClient** (Nativa) | [cite_start]Permite efetuar requisições HTTP GET para buscar os dados do OpenWeatherMap[cite: 40, 43]. |
| **time.h** (Nativa) | [cite_start]Controla a estrutura de tempo interna e a sincronização com o servidor NTP[cite: 40, 56]. |

> [cite_start]⚠️ **Nota Importante:** O projeto requer um arquivo adicional chamado `secrets.h` (não incluso no código principal por motivos de segurança) contendo as diretivas `#define WIFI_SSID`, `#define WIFI_PASSWORD` e `#define OPENWEATHER_API_KEY`[cite: 40].

---

## 📸 Galeria do Projeto

Use esta seção para demonstrar visualmente o DeskBuddy, o circuito e a interface funcionando na tela.

### Render do Design / Case do Gato
Para adicionar uma foto, salve a imagem na pasta do seu repositório (por exemplo, crie uma pasta chamada `img` e coloque o arquivo `case.jpg` dentro) e o GitHub vai carregar a imagem automaticamente:

![Design do Case do Gato](img/case.jpg)

### Interface do Display TFT
Para adicionar a foto da tela funcionando, salve a imagem no mesmo esquema (ex: `img/tela_funcionando.jpg`):

![Interface com Rosto de Gato e Relógio](img/tela_funcionando.jpg)

---

## 🛠️ Como Executar o Projeto

1. Clone este repositório no seu computador.
2. [cite_start]Crie o arquivo `secrets.h` na raiz do projeto e configure suas credenciais de Wi-Fi e chave da API do OpenWeatherMap[cite: 40].
3. [cite_start]Certifique-se de que as conexões físicas do display batem com os pinos definidos em `Tela.h`[cite: 38].
4. Faça o upload do código para o seu ESP32.

---
<p align="center">Desenvolvido com 💙 pela Liga Acadêmica de Robótica (LARI) - Unifor</p>
