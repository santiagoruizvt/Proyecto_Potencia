#include <LiquidCrystal.h> 
#include <EEPROM.h>
#include <TimerOne.h> //Para simular el reloj del RTC

//******************************************************************************
//	ETIQUETAS
//******************************************************************************
//Menu
#define MENU_PRINCIPAL          0
#define MENU_AJUSTE_FECHA_HORA  1
#define MENU_CONFIGURACION      2
#define MENU_PROGRAMA_SEMANAL   3
#define MENU_FERIADOS           4

//Dias de la semana
#define DOMINGO   0
#define LUNES     1
#define MARTES    2
#define MIERCOLES 3
#define JUEVES    4
#define VIERNES   5
#define SABADO    6

//Rangos
#define INICIO_1	0
#define FIN_1		  1
#define INICIO_2	2
#define FIN_2		  3
#define INICIO_3	4
#define FIN_3		  5
#define INICIO_4	6
#define FIN_4		  7

#define CANT_RANGOS 4

//Encoder
#define DERECHA   1
#define IZQUIERDA 2
#define ENTER     3

//Para lectura de EEPROM
#define HORA    0
#define MINUTOS 1


//******************************************************************************
//	VARIABLES
//******************************************************************************
int Encoder_OuputA  = 9;
int Encoder_OuputB  = 8;
int Encoder_Switch = 10;
int Salida_Rele = 13;

int Previous_Output;
int diadelasemana=0;
int diadelasemana_actual=0;
int estado_menu=0;
int estado_menu_anterior=0;
int estado_ajuste=0;
int estado_prog_sem=0;
int estado_prog_rango=0;
int proximo_menu=0;
int seg_anterior=0;
int rango_a_programar=0;
char Rango_num[CANT_RANGOS*2];
char Rangos_hoy[CANT_RANGOS*2];
char codigo_hora_actual=0;
char codigo_hora_actual_anterior=0;


//ESTRUCTURA DE FECHA Y HORA
struct RTC_Time
{
  //FECHA
  char dia; //[1,31]
  char mes; //[1,12]
  char anio; //[0,255]
  //HORA
  char seg; //[0,59]
  char min; //[0,59]
  char hora; //[0,23]
};

RTC_Time ActualTime;
RTC_Time AjusteTime;
RTC_Time AuxTime;

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
0b10000,
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
/*
//No entra porque la memoria del LCD permite hasta 8 caracteres especiales
//Usamos en su lugar el símbolo '>'
byte flecha[8] = {
0b00000,
0b00100,
0b00010,
0b11111,
0b00010,
0b00100,
0b00000,
0b00000
};*/
/**************************/

const int rs = 7, en = 6, d4 = 14, d5 = 15, d6 = 16, d7 = 17; //Mention the pin number for LCD connection
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  
  char BufferAux[50];

  // put your setup code here, to run once:
  lcd.begin(16, 2); //Initialise 16*2 LCD
  
  lcd.createChar(0,D);
  lcd.createChar(1,L);
  lcd.createChar(2,M);
  lcd.createChar(3,a);
  lcd.createChar(4,i);
  lcd.createChar(5,J);
  lcd.createChar(6,V);
  lcd.createChar(7,S);
  
  Serial.begin(9600);
  
  pinMode (Encoder_OuputA, INPUT);
  pinMode (Encoder_OuputB, INPUT);
  pinMode (Encoder_Switch, INPUT);
  Previous_Output = digitalRead(Encoder_OuputA); //Read the inital value of Output A
  pinMode (Salida_Rele, OUTPUT);
  
  //Acá debería leer los datos del RTC
  //FECHA
  ActualTime.dia=21;
  ActualTime.mes=5;
  ActualTime.anio=23;
  //HORA
  ActualTime.hora=19;
  ActualTime.min=0;
  ActualTime.seg=0;

  //Para simular el timer del RTC
  Timer1.initialize(1000000);
  Timer1.attachInterrupt(ISR_Segundo);

  Rango_num[INICIO_1]=Rango_num[FIN_1]='1';
  Rango_num[INICIO_2]=Rango_num[FIN_2]='2';
  Rango_num[INICIO_3]=Rango_num[FIN_3]='3';
  Rango_num[INICIO_4]=Rango_num[FIN_4]='4';

  #warning "Para las pruebas hardcodeo DOMINGO, pero este dato saldría del RTC"
  diadelasemana_actual = DOMINGO;

  #warning "Esto habria que hacerlo al inicio y cada vez que detecta un cambio de día, y cada vez que se reprograman los rangos"

  for(int i=0; i<CANT_RANGOS*2; i++)
  {
    Rangos_hoy[i]=EEPROM.read(diadelasemana_actual*8+i);

    if(i%2==0)
      Serial.print("Inicio ");
    else
      Serial.print("Fin ");
    
    sprintf(BufferAux,"%d: %d\n",i,Rangos_hoy[i]);
    Serial.print(BufferAux);
  }
}

