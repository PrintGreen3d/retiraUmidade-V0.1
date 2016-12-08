#include <Wire.h> //Inclusão necessária para o uso do LCD
#include <LiquidCrystal_I2C.h> //Inclusão da biblioteca do LCD I2C
#include <dht.h> //Inclusão do famoso DHT11
 
dht DHT;
 
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); //Setup do LCD 0x27
 
#define dhtPin A1 //A mesma porta A1 do exemplo anterior
 
float uAr; //Declaração da variável flutante da umidade do ar
float T; //Declaração da variável flutante da temperatura
const int Rele = 7;  // Define o pino que é usado para desativar o rele
 
void setup() 
{ 
   pinMode(7, OUTPUT);
   lcd.begin(20,4); //Inicialização do LCD 
   lcd.noBacklight();
   delay(250);
   lcd.backlight();
 
   lcd.setCursor(0,2);
   lcd.write("Controle Estufa"); 
   lcd.setCursor(0,3);
   lcd.write("PrintGreen3D"); 
   lcd.blink();
 
   delay(1000);
   lcd.noBlink(); 
 
for (int i = 0; i < 20; i++) {
   delay(100);
   lcd.scrollDisplayLeft();
}
delay(1000);
 
   lcd.clear(); // Turns off the blinking cursor
 
}
 
void loop() 
{
 
   DHT.read11(dhtPin);
   uAr = DHT.humidity;
   T = DHT.temperature;
 
   //Output lcd
   lcd.setCursor(0,0);
   lcd.write("Umidade    : ");
   lcd.print(uAr,0); 
   lcd.write("%");
 
   lcd.setCursor(0,1);
   lcd.write("Temperatura: ");
   lcd.print(T,0); 
   lcd.write(" C");
 
 
    if(T <= 80)
    {
      digitalWrite(7, HIGH); // Liga LED indicando tecla pressionada
    }
    else
    {
      digitalWrite(7,LOW); // Desliga LED indicando tecla solta
    }
   
   
   
   delay(10000); 
   lcd.noBlink(); 
   lcd.clear(); //Limpa para nova exibição
}
