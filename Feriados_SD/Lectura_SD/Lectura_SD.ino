#include <SPI.h>
#include <SD.h>

File myFile;

void setup() {
  Serial.begin(9600);
  leerFeriadosDeSd();

}

void loop() {
  // nothing happens after setup
}

void leerFeriadosDeSd() {
  if (!SD.begin(4)) {
    //TO DO: Si falla la inicializacion de la SD indicar.
    while (1);
  }

  //Abre el archivo de feriados
  myFile = SD.open("feriados.txt");
  if (myFile) {
    while (myFile.available()) {
      String feriado = myFile.readStringUntil(','); // Lee hasta encontrar ;
      EEPROM.write(feriado.toInt(), 1);
    }

    // Cerrar el archivo
    myFile.close();

  } else {
    //TO DO: Error leyendo feriados.txt
  }
}