//******************************************************************************
// Función:				Leer_Encoder
//
// Descripción:		Devuelve la dirección del enconder o si se apretó el pulsador,
//                teniendo prioridad el pulsador
//
// Parámetros:			void
// Valor devuelto:	int  IZQUIERDA(antihorario), DERECHA(horario) o ENTER
// 
//******************************************************************************
int Leer_Encoder(void){
  
  int giro_encoder=0;

  if (digitalRead(Encoder_OuputB) != Previous_Output)
  { 
    if (digitalRead(Encoder_OuputA) != Previous_Output) 
    { 
      giro_encoder = DERECHA;
    } 
    else
    {
      giro_encoder = IZQUIERDA;
    }
  }
  
  Previous_Output = digitalRead(Encoder_OuputB);

  //El ENTER tiene prioridad ante los giros
  if(digitalRead(Encoder_Switch)==0)
  {
    while(digitalRead(Encoder_Switch)==0)
    {
      //Espero a que suelte el pulsador
    };
    return(ENTER);  
  }

  return(giro_encoder);
}

//******************************************************************************
// Función:				Programacion_Semanal
//
// Descripción:		Se programan los rangos de encendido de cada día de la semana
//
// Parámetros:			void
// Valor devuelto:	void  
// 
//******************************************************************************
void Programacion_Semanal(void) {

  char BufferAux[5];

  switch(estado_prog_sem)
  {
    case 0:
      lcd.setCursor(0, 0);
      lcd.print("Seleccionar dia");
      lcd.setCursor(0, 1);
      lcd.print("D L Ma Mi J V S");

      //Selecciono DOMINGO
      lcd.setCursor(0, 1);
      lcd.write(byte(0));//D seleccionada
      diadelasemana=0;

      delay(1000);

      estado_prog_sem++;
    break;

    case 1:
      switch(Leer_Encoder())
      {
        default:
          break;

        case DERECHA:
          if(diadelasemana==6)
          {
            diadelasemana=0;
            Serial.print("Variable: ");
            Serial.print(diadelasemana);
            Serial.print("\n");
          }
          else
          {
            diadelasemana ++;
            Serial.print("Variable: ");
            Serial.print(diadelasemana);
            Serial.print("\n");
          }
          delay(250);
        break;
          
        case IZQUIERDA:

          if(diadelasemana>=0)
          {
            diadelasemana--;
            Serial.print("Variable: ");
            Serial.print(diadelasemana);
            Serial.print("\n");
          }
          if(diadelasemana<0)
          {
            diadelasemana=6;
            Serial.print("Variable: ");
            Serial.print(diadelasemana);
            Serial.print("\n");
          }
          delay(250);
        break;

        case ENTER:
          estado_prog_sem++;
        break;
      }
      
      Seleccion_dias();

      delay(10);
    break;

    case 2:
    //Presento el día en pantalla
    lcd.clear();
    lcd.setCursor(0, 0);
    switch(diadelasemana)
    {
      case DOMINGO:
        lcd.print("DOMINGO");
        break;
      
      case LUNES:
        lcd.print("LUNES");
        break;
      
      case MARTES:
        lcd.print("MARTES");
        break;
      
      case MIERCOLES:
        lcd.print("MIERCOLES");
        break;
      
      case JUEVES:
        lcd.print("JUEVES");
        break;
      
      case VIERNES:
        lcd.print("VIERNES");
        break;
      
      case SABADO:
        lcd.print("SABADO");
        break;
    }


    estado_prog_sem++;
    rango_a_programar=0;
    estado_prog_rango=0;
    break;

    case 3:
      if(Programar_Rango(rango_a_programar)==1)
        rango_a_programar++;

      if(rango_a_programar >= (CANT_RANGOS*2))
        estado_prog_sem++;
    break;

    case 4:
      lcd.setCursor(0, 0);
      lcd.print("Config guardada ");
      lcd.setCursor(0, 1);
      lcd.print("   con exito    ");
      delay(3000);
      estado_menu=MENU_PRINCIPAL;
      estado_prog_sem=0;
      diadelasemana=0;
    break;
  }
}


