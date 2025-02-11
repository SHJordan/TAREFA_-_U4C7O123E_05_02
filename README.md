# Controle de Servomotor por PWM com Raspberry Pi Pico W


https://github.com/user-attachments/assets/9969eb87-6c70-4a54-aa09-7a7d82452112


Este projeto demonstra o controle de um servomotor por PWM utilizando o microcontrolador RP2040 (Raspberry Pi Pico W) com o Pico SDK v2.1.0. A simulação foi realizada no ambiente online ([Wokwi](https://wokwi.com/projects/422461241937890305)), utilizando também um LED RGB (GPIO 12) para evidenciar a aplicação de PWM em modulação de brilho.

## Funcionalidades

- **PWM na GPIO 22:**  
  Configurado para operar a ~50 Hz (período de 20 ms), permitindo gerar pulsos de controle para o servomotor.

- **Posicionamento do Servomotor:**  
  - 180° (2.400 µs) – aguarda 5 segundos  
  - 90° (1.470 µs) – aguarda 5 segundos  
  - 0° (500 µs) – aguarda 5 segundos  

- **Movimentação Suave:**  
  Após os posicionamentos iniciais, o servo realiza um movimento contínuo entre 0° e 180°, com incremento de 5 µs a cada 10 ms.

- **Efeito LED RGB (GPIO 12):**  
  Um PWM separado é configurado para o LED, fazendo com que o brilho varie (fade) continuamente durante o movimento do servo.

## Instruções de Compilação e Simulação

1. **Clone o Repositório:**
   ```bash
   git clone https://github.com/SHJordan/TAREFA_-_U4C7O123E_05_02
