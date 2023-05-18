#include <LiquidCrystal.h> 

int Encoder_OuputA  = 9;
int Encoder_OuputB  = 8;
int Encoder_Switch = 10;

int Previous_Output;
int diadelasemana=0;
void Seleccion_dias();
/***Letras seleccionadas***/
byte L[8] = {
0b01111,
0b01111,
0b01111,
0b01111,
0b01111,
0b01111,
0b00000,
0b11111
};

byte M[8] = {
0b01110,
0b00100,
0b01010,
0b01010,
0b01110,
0b01110,
0b01110,
0b11111
};

byte a[8] = {
0b11111,
0b11111,
0b10001,
0b11110,
0b10000,
0b01110,
0b10000,
0b11111
};

byte i[8] = {
0b11011,
0b11111,
0b10011,
0b11011,
0b11011,
0b11011,
0b10001,
0b11111
};

byte J[8] = {
0b11000,
0b11101,
0b11101,
0b11101,
0b11101,
0b01101,
0b10011,
0b11111
};

byte V[8] = {
0b01110,
0b01110,
0b01110,
0b01110,
0b01110,
0b10101,
0b11011,
0b11111
};

byte S[8] = {
0b10001,
0b01110,
0b01111,
0b10001,
0b11110,
0b01110,
0b10001,
0b11111
};

byte D[8] = {
0b00011,
0b01101,
0b01110,
0b01110,
0b01110,
0b01101,
0b00011,
0b11111
};
/**************************/

const int rs = 7, en = 6, d4 = 5, d5 = 4, d6 = 3, d7 = 2; //Mention the pin number for LCD connection
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  // put your setup code here, to run once:
 lcd.begin(16, 2); //Initialise 16*2 LCD

  // Print a message to the LCD.
  lcd.setCursor(0, 0);
  lcd.print("Seleccionar dia");
  lcd.setCursor(0, 1);
  lcd.print("D L Ma Mi J V S");
  
  lcd.createChar(0,D);
  lcd.createChar(1,L);
  lcd.createChar(2,M);
  lcd.createChar(3,a);
  lcd.createChar(4,i);
  lcd.createChar(5,J);
  lcd.createChar(6,V);
  lcd.createChar(7,S);
  
  //Selecciono DOMINGO
  lcd.setCursor(0, 1);
  lcd.write(byte(0));//D seleccionada
  Serial.begin(9600);
  
  pinMode (Encoder_OuputA, INPUT);
  pinMode (Encoder_OuputB, INPUT);
  pinMode (Encoder_Switch, INPUT);
  Previous_Output = digitalRead(Encoder_OuputA); //Read the inital value of Output A
}

void loop() {
 if (digitalRead(Encoder_OuputA) != Previous_Output)
   { 
     if (digitalRead(Encoder_OuputB) != Previous_Output) 
     { 
        if(diadelasemana==6)
          {
            diadelasemana=0;
            Serial.print("Variable");
            Serial.print(diadelasemana);
            Serial.print("\n");
          }
        else
        {
            diadelasemana ++;
            Serial.print("Variable");
            Serial.print(diadelasemana);
            Serial.print("\n");
        }
     } 
     else
     {
        if(diadelasemana>=0)
        {
          diadelasemana--;
          Serial.print("Variable");
          Serial.print(diadelasemana);
          Serial.print("\n");
        }
        if(diadelasemana<0)
          {
            diadelasemana=6;
            Serial.print("Variable");
            Serial.print(diadelasemana);
            Serial.print("\n");
          }
     }
   }
    Previous_Output = digitalRead(Encoder_OuputA);
    Seleccion_dias();

delay(10);
}
void Seleccion_dias(void)
{
  switch(diadelasemana)
      {
        case 0:
          //Selecciono DOMINGO
          lcd.setCursor(0, 1);
          lcd.write(byte(0));//D seleccionada
          //Deselecciono resto de dias
          
          //Deselecciono SABADO
          lcd.setCursor(14, 1);
          lcd.print("S");
          //Deseleccionar_dias(diadelasemana);
  
          //Deselecciono LUNES
          lcd.setCursor(2, 1);
          lcd.print("L");
          break;
        
        case 1:
          //Selecciono LUNES
          lcd.setCursor(2, 1);
          lcd.write(byte(1));//L seleccionada
        
          //Deselecciono DOMINGO
          lcd.setCursor(0, 1);
          lcd.print("D");
  
          //Deselecciono MARTES
          lcd.setCursor(4, 1);
          lcd.print("Ma");
          break;
        
        case 2:
          //Selecciono MARTES
          lcd.setCursor(4, 1);
          lcd.write(byte(2));//M seleccionada
          lcd.write(byte(3));//a seleccionada
        
          //Deselecciono LUNES
          lcd.setCursor(2, 1);
          lcd.print("L");
  
          //Deselecciono MIERCOLES
          lcd.setCursor(7, 1);
          lcd.print("Mi");
          break;
        
        case 3:
          //Selecciono MIERCOLES
          lcd.setCursor(7, 1);
          lcd.write(byte(2));//M seleccionada
          lcd.write(byte(4));//i seleccionada
        
          //Deselecciono MARTES
          lcd.setCursor(4, 1);
          lcd.print("Ma");
  
          //Deselecciono JUEVES
          lcd.setCursor(10, 1);
          lcd.print("J");
          break;
        
        case 4:
          //Selecciono JUEVES
          lcd.setCursor(10, 1);
          lcd.write(byte(5));//J seleccionada
        
          //Deselecciono MIERCOLES
          lcd.setCursor(7, 1);
          lcd.print("Mi");
  
          //Deselecciono VIERNES
          lcd.setCursor(12, 1);
          lcd.print("V");
          break;
        
        case 5:
          //Selecciono VIERNES
          lcd.setCursor(12, 1);
          lcd.write(byte(6));//V seleccionada
        
          //Deselecciono JUEVES
          lcd.setCursor(10, 1);
          lcd.print("J");
  
          //Deselecciono SABADO
          lcd.setCursor(14, 1);
          lcd.print("S");
          break;
        
        case 6:
          //Selecciono SABADO
          lcd.setCursor(14, 1);
          lcd.write(byte(7));//S seleccionada
        
          //Deselecciono VIERNES
          lcd.setCursor(12, 1);
          lcd.print("V");
  
          //Deselecciono DOMINGO
          lcd.setCursor(0, 1);
          lcd.print("D");
          break;
    }
}
//void Deseleccionar_dias(int diadelasemana)
