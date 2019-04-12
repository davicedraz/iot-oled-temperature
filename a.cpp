#include <Wire.h>
#include "SSD1306Wire.h"

//Pinos do NodeMCU
// SDA => D1
// SCL => D2

// Inicializa o display Oled
SSD1306Wire display(0x3c, D1, D2);

void setup()
{
    Serial.begin(115200); //Baudrate
    display.init();
    display.flipScreenVertically();
}

void telainicial()
{
    //Apaga o display
    display.clear();
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    //Seleciona a fonte
    display.setFont(ArialMT_Plain_16);
    display.drawString(63, 10, "IoT");
    display.drawString(63, 26, "Instituto");
    display.drawString(63, 45, "Atlantico");
    display.display();
}

void ProgressBar()
{
    for (int counter = 0; counter <= 100; counter++)
    {
        display.clear();
        //Desenha a barra de progresso
        display.drawProgressBar(0, 32, 120, 10, counter);
        //Atualiza a porcentagem completa
        display.setTextAlignment(TEXT_ALIGN_CENTER);
        display.drawString(64, 15, String(counter) + "%");
        display.display();
        delay(10);
    }
}

void loop()
{
    ProgressBar();
    delay(3000);
    telainicial();
    delay(5000);
}