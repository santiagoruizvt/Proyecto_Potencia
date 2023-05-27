#include <LiquidCrystal.h> 
#include <EEPROM.h>
#include <TimerOne.h> //Para simular el reloj del RTC
#include <DS3231.h>
#include <Wire.h>

//******************************************************************************
//	ETIQUETAS
//******************************************************************************
//Descomentar para habilitar el debug por puerto serie
//#define DEBUG_SERIE

//Menu
#define MENU_PRINCIPAL          0
#define MENU_AJUSTE_FECHA_HORA  1
#define MENU_CONFIGURACION      2
#define MENU_PROGRAMA_SEMANAL   3
#define MENU_FERIADOS           4

//Dias de la semana
#define LUNES     1
#define MARTES    2
#define MIERCOLES 3
#define JUEVES    4
#define VIERNES   5
#define SABADO    6
#define DOMINGO   7

//Rangos
#define INICIO_1	0
#define FIN_1		  1
#define INICIO_2	2
#define FIN_2		  3
#define INICIO_3	4
#define FIN_3		  5
#define INICIO_4	6
#define FIN_4		  7

#define ULTIMA_POSICION_EVENTOS 55

#define CANT_RANGOS 4

//Encoder
#define DERECHA   1
#define IZQUIERDA 2
#define ENTER     3

//Para lectura de EEPROM
#define HORA    0
#define MINUTOS 1

//Para seteo de feriados
#define ES_FERIADO    1
#define NO_ES_FERIADO 0

#define TIEMPO_PARA_APAGAR_LCD  60 //en segundos

//******************************************************************************
//	VARIABLES
//******************************************************************************
int Encoder_OuputA  = 9;
int Encoder_OuputB  = 8;
int Encoder_Switch = 10;
int Salida_Rele = 13;
int Salida_Backlight = 4;

int Previous_Output;
int diadelasemana=0;
int estado_menu=0;
int estado_menu_anterior=0;
int estado_ajuste=0;
int estado_prog_sem=0;
int estado_prog_rango=0;
int estado_menu_feriados=0;
int proximo_menu=0;
int seg_anterior=0;
int rango_a_programar=0;
char Rango_num[CANT_RANGOS*2];
char Rangos_hoy[CANT_RANGOS*2];
char codigo_hora_actual=0;
char codigo_hora_actual_anterior=0;
char ultimo_dia=0;
bool modificacion_realizada=0;
bool cursor_feriado=0;
bool feriado=0;
int tiempo_sin_pulsar=TIEMPO_PARA_APAGAR_LCD;

//ESTRUCTURA DE FECHA Y HORA
struct RTC_Time
{
  //Dia de la semana
  char DoW;
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
RTC_Time AuxTime;

// Setup clock
DS3231 RTC;


//Pin usado para disparar la interrupción, la salida SQW del RTC debe conectarse a el pin a usar
#define CLINT 3

volatile byte tick = 1;

byte alarmBits = 0b00001110; // Cada un minuto

bool Century  = false;
bool h12=0;//Modo 24HS
bool PM ;

//Cantidad de dias por mes, usado para calcular la posicion en memoria
int cant_dias_mes[] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};


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

//******************************************************************************
// Función:				setup
//
// Descripción:		Configuración inicial del programa
// 
//******************************************************************************
void setup() 
{

  // Begin I2C communication
  Wire.begin();

  
  //Interrupción de RTC
  configuraInterrupcionRTC();

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
  pinMode (Salida_Backlight, OUTPUT);

  Leer_Fecha_Hora_RTC(ActualTime);

  //Para tener un timer de 1seg
  Timer1.initialize(1000000);
  Timer1.attachInterrupt(ISR_Segundo);

  Rango_num[INICIO_1]=Rango_num[FIN_1]='1';
  Rango_num[INICIO_2]=Rango_num[FIN_2]='2';
  Rango_num[INICIO_3]=Rango_num[FIN_3]='3';
  Rango_num[INICIO_4]=Rango_num[FIN_4]='4';

  //Cargo los rangos del día de hoy
  Cargar_Rangos();
  ultimo_dia=ActualTime.DoW;

  tiempo_sin_pulsar=TIEMPO_PARA_APAGAR_LCD;
}

