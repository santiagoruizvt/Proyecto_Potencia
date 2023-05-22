#include <LiquidCrystal.h> 
#include <TimerOne.h> //Para simular el reloj del RTC

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


//Encoder
#define DERECHA   1
#define IZQUIERDA 2
#define ENTER     3

int Encoder_OuputA  = 9;
int Encoder_OuputB  = 8;
int Encoder_Switch = 10;

int Previous_Output;
int diadelasemana=0;
int estado_menu=0;
int estado_menu_anterior=0;
int estado_ajuste=0;
int estado_prog_sem=0;
int proximo_menu=0;
int seg_anterior=0;

//*******ESTRUCTURA DE FECHA Y HORA*****************************************
struct RTC_Time
{
  //FECHA
  int dia; //[1,31]
  int mes; //[1,12]
  int anio; //[0,255]
  //HORA
  int seg; //[0,59]
  int min; //[0,59]
  int hora; //[0,23]
};

RTC_Time ActualTime;
RTC_Time AjusteTime;

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

const int rs = 7, en = 6, d4 = 5, d5 = 4, d6 = 3, d7 = 2; //Mention the pin number for LCD connection
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
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

  
}


void Menu_Principal(){

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


  Serial.print("proximo_menu:");
  Serial.println(proximo_menu);
  
  Seleccion_Principal();
}

void Seleccion_Principal()
{
  //Dibuja una flecha en el menu seleccionado
  //Si se presiona ENTER, entra a ese menu

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

void Seleccion_Configuracion()
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

void Reloj(){
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

void ISR_Segundo(){
  ActualTime.seg++;
}


void Ajuste_Reloj()
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

      delay(3000);
      estado_ajuste=0;
      estado_menu=MENU_PRINCIPAL;
    break;
  }
}

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

int Leer_Encoder(){
  
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
    return(ENTER);  

  return(giro_encoder);
}

void Programacion_Semanal() {

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

    lcd.setCursor(10, 0);
    lcd.print("Rango1");

    estado_prog_sem++;
    break;

    case 3:
    lcd.setCursor(0, 1);
    lcd.print("Encendido: 07:30");
    Efecto_Titilar(11,1,2,100);
    break;
  }
}

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


void loop(){

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
}
