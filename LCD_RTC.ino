/*
 * Sistema de controle da estufa 
 * Version: 2.0
 * 24/Novembro/2017
 * Hardware: 
 *            - Arduino Uno
 *            - RTC
 *            - LCD 16x2 I2C
 *            - Shield Rele 4 
 *            - Potenciometro 10k
 *            - Button
 *            - LM35
 * Software:
 *          O sistema ira ter uma tela de inicialão, após essa tela teremos uma que mostra dia, mes ano e horas.
 * 
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Inicializa o display no endereco 0x27
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
byte zero = 0x00;


// Modulo RTC no endereco 0x68
#define DS1307_ADDRESS 0x68


// Definição de Variaveis e Porta utilizadas
const int LM35    = A0;  // Define o pino que lera a saída do LM35
const int Rele    = 5;   // Define o pino que é usado para desativar o rele
float temperatura = 0;   // Variável que armazenará a temperatura medida
int Potenciometro = A1;  // center pin of the potentiometer
const int Enter   = 2;   // Botão de Seleção
int buttonState   = 0;   // variable for reading the pushbutton status
int StatusEnter   = 0;
const int ledPin  = 13;  // the number of the LED pin
int opcaoSwitch   = 0;

/*
 * CapturaTemperatura
 * Lê a porta A0 (sensor lm35) 20 vezes, após essa captura faz os calculos e divide pela quantidade de vezes capturada
 */
float CapturaTemperatura()
{
   float leitura[20] = {0}; // Armazena valores recolhidos pelo sensor
   float armazena = 0;

   //Faz a captura de valores
   for (int i = 0; i < 20; i++) 
   {
      delay(300);
      // leitura[i] = ((float(analogRead(LM35)) * 5 / (1023)) / 0.01);
      leitura[i] = (float( 5.0 * analogRead(LM35) * 100.0) / 1024.0);
      //reading = analogRead(tempPin);
     //tempC = reading / 9.31;
  }
  
  //Soma o array de valores capturados
  for (int i = 0; i < 20; i++) 
  {
    armazena = armazena + leitura[i];
  }
  // Media de temperatura (todos os valores somados dividido pela quantidade)
  armazena = (armazena / 20);
  return armazena;
}

/*
 * SelecionaDataeHora
 * Função para fazer a gravação no RTC, serve para atualizar a data e a hora deve ser usada somente uma vez, após a atualização a função deve
 * ser comentada.
 * Alterar somente as variaveis de inicialização.
 */
void SelecionaDataeHora()   //Seta a data e a hora do DS1307
{
  byte segundos = 00; //Valores de 0 a 59
  byte minutos = 24; //Valores de 0 a 59
  byte horas = 10; //Valores de 0 a 23
  byte diadasemana = 5; //Valores de 0 a 6 - 0=Domingo, 1 = Segunda, etc.
  byte diadomes = 24; //Valores de 1 a 31
  byte mes = 11; //Valores de 1 a 12
  byte ano = 17; //Valores de 0 a 99
  Wire.beginTransmission(DS1307_ADDRESS);
  Wire.write(zero); //Stop no CI para que o mesmo possa receber os dados

  //As linhas abaixo escrevem no CI os valores de 
  //data e hora que foram colocados nas variaveis acima
  Wire.write(ConverteParaBCD(segundos));
  Wire.write(ConverteParaBCD(minutos));
  Wire.write(ConverteParaBCD(horas));
  Wire.write(ConverteParaBCD(diadasemana));
  Wire.write(ConverteParaBCD(diadomes));
  Wire.write(ConverteParaBCD(mes));
  Wire.write(ConverteParaBCD(ano));
  Wire.write(zero);
  Wire.endTransmission(); 
}

byte ConverteParaBCD(byte val)
{ 
  //Converte o número de decimal para BCD
  return ( (val/10*16) + (val%10) );
}

byte ConverteparaDecimal(byte val)  
{ 
  //Converte de BCD para decimal
  return ( (val/16*10) + (val%16) );
}