//******************************************************************************
// Función:				Programar_Rango
//
// Descripción:		Se programan las horas y minutos de cada rango
//
// Parámetros:			int rango: INICIO1,FIN1,INICIO2,FIN2...etc
// Valor devuelto:	bool: devuelve un 1 cuando ya se programó ese rango, sino 0  
// 
//******************************************************************************
bool Programar_Rango(int rango)
{
  //Por cada rango a programar va a entrar acá
  char BufferAux[5];

  switch(estado_prog_rango)
  {
    case 0:
      lcd.setCursor(10, 0);
      lcd.print("Rango");
      lcd.setCursor(15, 0);
      lcd.print(Rango_num[rango]);

      if(rango%2==0)
      {
        lcd.setCursor(0, 1);
        lcd.print("Encendido:");//Rangos pares son encedido
      }
      else
      {
        lcd.setCursor(0, 1);
        lcd.print("Apagado:  ");//Rangos impares son apagado
      }

      AuxTime.hora = Leer_Rangos_EEPROM(diadelasemana, rango, HORA);
      AuxTime.min  = Leer_Rangos_EEPROM(diadelasemana, rango, MINUTOS);

      lcd.setCursor(11, 1);
      if((AuxTime.hora<0 || AuxTime.hora>23)||(AuxTime.min<0 || AuxTime.min>45))
      {
        //Si están fuera de rango los muestro como guiones 
        lcd.print("--:--");
        AuxTime.hora = 0;
        AuxTime.min  = 0;
      }
      else
      {
        sprintf(BufferAux,"%02d:%02d",AuxTime.hora,AuxTime.min);
        lcd.print(BufferAux);
      }
      estado_prog_rango++;
    break;

    case 1:
      //Programo la hora
      lcd.setCursor(11, 1);
      sprintf(BufferAux,"%02d:%02d",AuxTime.hora,AuxTime.min);
      lcd.print(BufferAux);
      Efecto_Titilar(11,1,2,100);
      
      switch(Leer_Encoder())
      {
        case IZQUIERDA:
          AuxTime.hora--;
          if(AuxTime.hora<0)
            AuxTime.hora=23;
        break;

        case DERECHA:
          AuxTime.hora++;
          if(AuxTime.hora>23)
            AuxTime.hora=0;
        break;

        case ENTER:
          estado_prog_rango++;
        break;

      }
    break;

    case 2:
      //Programo los minutos
      lcd.setCursor(11, 1);
      sprintf(BufferAux,"%02d:%02d",AuxTime.hora,AuxTime.min);
      lcd.print(BufferAux);
      Efecto_Titilar(14,1,2,100);
      
      switch(Leer_Encoder())
      {
        case IZQUIERDA:
          AuxTime.min=AuxTime.min-15;
          if(AuxTime.min<0)
            AuxTime.min=45;
        break;

        case DERECHA:
          AuxTime.min=AuxTime.min+15;
          if(AuxTime.min>45)
            AuxTime.min=0;
        break;

        case ENTER:
          estado_prog_rango++;
        break;

      }
    break;

    case 3:
      //Guardo el rango en EEPROM

      #warning "Habria que hacer una logica que se fije que el inicio sea antes del fin en cada rango, y que no se superpongan"

      Escribir_Rangos_EEPROM(diadelasemana,rango,AuxTime.hora,AuxTime.min);


      estado_prog_rango=0;
      return(1);
    break;

  }
  return(0);
}