//******************************************************************************
// Función:				  Leer_Fecha_Hora_RTC
//
// Descripción:		  Carga en la estructura la fecha y hora guardada en el RTC
//
// Parámetros:			RTC_Time Time (estructura a cargar)
// Valor devuelto:	void
// 
//******************************************************************************
void Leer_Fecha_Hora_RTC(RTC_Time& Time)
{
  //Leo los datos del RTC
  Time.DoW=RTC.getDoW();
  //FECHA
  Time.dia=RTC.getDate();
  Time.mes=RTC.getMonth(Century);
  Time.anio=RTC.getYear();
  //HORA
  Time.hora=RTC.getHour(h12, PM);
  Time.min=RTC.getMinute();
  Time.seg=RTC.getSecond();
}

//******************************************************************************
// Función:				  Cargar_Rangos
//
// Descripción:		  Carga los rangos de encendido del día de hoy
//
// Parámetros:			void
// Valor devuelto:	void
// 
//******************************************************************************
void Cargar_Rangos(void)
{
  char BufferAux[50];

  for(int i=0; i<CANT_RANGOS*2; i++)
  {
    Rangos_hoy[i]=EEPROM.read((ActualTime.DoW-1)*8+i);

    #ifdef DEBUG_SERIE
      if(i%2==0)
        Serial.print("Inicio ");
      else
        Serial.print("Fin    ");
      
      sprintf(BufferAux,"%c: %d\n",Rango_num[i],Rangos_hoy[i]);
      Serial.print(BufferAux);
    #endif
  }
}

