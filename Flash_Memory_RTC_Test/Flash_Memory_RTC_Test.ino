#include <string.h>
#include "SoftwareSerial.h"
#include <DS3231.h>
#include <Wire.h>
#include <EEPROM.h>

/*

  0  DOMINGO  INICIO_1
  1  DOMINGO  FIN_1
  2  DOMINGO  INICIO_2
  3  DOMINGO  FIN_2
  4  DOMINGO  INICIO_3
  5  DOMINGO  FIN_3
  6  DOMINGO  INICIO_4
  7 LUNES FIN_4
  8 LUNES INICIO_1
  9 LUNES FIN_1
  10  LUNES INICIO_2
  11  LUNES FIN_2
  12  LUNES INICIO_3
  13  LUNES FIN_3
  14  LUNES INICIO_4
  15  LUNES FIN_4
  16  MARTES  INICIO_1
  17  MARTES  FIN_1
  18  MARTES  INICIO_2
  19  MARTES  FIN_2
  20  MARTES  INICIO_3
  21  MARTES  FIN_3
  22  MARTES  INICIO_4
  23  MARTES  FIN_4
  24  MIERCOLES INICIO_1
  25  MIERCOLES FIN_1
  26  MIÉRCOLES INICIO_2
  27  MIÉRCOLES FIN_2
  28  MIÉRCOLES INICIO_3
  29  MIÉRCOLES FIN_3
  30  MIÉRCOLES INICIO_4
  31  MIERCOLES FIN_4
  32  JUEVES  INICIO_1
  33  JUEVES  FIN_1
  34  JUEVES  INICIO_2
  35  JUEVES  FIN_2
  36  JUEVES  INICIO_3
  37  JUEVES  FIN_3
  38  JUEVES  INICIO_4
  39  JUEVES  FIN_4
  40  VIERNES INICIO_1
  41  VIERNES FIN_1
  42  VIERNES INICIO_2
  43  VIERNES FIN_2
  44  VIERNES INICIO_3
  45  VIERNES FIN_3
  46  VIERNES INICIO_4
  47  VIERNES FIN_4
  48  SABADO  INICIO_1
  49  SABADO  FIN_1
  50  SÁBADO  INICIO_2
  51  SÁBADO  FIN_2
  52  SÁBADO  INICIO_3
  53  SÁBADO  FIN_3
  54  SÁBADO  INICIO_4
  55  SABADO  FIN_4
  56  - 1/1/2023
  57  - 2/1/2023
  58  - 3/1/2023
  59  - 4/1/2023
  60  - 5/1/2023
  61  - 6/1/2023
  62  - 7/1/2023
  63  - 8/1/2023
  64  - 9/1/2023
  65  - 10/1/2023
  66  - 11/1/2023
  67  - 12/1/2023
  68  - 13/1/2023
  69  - 14/1/2023
  70  - 15/1/2023
  71  - 16/1/2023
  72  - 17/1/2023
  73  - 18/1/2023
  74  - 19/1/2023
  75  - 20/1/2023
  76  - 21/1/2023
  77  - 22/1/2023
  78  - 23/1/2023
  79  - 24/1/2023
  80  - 25/1/2023
  81  - 26/1/2023
  82  - 27/1/2023
  83  - 28/1/2023
  84  - 29/1/2023
  85  - 30/1/2023
  86  - 31/1/2023
  87  - 1/2/2023
  88  - 2/2/2023
  89  - 3/2/2023
  90  - 4/2/2023
  91  - 5/2/2023
  92  - 6/2/2023
  93  - 7/2/2023
  94  - 8/2/2023
  95  - 9/2/2023
  96  - 10/2/2023
  97  - 11/2/2023
  98  - 12/2/2023
  99  - 13/2/2023
  100 - 14/2/2023
  101 - 15/2/2023
  102 - 16/2/2023
  103 - 17/2/2023
  104 - 18/2/2023
  105 - 19/2/2023
  106 - 20/2/2023
  107 - 21/2/2023
  108 - 22/2/2023
  109 - 23/2/2023
  110 - 24/2/2023
  111 - 25/2/2023
  112 - 26/2/2023
  113 - 27/2/2023
  114 - 28/2/2023
  115 - 29/2/2023
  116 - 1/3/2023
  117 - 2/3/2023
  118 - 3/3/2023
  119 - 4/3/2023
  120 - 5/3/2023
  121 - 6/3/2023
  122 - 7/3/2023
  123 - 8/3/2023
  124 - 9/3/2023
  125 - 10/3/2023
  126 - 11/3/2023
  127 - 12/3/2023
  128 - 13/3/2023
  129 - 14/3/2023
  130 - 15/3/2023
  131 - 16/3/2023
  132 - 17/3/2023
  133 - 18/3/2023
  134 - 19/3/2023
  135 - 20/3/2023
  136 - 21/3/2023
  137 - 22/3/2023
  138 - 23/3/2023
  139 - 24/3/2023
  140 - 25/3/2023
  141 - 26/3/2023
  142 - 27/3/2023
  143 - 28/3/2023
  144 - 29/3/2023
  145 - 30/3/2023
  146 - 31/3/2023
  147 - 1/4/2023
  148 - 2/4/2023
  149 - 3/4/2023
  150 - 4/4/2023
  151 - 5/4/2023
  152 - 6/4/2023
  153 - 7/4/2023
  154 - 8/4/2023
  155 - 9/4/2023
  156 - 10/4/2023
  157 - 11/4/2023
  158 - 12/4/2023
  159 - 13/4/2023
  160 - 14/4/2023
  161 - 15/4/2023
  162 - 16/4/2023
  163 - 17/4/2023
  164 - 18/4/2023
  165 - 19/4/2023
  166 - 20/4/2023
  167 - 21/4/2023
  168 - 22/4/2023
  169 - 23/4/2023
  170 - 24/4/2023
  171 - 25/4/2023
  172 - 26/4/2023
  173 - 27/4/2023
  174 - 28/4/2023
  175 - 29/4/2023
  176 - 30/4/2023
  177 - 1/5/2023
  178 - 2/5/2023
  179 - 3/5/2023
  180 - 4/5/2023
  181 - 5/5/2023
  182 - 6/5/2023
  183 - 7/5/2023
  184 - 8/5/2023
  185 - 9/5/2023
  186 - 10/5/2023
  187 - 11/5/2023
  188 - 12/5/2023
  189 - 13/5/2023
  190 - 14/5/2023
  191 - 15/5/2023
  192 - 16/5/2023
  193 - 17/5/2023
  194 - 18/5/2023
  195 - 19/5/2023
  196 - 20/5/2023
  197 - 21/5/2023
  198 - 22/5/2023
  199 - 23/5/2023
  200 - 24/5/2023
  201 - 25/5/2023
  202 - 26/5/2023
  203 - 27/5/2023
  204 - 28/5/2023
  205 - 29/5/2023
  206 - 30/5/2023
  207 - 31/5/2023
  208 - 1/6/2023
  209 - 2/6/2023
  210 - 3/6/2023
  211 - 4/6/2023
  212 - 5/6/2023
  213 - 6/6/2023
  214 - 7/6/2023
  215 - 8/6/2023
  216 - 9/6/2023
  217 - 10/6/2023
  218 - 11/6/2023
  219 - 12/6/2023
  220 - 13/6/2023
  221 - 14/6/2023
  222 - 15/6/2023
  223 - 16/6/2023
  224 - 17/6/2023
  225 - 18/6/2023
  226 - 19/6/2023
  227 - 20/6/2023
  228 - 21/6/2023
  229 - 22/6/2023
  230 - 23/6/2023
  231 - 24/6/2023
  232 - 25/6/2023
  233 - 26/6/2023
  234 - 27/6/2023
  235 - 28/6/2023
  236 - 29/6/2023
  237 - 30/6/2023
  238 - 1/7/2023
  239 - 2/7/2023
  240 - 3/7/2023
  241 - 4/7/2023
  242 - 5/7/2023
  243 - 6/7/2023
  244 - 7/7/2023
  245 - 8/7/2023
  246 - 9/7/2023
  247 - 10/7/2023
  248 - 11/7/2023
  249 - 12/7/2023
  250 - 13/7/2023
  251 - 14/7/2023
  252 - 15/7/2023
  253 - 16/7/2023
  254 - 17/7/2023
  255 - 18/7/2023
  256 - 19/7/2023
  257 - 20/7/2023
  258 - 21/7/2023
  259 - 22/7/2023
  260 - 23/7/2023
  261 - 24/7/2023
  262 - 25/7/2023
  263 - 26/7/2023
  264 - 27/7/2023
  265 - 28/7/2023
  266 - 29/7/2023
  267 - 30/7/2023
  268 - 31/7/2023
  269 - 1/8/2023
  270 - 2/8/2023
  271 - 3/8/2023
  272 - 4/8/2023
  273 - 5/8/2023
  274 - 6/8/2023
  275 - 7/8/2023
  276 - 8/8/2023
  277 - 9/8/2023
  278 - 10/8/2023
  279 - 11/8/2023
  280 - 12/8/2023
  281 - 13/8/2023
  282 - 14/8/2023
  283 - 15/8/2023
  284 - 16/8/2023
  285 - 17/8/2023
  286 - 18/8/2023
  287 - 19/8/2023
  288 - 20/8/2023
  289 - 21/8/2023
  290 - 22/8/2023
  291 - 23/8/2023
  292 - 24/8/2023
  293 - 25/8/2023
  294 - 26/8/2023
  295 - 27/8/2023
  296 - 28/8/2023
  297 - 29/8/2023
  298 - 30/8/2023
  299 - 31/8/2023
  300 - 1/9/2023
  301 - 2/9/2023
  302 - 3/9/2023
  303 - 4/9/2023
  304 - 5/9/2023
  305 - 6/9/2023
  306 - 7/9/2023
  307 - 8/9/2023
  308 - 9/9/2023
  309 - 10/9/2023
  310 - 11/9/2023
  311 - 12/9/2023
  312 - 13/9/2023
  313 - 14/9/2023
  314 - 15/9/2023
  315 - 16/9/2023
  316 - 17/9/2023
  317 - 18/9/2023
  318 - 19/9/2023
  319 - 20/9/2023
  320 - 21/9/2023
  321 - 22/9/2023
  322 - 23/9/2023
  323 - 24/9/2023
  324 - 25/9/2023
  325 - 26/9/2023
  326 - 27/9/2023
  327 - 28/9/2023
  328 - 29/9/2023
  329 - 30/9/2023
  330 - 1/10/2023
  331 - 2/10/2023
  332 - 3/10/2023
  333 - 4/10/2023
  334 - 5/10/2023
  335 - 6/10/2023
  336 - 7/10/2023
  337 - 8/10/2023
  338 - 9/10/2023
  339 - 10/10/2023
  340 - 11/10/2023
  341 - 12/10/2023
  342 - 13/10/2023
  343 - 14/10/2023
  344 - 15/10/2023
  345 - 16/10/2023
  346 - 17/10/2023
  347 - 18/10/2023
  348 - 19/10/2023
  349 - 20/10/2023
  350 - 21/10/2023
  351 - 22/10/2023
  352 - 23/10/2023
  353 - 24/10/2023
  354 - 25/10/2023
  355 - 26/10/2023
  356 - 27/10/2023
  357 - 28/10/2023
  358 - 29/10/2023
  359 - 30/10/2023
  360 - 31/10/2023
  361 - 1/11/2023
  362 - 2/11/2023
  363 - 3/11/2023
  364 - 4/11/2023
  365 - 5/11/2023
  366 - 6/11/2023
  367 - 7/11/2023
  368 - 8/11/2023
  369 - 9/11/2023
  370 - 10/11/2023
  371 - 11/11/2023
  372 - 12/11/2023
  373 - 13/11/2023
  374 - 14/11/2023
  375 - 15/11/2023
  376 - 16/11/2023
  377 - 17/11/2023
  378 - 18/11/2023
  379 - 19/11/2023
  380 - 20/11/2023
  381 - 21/11/2023
  382 - 22/11/2023
  383 - 23/11/2023
  384 - 24/11/2023
  385 - 25/11/2023
  386 - 26/11/2023
  387 - 27/11/2023
  388 - 28/11/2023
  389 - 29/11/2023
  390 - 30/11/2023
  391 - 1/12/2023
  392 - 2/12/2023
  393 - 3/12/2023
  394 - 4/12/2023
  395 - 5/12/2023
  396 - 6/12/2023
  397 - 7/12/2023
  398 - 8/12/2023
  399 - 9/12/2023
  400 - 10/12/2023
  401 - 11/12/2023
  402 - 12/12/2023
  403 - 13/12/2023
  404 - 14/12/2023
  405 - 15/12/2023
  406 - 16/12/2023
  407 - 17/12/2023
  408 - 18/12/2023
  409 - 19/12/2023
  410 - 20/12/2023
  411 - 21/12/2023
  412 - 22/12/2023
  413 - 23/12/2023
  414 - 24/12/2023
  415 - 25/12/2023
  416 - 26/12/2023
  417 - 27/12/2023
  418 - 28/12/2023
  419 - 29/12/2023
  420 - 30/12/2023
  421 - 31/12/2023


*/