//******************************************************************************
// Función:				Leer_Rangos_EEPROM
//
// Descripción:		Se leen las horas o minutos (según se solicite) guardado 
//                en la EEPROM del día solicitado en el rango solicitado.
//
// Parámetros:			int dia (LUNES, MARTES... etc)
//                  int rango (INICIO1, FIN1, INICIO2... etc)
// Valor devuelto:	bool hora_min: aclaras si queres recibir la hora o los mins   
//                                 (HORA o MINUTOS)
// 
//******************************************************************************
char Leer_Rangos_EEPROM(int dia, int rango, bool hora_min)
{
  char codigo_hora=0;
  int direccion_eeprom=0;

  //Leo la hora y fecha programada
  direccion_eeprom= dia*8+rango;
  codigo_hora = EEPROM.read(direccion_eeprom);

  if(hora_min == HORA)
    return(codigo_hora/4);
  else //if(hora_min == MINUTOS)
  {
    codigo_hora=codigo_hora%4; //Agarro el resto (de 0 a 3)
    codigo_hora=codigo_hora*15;//Lo multiplico por 15
    return(codigo_hora);
  }
    
}

//******************************************************************************
// Función:				Escribir_Rangos_EEPROM
//
// Descripción:		Se escribe en la EEPROM el código generado por las horas y 
//                minutos en la dirección generada por el día y rango indicados 
//
// Parámetros:			int dia (LUNES, MARTES... etc)
//                  int rango (INICIO1, FIN1, INICIO2... etc)
//                  int hora
//                  int min (múltiplo de 15 preferentemente)
//
// Valor devuelto:	void
// 
//******************************************************************************
void Escribir_Rangos_EEPROM(int dia, int rango, int hora, int min)
{
  char codigo_hora=0;
  int direccion_eeprom=0;

  direccion_eeprom= dia*8+rango;

  codigo_hora=((hora*4)+(min/15));

  EEPROM.write(direccion_eeprom,codigo_hora);

}