//******************************************************************************
// Función:				  Leer_Encoder
//
// Descripción:		  Devuelve la dirección del enconder o si se apretó el 
//                  pulsador, teniendo prioridad el pulsador
//
// Parámetros:			void
// Valor devuelto:	int  IZQUIERDA(antihorario), DERECHA(horario) o ENTER
// 
//******************************************************************************
int Leer_Encoder(void)
{
  int giro_encoder=0;

  if (digitalRead(Encoder_OuputB) != Previous_Output)
  { 
    if (digitalRead(Encoder_OuputA) != Previous_Output) 
    { 
      giro_encoder = DERECHA;
      tiempo_sin_pulsar=TIEMPO_PARA_APAGAR_LCD;
    } 
    else
    {
      giro_encoder = IZQUIERDA;
      tiempo_sin_pulsar=TIEMPO_PARA_APAGAR_LCD;
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

    if(tiempo_sin_pulsar>0)  //Si está el backlight apagado, no realiza ninguna acción el enter, solo prende el backlight
      return(ENTER);  
    
    tiempo_sin_pulsar=TIEMPO_PARA_APAGAR_LCD;
  }

  return(giro_encoder);
}

//******************************************************************************
// Función:				  Programacion_Semanal
//
// Descripción:		  Se programan los rangos de encendido de cada día de la semana
//
// Parámetros:			void
// Valor devuelto:	void  
// 
//******************************************************************************
void Programacion_Semanal(void)
{

  char BufferAux[5];

  switch(estado_prog_sem)
  {
    default:
      estado_prog_sem=0;
    break;

    case 0:
      lcd.setCursor(0, 0);
      lcd.print("Seleccionar dia");
      lcd.setCursor(0, 1);
      lcd.print("D L Ma Mi J V S");

      //Selecciono DOMINGO
      //lcd.setCursor(0, 1);
      //lcd.write(byte(0));//D seleccionada
      //diadelasemana=DOMINGO;

      //delay(1000);

      diadelasemana=ActualTime.DoW;

      estado_prog_sem++;
    break;

    case 1:
      switch(Leer_Encoder())
      {
        default:
          break;

        case DERECHA:
          diadelasemana ++;
          if(diadelasemana > DOMINGO)
            diadelasemana = LUNES;

            #ifdef DEBUG_SERIE
              Serial.print("Variable: ");
              Serial.print(diadelasemana);
              Serial.print("\n");
            #endif

          delay(50);
        break;
          
        case IZQUIERDA:
          diadelasemana--;
          if(diadelasemana < LUNES)
            diadelasemana = DOMINGO;

            #ifdef DEBUG_SERIE
              Serial.print("Variable: ");
              Serial.print(diadelasemana);
              Serial.print("\n");
            #endif

          delay(50);
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
      default:
        diadelasemana=DOMINGO;
        lcd.print("DOMINGO");
      break;

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
      Cargar_Rangos();
      modificacion_realizada=1;
    break;
  }
}


//******************************************************************************
// Función:				  Programar_Rango
//
// Descripción:		  Se programan las horas y minutos de cada rango
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
    default:
      estado_prog_rango=0;
    break;

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
        default:
        break;
        
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
        default:
        break;

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
// Función:				  Leer_Rangos_EEPROM
//
// Descripción:		  Se leen las horas o minutos (según se solicite) guardado 
//                  en la EEPROM del día solicitado en el rango solicitado.
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
  direccion_eeprom= (dia-1)*8+rango;
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
// Función:				  Escribir_Rangos_EEPROM
//
// Descripción:		  Se escribe en la EEPROM el código generado por las horas y 
//                  minutos en la dirección generada por el día y rango indicados 
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

  direccion_eeprom= (dia-1)*8+rango;

  codigo_hora=((hora*4)+(min/15));

  EEPROM.write(direccion_eeprom,codigo_hora);

}

//******************************************************************************
// Función:				  Seleccion_dias
//
// Descripción:		  Se utiliza el encoder para seleccionar el día de la semana
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
    default:
      diadelasemana=DOMINGO;
    break;

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
// Función:				  Menu_Principal
//
// Descripción:		  Muestra la fecha y la hora y utiliza el encoder para elegir
//                  ajustar la hora/fecha o configurar la programación
//
// Parámetros:			void
//
// Valor devuelto:	void
// 
//******************************************************************************
void Menu_Principal(void)
{

  char BufferAux[16];

  //Muestro la fecha y hora actual

  lcd.setCursor(0, 0);
  switch(ActualTime.DoW)
  {
    default: 
      //Distingo si me trae basura
      lcd.print("?");
    break;

    case DOMINGO:
      lcd.print("D");
    break;
    
    case LUNES:
      lcd.print("L");
    break;
    
    case MARTES:
      lcd.print("M");
    break;
    
    case MIERCOLES:
      lcd.print("X");
    break;
    
    case JUEVES:
      lcd.print("J");
    break;
    
    case VIERNES:
      lcd.print("V");
    break;
    
    case SABADO:
      lcd.print("S");
    break;
  }

  lcd.setCursor(1, 0);
  sprintf(BufferAux," %02d/%02d/%02d %02d:%02d",ActualTime.dia,ActualTime.mes,ActualTime.anio,ActualTime.hora,ActualTime.min);
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

  #ifdef DEBUG_SERIE
    Serial.print("proximo_menu:");
    Serial.println(proximo_menu);
  #endif
  
  Seleccion_Principal();

  //Hago titilar el ':' para que se vea que está vivo 
  Efecto_Titilar(13,0,1,50);
}

//******************************************************************************
// Función:				  Seleccion_Principal
//
// Descripción:		  Dibuja una flecha en el menú seleccionado dentro del 
//                  principal. Si se presiona ENTER, entra a ese menú.
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
// Función:				  Seleccion_Configuracion
//
// Descripción:		  Muestra el menú Configuración y dibuja una flecha en el menu 
//                  seleccionado. Si se presiona ENTER, entra a ese menu.
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
// Función:				  Reloj
//
// Descripción:		  Reloj en base al timer de 1seg para simular el RTC
//
// Parámetros:			void
//
// Valor devuelto:	void
// 
//******************************************************************************
  /*
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
*/

//******************************************************************************
// Función:				  ISR_Segundo
//
// Descripción:		  Timer de 1seg
//
// Parámetros:			void
//
// Valor devuelto:	void
// 
//******************************************************************************
void ISR_Segundo(void){

  ActualTime.seg++;
  
  if(tiempo_sin_pulsar>0)
    tiempo_sin_pulsar--;

}

//******************************************************************************
// Función:				  Ajuste_Reloj
//
// Descripción:		  Menú para modificar la hora y fecha actual usando el encoder
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
    default:
      estado_ajuste=0;
    break;

    case 0:
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
      lcd.print("Dia: ");

      switch(ActualTime.DoW)
      {
        default: 
          //Distingo si me trae basura
          lcd.print("?? ");
        break;

        case DOMINGO:
          lcd.print("Do ");
        break;
        
        case LUNES:
          lcd.print("Lu ");
        break;
        
        case MARTES:
          lcd.print("Ma ");
        break;
        
        case MIERCOLES:
          lcd.print("Mi ");
        break;
        
        case JUEVES:
          lcd.print("Ju ");
        break;
        
        case VIERNES:
          lcd.print("Vi ");
        break;
        
        case SABADO:
          lcd.print("Sa ");
        break;
      }

      sprintf(BufferAux,"%02d/%02d/%02d",ActualTime.dia,ActualTime.mes,ActualTime.anio);
      lcd.print(BufferAux);

      lcd.setCursor(0, 1);
      sprintf(BufferAux,"Hora:   %02d:%02d     ",ActualTime.hora,ActualTime.min);
      lcd.print(BufferAux);

      AuxTime = ActualTime;//Copio la estructura para modificarla

      estado_ajuste++;
    break;

    
    case 2:
      //Modifico el día de la semana
      lcd.setCursor(5, 0);
      
      switch(AuxTime.DoW)
      {
        default: 
          //Distingo si me trae basura
          lcd.print("?? ");
        break;

        case DOMINGO:
          lcd.print("Do ");
        break;
        
        case LUNES:
          lcd.print("Lu ");
        break;
        
        case MARTES:
          lcd.print("Ma ");
        break;
        
        case MIERCOLES:
          lcd.print("Mi ");
        break;
        
        case JUEVES:
          lcd.print("Ju ");
        break;
        
        case VIERNES:
          lcd.print("Vi ");
        break;
        
        case SABADO:
          lcd.print("Sa ");
        break;
      }

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
        AuxTime.DoW=Modificar_Variable(AuxTime.DoW,1,7);

        Efecto_Titilar(5,0,2,100);
      }
    break;

    case 3:
      //Modifico el día
      lcd.setCursor(8, 0);
      sprintf(BufferAux,"%02d",AuxTime.dia);
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
        AuxTime.dia=Modificar_Variable(AuxTime.dia,1,31);

        Efecto_Titilar(8,0,2,100);
      }
    break;
      
    case 4:
      //Modifico el mes
      lcd.setCursor(11, 0);
      sprintf(BufferAux,"%02d",AuxTime.mes);
      lcd.print(BufferAux);

      if(digitalRead(Encoder_Switch)==0)//ENTER
      {
        
        if(Validar_Fecha(AuxTime.dia,AuxTime.mes)==false)
        {
          lcd.setCursor(0, 0);
          lcd.print("La fecha elegida");
          lcd.setCursor(0, 1);
          lcd.print("   no existe    ");
          delay(3000);
          estado_ajuste=0;
          estado_menu=MENU_PRINCIPAL;
        }
        else
          estado_ajuste++;
        
        while(digitalRead(Encoder_Switch)==0)
        {
          //Espero a que suelte el pulsador
        };
      }
      else
      {
        AuxTime.mes=Modificar_Variable(AuxTime.mes,1,12);
        
        Efecto_Titilar(11,0,2,100);
      }


    break;

    case 5:
      //Modifico el año
      lcd.setCursor(14, 0);
      sprintf(BufferAux,"%02d",AuxTime.anio);
      lcd.print(BufferAux);

      if(digitalRead(Encoder_Switch)==0)//ENTER
      {
        
        if((AuxTime.dia==29)&&(AuxTime.mes==2)&&(AuxTime.anio%4!=0))
        {
          lcd.setCursor(0, 0);
          lcd.print("La fecha elegida");
          lcd.setCursor(0, 1);
          lcd.print("   no existe    ");
          delay(3000);
          estado_ajuste=0;
          estado_menu=MENU_PRINCIPAL;
        }
        else
          estado_ajuste++;
          
        while(digitalRead(Encoder_Switch)==0)
        {
          //Espero a que suelte el pulsador
        };
      }
      else
      {
        AuxTime.anio=Modificar_Variable(AuxTime.anio,0,99);//De 2023 a 2099
        
        Efecto_Titilar(14,0,2,100);
      }
    break;

    
    case 6:
      //Modifico la hora
      lcd.setCursor(8, 1);
      sprintf(BufferAux,"%02d",AuxTime.hora);
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
        AuxTime.hora=Modificar_Variable(AuxTime.hora,0,23);
        
        Efecto_Titilar(8,1,2,100);
      }
    break;

    
    case 7:
      //Modifico los minutos
      lcd.setCursor(11, 1);
      sprintf(BufferAux,"%02d",AuxTime.min);
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
        AuxTime.min=Modificar_Variable(AuxTime.min,0,59);
        
        Efecto_Titilar(11,1,2,100);
      }
    break;

    case 8:
  
      lcd.setCursor(0, 0);
      lcd.print("  Modificacion  ");
      lcd.setCursor(0, 1);
      lcd.print("    exitosa     ");
      
      //Copio la hora seleccionada en la actual
      AuxTime.seg = 0;
      ActualTime = AuxTime;

      //Como la fecha es válida guardo fecha y hora en el RTC
      RTC.setSecond(0);
      RTC.setMinute(ActualTime.min);
      RTC.setHour(ActualTime.hora);
      RTC.setDoW(ActualTime.DoW);
      RTC.setDate(ActualTime.dia);
      RTC.setMonth(ActualTime.mes);
      RTC.setYear(ActualTime.anio);
      RTC.setClockMode(h12);

      delay(3000);
      estado_ajuste=0;
      estado_menu=MENU_PRINCIPAL;
      modificacion_realizada=1;
    break;
  }
}