void MostraHora()
{
  // Refazer esta função - Jeito mais facil que pensei no momento, passar os valores como array, e trabalhar com eles 
  // tomara que não estrague tudo.
  // Fazer backup dessa versão antes de alterar.
  
  // Le os valores (data e hora) do modulo DS1307
  Wire.beginTransmission(DS1307_ADDRESS);
  Wire.write(zero);
  Wire.endTransmission();
  Wire.requestFrom(DS1307_ADDRESS, 7);
  int segundos = ConverteparaDecimal(Wire.read());
  int minutos = ConverteparaDecimal(Wire.read());
  int horas = ConverteparaDecimal(Wire.read() & 0b111111);
  int diadasemana = ConverteparaDecimal(Wire.read()); 
  int diadomes = ConverteparaDecimal(Wire.read());
  int mes = ConverteparaDecimal(Wire.read());
  int ano = ConverteparaDecimal(Wire.read());

  //Comprimenta
  if((horas >= 1) && (horas <=12))
  {
     lcd.setCursor(0,0);
     lcd.print("Bom dia!");
  }
  else if((horas >= 13) && (horas <=18))
  {
     lcd.setCursor(0,0);
     lcd.print("Boa Tarde!");
  }
  else
  {
     lcd.setCursor(0,0);
     lcd.print("Boa Noite!");
  }


  // Mostra hora no display
  lcd.setCursor(10,0);
  lcd.print(" ");
  //Acrescenta o 0 (zero) se a hora for menor do que 10
  if (horas <10)
    lcd.print("0");
    lcd.print(horas);
    lcd.print(":");
  //Acrescenta o 0 (zero) se minutos for menor do que 10
  if (minutos < 10)
     lcd.print("0");
     lcd.print(minutos);
     lcd.setCursor(2,1);
  //Mostra o dia da semana
  switch(diadasemana)
    {
      case 0:lcd.print("Dom");
      break;
      case 1:lcd.print("Seg");
      break;
      case 2:lcd.print("Ter");
      break;
      case 3:lcd.print("Quar");
      break;
      case 4:lcd.print("Qui");
      break;
      case 5:lcd.print("Sex");
      break;
      case 6:lcd.print("Sab");
    }
    lcd.setCursor(6,1);
    //Acrescenta o 0 (zero) se dia do mes for menor do que 10
    if (diadomes < 10)
      lcd.print("0");
      lcd.print(diadomes);
      lcd.print("/");
    // Acrescenta o 0 (zero) se mes for menor do que 10
    if (mes < 10)
      lcd.print("0");
      lcd.print(mes);
      lcd.print("/");
      lcd.print(ano);
}

int MostraPosicao()
{
    int sensorValue = map(analogRead(Potenciometro), 0, 1025, 1, 50);
    // int pot = map(analogRead(potPin), 0, 1025, 1, 50);

    //lcd.clear();
    // set cursor to second row, first column
    //lcd.setCursor(0, 1);
    //lcd.print(sensorValue);
    //lcd.print("      ");
    delay (300);
    return (sensorValue);
    //lcd.clear();
}

byte converteparaDecimal(byte val)  
{ 
  //Converte de BCD para decimal
  return ( (val/16*10) + (val%16) );
}

int meuMenuOperacional(int Opcao)
{
  switch (Opcao) {
    case 1:
      //Pega a hora atual
      // Le os valores (data e hora) do modulo DS1307
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      break;
    case 2:
      //do something when var equals 2
      break;
    case 3:
      //do something when var equals 3
      break;
    case 4:
    default:
      // if nothing else matches, do the default
      // default is optional
      break;
  }

  
}



void setup()
{
  //A linha é usada para acertar a Data e a Hora
  //SelecionaDataeHora();

  // Inicializa o botão de seleção (ENTER)
  pinMode(Enter, INPUT);
  pinMode(ledPin, OUTPUT);
  
  pinMode(Rele, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  analogReference(INTERNAL);
  lcd.setBacklight(HIGH);
  
  pinMode(7, OUTPUT);
  lcd.begin(20,4); //Inicialização do LCD 
  lcd.noBacklight();
  delay(250);
  lcd.backlight();
  lcd.setCursor(0,2);
  lcd.write(" Controle Estufa"); 
  lcd.setCursor(0,3);
  lcd.write("**PrintGreen3D**"); 
  //lcd.blink();
  delay(3000);
  lcd.noBlink(); 
  
  for (int i = 0; i < 20; i++) {
   delay(200);
   lcd.scrollDisplayLeft();
  }
  delay(2500);
  lcd.clear(); // Turns off the blinking cursor
}



void loop()
{
    int potencia = 0;
    //Inicia mostrando a Hora
    MostraHora();
    
    // Verifica se o botão foi pressionado
    buttonState = digitalRead(Enter);
    //Se sim a entrada do arduino muda para Ligada (HIGH)
    if (buttonState == HIGH) 
    {
      //Limpo o LCD    
      lcd.clear();
      //Atribui o valor a variavel StatusEnter
      StatusEnter = 1;
      delay (200);
      while(StatusEnter == 1)
      {
         // lcd.setCursor(1,0);
        //  lcd.print("Filamento");
        //  lcd.setCursor(1,1);
        //  lcd.print("Granulos");

          potencia = MostraPosicao();

          if((potencia >=0) && (potencia <= 12))
          {
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("> Filamentos");
            lcd.setCursor(0,1);
            lcd.print("  Granulos");
            opcaoSwitch = 1;
          }
          else if ((potencia >=13) && (potencia <= 25))
          {
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("  Filamentos");
            lcd.setCursor(0,1);
            lcd.print("> Granulos");
            opcaoSwitch = 2;
          }

          else if ((potencia >=26) && (potencia <= 40))
          {
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("  Granulos");
            lcd.setCursor(0,1);
            lcd.print("> Def. Manual");
            opcaoSwitch = 3;
          }
          else
          {
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("  Def. Manual");
            lcd.setCursor(0,1);
            lcd.print("> Sair");
            opcaoSwitch = 4;
          }

          meuMenuOperacional(opcaoSwitch);
          


          

          
          
          
      
          //Verifica o botão novamente
          buttonState = digitalRead(Enter);
          if (buttonState == HIGH) 
          {
            //Atribui o valor a variavel StatusEnter
            StatusEnter = 0;
            //lcd.clear();
            lcd.setCursor(0,0);
            lcd.print(StatusEnter);
            delay(200);
            lcd.clear();
          }
      }          
    
    }   
}