int meses[] = { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
int eventos[] = {0, 7, 16, 24, 32, 40, 48};
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

//char buffer[140];

void setup() {
  // Begin I2C communication
  Wire.begin();

  // Begin Serial communication
  Serial.begin(9600);
  while (!Serial);

  Serial.println( crearActualizarEvento(2,1,3,15,3,30) );
  Serial.println( fechaHoraEnEvento() );

  //DESCOMENTAR: Para setear la fecha y hora actual
  //RTC.setEpoch(1684723490);

}

void loop() {

  //    Serial.println( eepromPositionDecoder() );

  //    Serial.print("Evento: ");
  //setEvent(7,4,17,30,17,45);

  delay(1000);

  /*

      //Iteramos los dias del año en busqueda de el feriado del dia
      for (int i = 0; i <= sizeof(daysFromYear); i++) {
        //Comparamos el dia de hoy con el indice del array hasta encontrar el dia de hoy
        if( actualDayOfYear == daysFromYear[i]) {
          //Una vez encontrado consultamos si en esa posicion la EEPROM tiene valor 1, de ser así es feriado.
          if( EEPROM.read(i+5) == 1 ) {
            feriado = true;
          }
        }
      }

      if(!feriado) {

      } else {
      //1 seg * 60 * 5
      //No vuelve a preguntar por 5 min.
      delay(1000*60*5);
      }

  */




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

  if( EEPROM.read(daysAccum) == 1 ) {
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

  for (int i = 0; i < 7; i=i+2) {
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