//******************************************************************************
// Función:				  Validar_Fecha
//
// Descripción:		  Verifica que la combinación dia/mes exista
//
// Parámetros:			dia y mes
//
// Valor devuelto:	bool: true (fecha valida) false (fecha inexistente)
// 
//******************************************************************************
bool Validar_Fecha(int dia,int mes)
{
  if(dia>cant_dias_mes[mes-1])
    return false;
  else 
    return true;
}

//******************************************************************************
// Función:				  Efecto_Titilar
//
// Descripción:		  Efecto que hace titilar los digitos de pantalla cada 1 seg
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
// Función:				  Modificar_Variable
//
// Descripción:		  Permite modificar una variable con el encoder entre un  
//                  mínimo y un máximo
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
// Función:				  Menu_Feriados
//
// Descripción:		  Muestra los feriados y permite agregar o quitar.
//
// Parámetros:			void
//
// Valor devuelto:	void
// 
//******************************************************************************
void Menu_Feriados(void)
{
  char BufferAux[30];

  switch(estado_menu_feriados)
  {
    default:
      estado_menu_feriados=0;
    break;

    case 0:
      //Parto desde la hora actual
      AuxTime = ActualTime;

      estado_menu_feriados++;
    break;

    case 1:

      switch(Leer_Encoder())
      {
        default:
          break;

        case DERECHA:
          AuxTime.dia++;
          if(AuxTime.dia > cant_dias_mes[AuxTime.mes-1])
          {
            AuxTime.dia=1;
            AuxTime.mes++;

            if(AuxTime.mes>12)
              AuxTime.mes=1;
          }
        break;
          
        case IZQUIERDA:
          AuxTime.dia--;
          if(AuxTime.dia < 1)
          {
            AuxTime.mes--;

            if(AuxTime.mes<1)
              AuxTime.mes=12;

            AuxTime.dia=cant_dias_mes[AuxTime.mes-1];
          }
        break;

        case ENTER:
          estado_menu_feriados++;
          lcd.setCursor(0, 1);
          lcd.print(">");
          lcd.setCursor(0, 0);
          sprintf(BufferAux,"%02d/%02d",AuxTime.dia,AuxTime.mes);
          lcd.print(BufferAux);
          return;
        break;
      }
      
      lcd.setCursor(0, 0);
      sprintf(BufferAux,"%02d/%02d",AuxTime.dia,AuxTime.mes);
      lcd.print(BufferAux);

      lcd.setCursor(6, 0);
      if(verificaFeriado(AuxTime.dia,AuxTime.mes) == true)
      {
        lcd.print("es feriado");
        lcd.setCursor(1, 1);
        lcd.print("Eliminar");
        feriado = ES_FERIADO;
      }
      else
      {
        lcd.print("          ");
        lcd.setCursor(1, 1);
        lcd.print("Agregar ");
        feriado = NO_ES_FERIADO;
      }
      cursor_feriado=0;
      
      lcd.setCursor(11, 1);
      lcd.print("Salir");

      //Hago titilar a los números
      Efecto_Titilar(0,0,5,100);
      lcd.setCursor(2, 0);
      lcd.print("/");

    break;

    case 2:
      //Elijo entre Agregar/Eliminar o Salir
      switch(Leer_Encoder())
      {
        default:
          break;

        case DERECHA:
          cursor_feriado=1;
          lcd.setCursor(0, 1);
          lcd.print(" ");
          lcd.setCursor(10, 1);
          lcd.print(">");
        break;
          
        case IZQUIERDA:
          cursor_feriado=0;
          lcd.setCursor(0, 1);
          lcd.print(">");
          lcd.setCursor(10, 1);
          lcd.print(" ");
        break;

        case ENTER:
          estado_menu_feriados++;
        break;
      }
    break;

    case 3:
      if(cursor_feriado == 0)
      {
        modificacion_realizada=true;
        lcd.setCursor(0, 0);
        // Modifico el estado actual
        if(feriado== ES_FERIADO)
        {
          seteaFeriado(AuxTime.dia, AuxTime.mes, NO_ES_FERIADO);
          sprintf(BufferAux,"%02d/%02d se quita  ",AuxTime.dia,AuxTime.mes);
          lcd.print(BufferAux);
          lcd.setCursor(0, 1);
          lcd.print("de los feriados  ");
        }
        else
        {
          seteaFeriado(AuxTime.dia, AuxTime.mes, ES_FERIADO); 
          sprintf(BufferAux,"%02d/%02d se agrega ",AuxTime.dia,AuxTime.mes);
          lcd.print(BufferAux);
          lcd.setCursor(0, 1);
          lcd.print("a los feriados   ");
        }
        delay(3000);
        //Me permite seguir eligiendo feriados
        estado_menu_feriados=1;
        lcd.clear();
      }
      else
      {
        //Presionó Salir
        estado_menu_feriados=0;
        estado_menu=MENU_PRINCIPAL;
      }
      
    break;

  }
}

