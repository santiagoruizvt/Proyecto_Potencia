#include <string.h>
#include "SoftwareSerial.h"
#include <DS3231.h>
#include <Wire.h>
#include <EEPROM.h>
#include <SPI.h>
#include <SD.h>

//Cantidad de dias por mes, usado para calcular la posicion en memoria
int meses[] = { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

//Indice del inicio del primer evento de cada dia.
int eventos[] = {0, 7, 16, 24, 32, 40, 48};

//Decodificador de horas, segun el valor guardado en la EEPROM para la parte eventos, se obtiene la hora y los minutos
int horas[96][2] = {{0, 0}, {0, 15}, {0, 30}, {0, 45}, {1, 0}, {1, 15}, {1, 30}, {1, 45}, {2, 0}, {2, 15}, {2, 30}, {2, 45}, {3, 0}, {3, 15}, {3, 30}, {3, 45},
  {4, 0}, {4, 15}, {4, 30}, {4, 45}, {5, 0}, {5, 15}, {5, 30}, {5, 45}, {6, 0}, {6, 15}, {6, 30}, {6, 45}, {7, 0}, {7, 15}, {7, 30}, {7, 45},
  {8, 0}, {8, 15}, {8, 30}, {8, 45}, {9, 0}, {9, 15}, {9, 30}, {9, 45}, {10, 0}, {10, 15}, {10, 30}, {10, 45}, {11, 0}, {11, 15}, {11, 30},
  {11, 45}, {12, 0}, {12, 15}, {12, 30}, {12, 45}, {13, 0}, {13, 15}, {13, 30}, {13, 45}, {14, 0}, {14, 15}, {14, 30}, {14, 45}, {15, 0},
  {15, 15}, {15, 30}, {15, 45}, {16, 0}, {16, 15}, {16, 30}, {16, 45}, {17, 0}, {17, 15}, {17, 30}, {17, 45}, {18, 0}, {18, 15}, {18, 30},
  {18, 45}, {19, 0}, {19, 15}, {19, 30}, {19, 45}, {20, 0}, {20, 15}, {20, 30}, {20, 45}, {21, 0}, {21, 15}, {21, 30}, {21, 45}, {22, 0},
  {22, 15}, {22, 30}, {22, 45}, {23, 0}, {23, 15}, {23, 30}, {23, 45}
};

// Setup clock
DS3231 RTC;

//Pin usado para disparar la interrupción, la salida SQW del RTC debe conectarse a el pin a usar
#define CLINT 3

volatile byte tick = 1;

byte alarmBits = 0b00001110; // Cada un minuto

byte Year ;
byte Month ;
byte Date ;
byte DoW ;
byte Hour ;
byte Minute ;
byte Second ;
bool Century  = false;
bool h12 ;
bool PM ;

bool feriado = false;

//SD
File fileSD;

void setup() {
  // Begin I2C communication
  Wire.begin();

  // Begin Serial communication
  Serial.begin(9600);
  while (!Serial);

  Serial.println( crearActualizarEvento(2, 1, 3, 15, 3, 30) );
  Serial.println( fechaHoraEnEvento() );

  //DESCOMENTAR: Para setear la fecha y hora actual
  //RTC.setEpoch(1684723490);


  //Interrupción de RTC
  configuraInterrupcionRTC();

  cargarFeriadosDeSD();

}

void loop() {

  if (tick) {
    tick = 0;

    //Si NO es feriado continua
    if (!verificaFeriado()) {
      //Si la fecha y Hora del RTC esta en el horario definido por un evento (INICIO - FIN) activa salida
      if (fechaHoraEnEvento()) {
        /*
           TO DO: SALIDA DIGITAL ON
        */
      } else {
        /*
           TO DO: SALIDA DIGITAL OFF
        */
      }
    }

    // Clear Alarm 1 flag
    RTC.checkIfAlarm(1);
  }

  /*
    Serial.print(RTC.getYear(), DEC);
    Serial.print("-");
    Serial.print(RTC.getMonth(Century), DEC);
    Serial.print("-");
    Serial.print(RTC.getDate(), DEC);
    Serial.print(" ");
    Serial.print(RTC.getHour(h12, PM), DEC); //24-hr
    Serial.print(":");
    Serial.print(RTC.getMinute(), DEC);
    Serial.print(":");
    Serial.println(RTC.getSecond(), DEC);
    Serial.print(" - ");
    Serial.println(RTC.getDoW(), DEC);
    Serial.println("---");
    //Serial.println(EEPROM.read(1));
  */

}

bool verificaFeriado() {

  //Obtenemos el dia del año en formato "M|D"
  int indexMonthEeprom = RTC.getMonth(Century) - 1;
  int daysAccum = 0;
  for (int i = 0; i < indexMonthEeprom; i++) {
    daysAccum += meses[i];
  }

  daysAccum += 55 + RTC.getDate();

  if ( EEPROM.read(daysAccum) == 1 ) {
    return true;
  } else {
    return false;
  }

}

void seteaFeriado(int dia, int mes) {

  int indexMonthEeprom = RTC.getMonth(Century) - 1;
  int daysAccum = 0;

  for (int i = 0; i < indexMonthEeprom; i++) {
    daysAccum += meses[i];
  }

  daysAccum += 55 + RTC.getDate();
  EEPROM.write(daysAccum, 1);

}


int crearActualizarEvento(int day, int event, int horaInicio, int minutosInicio, int horaFin, int minutosFin) {
  /*

     Domingo: 1
     Lunes: 2
     Martes: 3
     Miercoles: 4
     Jueves: 5
     Viernes: 6
     Sabado: 7

  */

  int indexEvent = eventos[day - 1];

  switch (event) {
    case 1:
      indexEvent += 0;
      break;
    case 2:
      indexEvent += 2;
      break;
    case 3:
      indexEvent += 4;
      break;
    case 4:
      indexEvent += 6;
      break;
  }

  int horaFechaInicial = (horaInicio * 4) + (minutosInicio / 15);
  int horaFechaFinal = (horaFin * 4) + (minutosFin / 15);

  EEPROM.write(indexEvent, horaFechaInicial);
  EEPROM.write(indexEvent + 1, horaFechaFinal);

  return 1;

}

bool fechaHoraEnEvento() {

  int indexEvent = eventos[RTC.getDoW() - 1];

  for (int i = 0; i < 7; i = i + 2) {
    int year = 2000 + RTC.getYear();

    //fechas horas codificadas
    int horaInicialCodificada =  EEPROM.read(indexEvent + i);
    int horaFinalCodificada =  EEPROM.read(indexEvent + i + 1);

    //fechas horas de evento decodificadas
    int horaInicial = horas[horaInicialCodificada][0];
    int minutosInicial = horas[horaInicialCodificada][1];

    int horaFinal = horas[horaFinalCodificada][0];
    int minutosFinal = horas[horaFinalCodificada][1];

    DateTime fechaHoraInicial(year, RTC.getMonth(Century), RTC.getDate(), horaInicial, minutosInicial, 0);
    DateTime fechaHoraFinal(year, RTC.getMonth(Century), RTC.getDate(), horaFinal, minutosFinal, 0);
    DateTime now = RTClib::now().unixtime();

    // Obtener los valores de tiempo en segundos
    uint32_t nowUnix = now.unixtime();
    uint32_t inicialUnix = fechaHoraInicial.unixtime();
    uint32_t finalUnix = fechaHoraFinal.unixtime();

    if ( ( nowUnix > inicialUnix ) && ( nowUnix < finalUnix ) ) {
      return true;
    }

  }

  return false;

}

void configuraInterrupcionRTC () {

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
  pinMode(LED_BUILTIN, OUTPUT);
}

bool cargarFeriadosDeSD() {

  if (!SD.begin(4)) {
    //Si falla la inicialización de la SD devuelve false
    while (1);
    return false;
  }

  //Busca el archivo feriados.txt de la SD
  fileSD = SD.open("feriados.txt", FILE_READ);
  if (fileSD) {
    String content = fileSD.readString(); // Leer el contenido completo del archivo en una cadena
    fileSD.close(); // Cerrar el archivo

    //Convetimos la linea con los feriados en array de char
    int str_len = content.length() + 1;
    char char_array[str_len];
    content.toCharArray(char_array, str_len);
    
    char *token;
    char *token_dos;
    char *mystring = char_array;
    const char *delimiter =",";
    const char *delimiter_dos ="/";

   token = strtok(mystring, delimiter);

   while (token != NULL) {
      token_dos = strtok(token, delimiter_dos);
                Serial.println(token);
       while (token_dos != NULL) {
          Serial.println(token_dos);
          token_dos=strtok(NULL, delimiter_dos);
       }
       
      token=strtok(NULL, delimiter);
   }
   
  } else {
    //si no puede leer o abrir el archivo devuelve false
    return false;
  }
}

//Funciones de auxiliares

// Cuenta la cantidad de veces que aparece el caracter
int countOccurrences(String str, char character) {
  int count = 0;
  for (int i = 0; i < str.length(); i++) {
    if (str.charAt(i) == character) {
      count++;
    }
  }
  return count;
}

void isr_TickTock() {
  // interrupt signals to loop
  tick = 1;
  return;
}