//******************************************************************************
// Función:				Seleccion_dias
//
// Descripción:		Se utiliza el encoder para seleccionar el día de la semana
//
// Parámetros:			void
//
// Valor devuelto:	void
// 
//******************************************************************************
void Seleccion_dias(void)
{
  switch(diadelasemana)
  {
    case DOMINGO:
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
    
    case LUNES:
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
    
    case MARTES:
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
    
    case MIERCOLES:
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
    
    case JUEVES:
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
    
    case VIERNES:
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
    
    case SABADO:
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

//******************************************************************************
// Función:				Menu_Principal
//
// Descripción:		Muestra la fecha y la hora y utiliza el encoder para elegir
//                ajustar la hora/fecha o configurar la programación
//
// Parámetros:			void
//
// Valor devuelto:	void
// 
//******************************************************************************
void Menu_Principal(void){

  char BufferAux[16];

  //Muestro la fecha y hora actual
  lcd.setCursor(0, 0);
  sprintf(BufferAux," %02d/%02d/%02d %02d:%02d ",ActualTime.dia,ActualTime.mes,ActualTime.anio,ActualTime.hora,ActualTime.min);
  lcd.print(BufferAux);

  lcd.setCursor(1, 1);
  lcd.print("Reloj");
  lcd.setCursor(10, 1);
  lcd.print("Config");


  //Para la primera vez que entra aca
  //if((proximo_menu != MENU_CONFIGURACION)&&(proximo_menu != MENU_AJUSTE_FECHA_HORA))
  if(proximo_menu != MENU_CONFIGURACION)
  {
    proximo_menu = MENU_AJUSTE_FECHA_HORA;
    lcd.setCursor(0, 1);
    //lcd.write(byte(8));//Flecha señalando Reloj
    lcd.print(">");
  }


  //Serial.print("proximo_menu:");
  //Serial.println(proximo_menu);
  
  Seleccion_Principal();

  //Hago titilar el ':' para que se vea que está vivo 
  Efecto_Titilar(12,0,1,50);
}

//******************************************************************************
// Función:				Seleccion_Principal
//
// Descripción:		Dibuja una flecha en el menu seleccionado dentro del principal
//                Si se presiona ENTER, entra a ese menu
//
// Parámetros:			void
//
// Valor devuelto:	void
// 
//******************************************************************************
void Seleccion_Principal(void)
{

  switch(Leer_Encoder())
  {
    default:
      break;

    case DERECHA:
      lcd.setCursor(0, 1);
      lcd.print(" ");
      lcd.setCursor(9, 1);
      //lcd.write(byte(8));//Flecha señalando Config
      lcd.print(">");
      proximo_menu = MENU_CONFIGURACION;
      delay(250);
    break;
      
    case IZQUIERDA:
      lcd.setCursor(0, 1);
      //lcd.write(byte(8));//Flecha señalando Reloj
      lcd.print(">");
      lcd.setCursor(9, 1);
      lcd.print(" ");
      proximo_menu = MENU_AJUSTE_FECHA_HORA;
      delay(250);
    break;

    case ENTER:
      estado_menu = proximo_menu;
    break;
  }

}

//******************************************************************************
// Función:				Seleccion_Configuracion
//
// Descripción:		Muestra el menú Configuración y dibuja una flecha en el menu 
//                seleccionado. Si se presiona ENTER, entra a ese menu.
//
// Parámetros:			void
//
// Valor devuelto:	void
// 
//******************************************************************************
void Seleccion_Configuracion(void)
{
  lcd.setCursor(1, 0);
  lcd.print("Prog. semanal");
  
  lcd.setCursor(1, 1);
  lcd.print("Feriados");

  
  //Para la primera vez que entra aca
  if((proximo_menu != MENU_PROGRAMA_SEMANAL)&&(proximo_menu != MENU_FERIADOS))
  {
    lcd.setCursor(0, 0);
    //lcd.write(byte(8));//Flecha señalando Prog Semanal
    lcd.print(">");
    lcd.setCursor(0, 1);
    lcd.print(" ");
    proximo_menu = MENU_PROGRAMA_SEMANAL;

    delay(1000);
  }

  //Dibuja una flecha en el menu seleccionado
  //Si se presiona ENTER, entra a ese menu

  switch(Leer_Encoder())
  {
    default:
      break;

    case IZQUIERDA:
      lcd.setCursor(0, 0);
      //lcd.write(byte(8));//Flecha señalando Prog Semanal
      lcd.print(">");
      lcd.setCursor(0, 1);
      lcd.print(" ");
      proximo_menu = MENU_PROGRAMA_SEMANAL;
      delay(250);
    break;
      
    case DERECHA:
      lcd.setCursor(0, 0);
      lcd.print(" ");
      lcd.setCursor(0, 1);
      //lcd.write(byte(8));//Flecha señalando Feriados
      lcd.print(">");
      proximo_menu = MENU_FERIADOS;
      delay(250);
    break;

    case ENTER:
      estado_menu = proximo_menu;
    break;
  }

}

//******************************************************************************
// Función:				Reloj
//
// Descripción:		Reloj en base al timer de 1seg para simular el RTC
//
// Parámetros:			void
//
// Valor devuelto:	void
// 
//******************************************************************************
void Reloj(void){
  if(ActualTime.seg >59)
  {
    ActualTime.seg=0;
    ActualTime.min++;
    if(ActualTime.min >59)
    {
      ActualTime.min=0;
      ActualTime.hora++;
      if(ActualTime.hora>23)
        ActualTime.hora=0;  //Faltaría cambiar de día
    }
  }
}

//******************************************************************************
// Función:				ISR_Segundo
//
// Descripción:		Timer de 1seg
//
// Parámetros:			void
//
// Valor devuelto:	void
// 
//******************************************************************************
void ISR_Segundo(void){

  ActualTime.seg++;

}

//******************************************************************************
// Función:				Ajuste_Reloj
//
// Descripción:		Menú para modificar la hora y fecha actual usando el encoder
//
// Parámetros:			void
//
// Valor devuelto:	void
// 
//******************************************************************************
void Ajuste_Reloj(void)
{
  char BufferAux[16];

  switch(estado_ajuste)
  {
    case 0:
      //Muestro la fecha y hora actual
      lcd.setCursor(0, 0);
      lcd.print("Seleccione hora ");
      lcd.setCursor(0, 1);
      lcd.print("y fecha actual  ");
      delay(3000);
      estado_ajuste++;
    break;

    case 1:
      //Muestro la fecha y hora actual
      lcd.setCursor(0, 0);
      sprintf(BufferAux,"Dia:  %02d/%02d/%02d  ",ActualTime.dia,ActualTime.mes,ActualTime.anio+2000);
      lcd.print(BufferAux);

      lcd.setCursor(0, 1);
      sprintf(BufferAux,"Hora: %02d:%02d       ",ActualTime.hora,ActualTime.min);
      lcd.print(BufferAux);

      AjusteTime = ActualTime;//Copio la estructura para modificarla

      estado_ajuste++;
    break;

    case 2:
      //Modifico el día
      lcd.setCursor(6, 0);
      sprintf(BufferAux,"%02d",AjusteTime.dia);
      lcd.print(BufferAux);

      if(digitalRead(Encoder_Switch)==0)//ENTER
      {
        estado_ajuste++;
        while(digitalRead(Encoder_Switch)==0)
        {
          //Espero a que suelte el pulsador
        };
      }
      else
      {
        AjusteTime.dia=Modificar_Variable(AjusteTime.dia,1,31);

        Efecto_Titilar(6,0,2,100);
      }

    break;
      
    case 3:
      //Modifico el mes
      lcd.setCursor(9, 0);
      sprintf(BufferAux,"%02d",AjusteTime.mes);
      lcd.print(BufferAux);

      if(digitalRead(Encoder_Switch)==0)//ENTER
      {
        estado_ajuste++;
        while(digitalRead(Encoder_Switch)==0)
        {
          //Espero a que suelte el pulsador
        };
      }
      else
      {
        AjusteTime.mes=Modificar_Variable(AjusteTime.mes,1,12);
        
        Efecto_Titilar(9,0,2,100);
      }
    break;

    case 4:
      //Modifico el año
      lcd.setCursor(12, 0);
      lcd.print("20");
      lcd.setCursor(14, 0);
      sprintf(BufferAux,"%02d",AjusteTime.anio);
      lcd.print(BufferAux);

      if(digitalRead(Encoder_Switch)==0)//ENTER
      {
        estado_ajuste++;
        while(digitalRead(Encoder_Switch)==0)
        {
          //Espero a que suelte el pulsador
        };
      }
      else
      {
        AjusteTime.anio=Modificar_Variable(AjusteTime.anio,23,99);//De 2023 a 2099
        
        Efecto_Titilar(12,0,4,100);
      }
    break;

    
    case 5:
      //Modifico la hora
      lcd.setCursor(6, 1);
      sprintf(BufferAux,"%02d",AjusteTime.hora);
      lcd.print(BufferAux);

      if(digitalRead(Encoder_Switch)==0)//ENTER
      {
        estado_ajuste++;
        while(digitalRead(Encoder_Switch)==0)
        {
          //Espero a que suelte el pulsador
        };
      }
      else
      {
        AjusteTime.hora=Modificar_Variable(AjusteTime.hora,0,23);
        
        Efecto_Titilar(6,1,2,100);
      }
    break;

    
    case 6:
      //Modifico los minutos
      lcd.setCursor(9, 1);
      sprintf(BufferAux,"%02d",AjusteTime.min);
      lcd.print(BufferAux);

      if(digitalRead(Encoder_Switch)==0)//ENTER
      {
        estado_ajuste++;
        while(digitalRead(Encoder_Switch)==0)
        {
          //Espero a que suelte el pulsador
        };
      }
      else
      {
        AjusteTime.min=Modificar_Variable(AjusteTime.min,0,59);
        
        Efecto_Titilar(9,1,2,100);
      }
    break;

    case 7:
      lcd.setCursor(0, 0);
      lcd.print("  Modificacion  ");
      lcd.setCursor(0, 1);
      lcd.print("    exitosa     ");

      //Copio la hora seleccionada en la actual
      AjusteTime.seg = 0;
      ActualTime = AjusteTime;

#warning "Habria que indicar una falla si se elige una fecha que no existe como 31/02/24"
#warning "Los datos seleccionados deberian guardarse en el RTC"

      delay(3000);
      estado_ajuste=0;
      estado_menu=MENU_PRINCIPAL;
    break;
  }
}

//******************************************************************************
// Función:				Efecto_Titilar
//
// Descripción:		Efecto que hace titilar los digitos de pantalla cada 1 seg
//
// Parámetros:			int col (columna inicial)
//                  int fila (fila inicial)
//                  int digitos (digitos a hacer titilar)
//                  int delay_ms (para ajustar el tiempo en off)
//
// Valor devuelto:	void
// 
//******************************************************************************
void Efecto_Titilar(int col,int fila,int digitos,int delay_ms)
{
  if(ActualTime.seg != seg_anterior)
  {
    lcd.setCursor(col,fila);

    for(int i=0; i<digitos;i++)
      lcd.print(" ");

    delay(delay_ms);

    seg_anterior=ActualTime.seg;
  }
}

//******************************************************************************
// Función:				Modificar_Variable
//
// Descripción:		Permite modificar una variable con el encoder entre un mínimo 
//                y un máximo
//
// Parámetros:			int variable_a_modificar
//                  int minimo
//                  int maximo
//
// Valor devuelto:	int: devuelve la variable modificada
// 
//******************************************************************************
int Modificar_Variable(int variable_a_modificar, int minimo, int maximo)
{
  switch(Leer_Encoder())
  {
    default:
      break;

    case DERECHA:
      variable_a_modificar++;
      if(variable_a_modificar > maximo)
        variable_a_modificar=minimo;
    break;
      
    case IZQUIERDA:
      variable_a_modificar--;
      if(variable_a_modificar < minimo)
        variable_a_modificar=maximo;
    break;
  }

  return(variable_a_modificar);
}

//******************************************************************************
// Función:				loop
//
// Descripción:		Bucle principal
// 
//******************************************************************************
void loop(){

  char BufferAux[50];

	Reloj();

  if(estado_menu != estado_menu_anterior)
  {
    lcd.clear();
    //proximo_menu = 0;
    estado_menu_anterior = estado_menu;
    delay(300);
  }

	switch(estado_menu)
	{
		default:
		case MENU_PRINCIPAL:
        Menu_Principal();
		 break;
		
		case MENU_AJUSTE_FECHA_HORA:
      Ajuste_Reloj();
		 break;
		
		case MENU_CONFIGURACION:
      Seleccion_Configuracion();
		 break;
		
		case MENU_PROGRAMA_SEMANAL:
      Programacion_Semanal();
		 break;
		
		case MENU_FERIADOS:
      lcd.setCursor(0, 0);
      lcd.print("Menu Feriados");
      lcd.setCursor(0, 1);
      lcd.print("(a realizar) ");

      if(digitalRead(Encoder_Switch)==0)
      {
        estado_menu=MENU_PRINCIPAL;
        delay(200);
      }

		 break;
		 
	}

  codigo_hora_actual = (ActualTime.hora*4)+(ActualTime.min/15);

  if(codigo_hora_actual != codigo_hora_actual_anterior)
  {
    sprintf(BufferAux,"Codigo actual: %d\n",codigo_hora_actual);
    Serial.print(BufferAux);
    codigo_hora_actual_anterior=codigo_hora_actual;
  }

  //Sólo con esto manejo la salida
  if(((codigo_hora_actual>=Rangos_hoy[INICIO_1])&&(codigo_hora_actual<Rangos_hoy[FIN_1]))||
     ((codigo_hora_actual>=Rangos_hoy[INICIO_2])&&(codigo_hora_actual<Rangos_hoy[FIN_2]))||
     ((codigo_hora_actual>=Rangos_hoy[INICIO_3])&&(codigo_hora_actual<Rangos_hoy[FIN_3]))||
     ((codigo_hora_actual>=Rangos_hoy[INICIO_4])&&(codigo_hora_actual<Rangos_hoy[FIN_4])))
  {
    //Si se encuentra dentro de alguno de los rangos, alimento la carga
    digitalWrite(Salida_Rele,1);
  }
  else
  {
    //Si se encuentra fuera de los rangos dejo de alimentar la carga
    digitalWrite(Salida_Rele,0);
  }

}