//******************************************************************************
// Función:				  verificaFeriado
//
// Descripción:		  Chequea en la EEPROM si hoy es feriado
//
// Parámetros:			int dia, int mes
//
// Valor devuelto:	bool: true (es feriado), false (no es feriado)
// 
//******************************************************************************
bool verificaFeriado(int dia, int mes) 
{
  //Obtenemos el dia del año en formato "M|D"
  int indexMonthEeprom = mes - 1;
  int daysAccum = 0;

  for(int i = 0; i < indexMonthEeprom; i++) 
  {
    daysAccum += cant_dias_mes[i];
  }

  daysAccum += ULTIMA_POSICION_EVENTOS + dia;

  if(EEPROM.read(daysAccum) == ES_FERIADO) 
  {
    #ifdef DEBUG_SERIE
      Serial.print("Feriado detectado\n");
    #endif
    return true;
  }
  else 
  {
    return false;
  }
}

//******************************************************************************
// Función:				  seteaFeriado
//
// Descripción:		  Setea como feriado el conjunto dia/mes recibido guardandolo
//                  en EEPROM 
//
// Parámetros:			int dia, int mes, bool feriado (para indicar si es o no es) 
//
// Valor devuelto:	void
// 
//******************************************************************************
void seteaFeriado(int dia, int mes, bool feriado) 
{
  int indexMonthEeprom = mes - 1;
  int daysAccum = 0;

  for(int i = 0; i < indexMonthEeprom; i++) 
  {
    daysAccum += cant_dias_mes[i];
  }

  daysAccum += ULTIMA_POSICION_EVENTOS + dia;
  EEPROM.write(daysAccum, feriado);
}

//******************************************************************************
// Función:				  configuraInterrupcionRTC
//
// Descripción:		  Configura la Interrupción del RTC
//
// Parámetros:			void
//
// Valor devuelto:	void
// 
//******************************************************************************
void configuraInterrupcionRTC (void) 
{
  // Set alarm 1 to fire at one-second intervals
  RTC.turnOffAlarm(1);
  RTC.setA1Time(0, 0, 0, 0, alarmBits, false, false, false);
  // enable Alarm 1 interrupts
  RTC.turnOnAlarm(1);
  // clear Alarm 1 flag
  RTC.checkIfAlarm(1);

  pinMode(CLINT, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(CLINT), isr_TickTock, FALLING);

  // Use builtin LED to blink
  //pinMode(LED_BUILTIN, OUTPUT);
}

//******************************************************************************
// Función:				  isr_TickTock
//
// Descripción:		  Interrupción del RTC
//
// Parámetros:		  void
//
// Valor devuelto:	void
// 
//******************************************************************************
void isr_TickTock(void) {
  // interrupt signals to loop
  tick = 1;
  return;
}

//******************************************************************************
// Función:				loop
//
// Descripción:		Bucle principal del programa
// 
//******************************************************************************
void loop(){

  char BufferAux[50];

	//Reloj();

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
      Menu_Feriados();
		break;
		 
	}

  if((tick)||(modificacion_realizada))  
  {
    //Aca entra cada un minuto o cuando se haga una modificación en el Reloj o en los Rangos    
    tick = 0;
    modificacion_realizada=0;

    Leer_Fecha_Hora_RTC(ActualTime);

    //Si cambia el día, leo los rangos permitidos
    if(ultimo_dia != ActualTime.DoW)
    {
      Cargar_Rangos();
      ultimo_dia=ActualTime.DoW;
    }  

    codigo_hora_actual = (ActualTime.hora*4)+(ActualTime.min/15);

    if(codigo_hora_actual != codigo_hora_actual_anterior)
    {
      #ifdef DEBUG_SERIE
        sprintf(BufferAux,"Codigo actual: %d\n",codigo_hora_actual);
        Serial.print(BufferAux);
      #endif
      codigo_hora_actual_anterior=codigo_hora_actual;
    }

    //Sólo con esto manejo la salida

    if((((codigo_hora_actual>=Rangos_hoy[INICIO_1])&&(codigo_hora_actual<Rangos_hoy[FIN_1]))||
        ((codigo_hora_actual>=Rangos_hoy[INICIO_2])&&(codigo_hora_actual<Rangos_hoy[FIN_2]))||
        ((codigo_hora_actual>=Rangos_hoy[INICIO_3])&&(codigo_hora_actual<Rangos_hoy[FIN_3]))||
        ((codigo_hora_actual>=Rangos_hoy[INICIO_4])&&(codigo_hora_actual<Rangos_hoy[FIN_4])))&&(!verificaFeriado(ActualTime.dia,ActualTime.mes)))
    {
      //Si no es feriado y se encuentra dentro de alguno de los rangos programados, alimento la carga
      digitalWrite(Salida_Rele,1);
    }
    else
    {
      //Si se encuentra fuera de los rangos dejo de alimentar la carga
      digitalWrite(Salida_Rele,0);
    }

    // Clear Alarm 1 flag
    RTC.checkIfAlarm(1);
  }

  //Controlo el backlight del lcd
  if(!tiempo_sin_pulsar)
    digitalWrite(Salida_Backlight,1);//Si pasa cierto tiempo en el que no se tocó el encoder, se apaga el display
  else
    digitalWrite(Salida_Backlight,0);//Si se toca el encoder, se enciende
  
}
