#include <SPI.h>
#include <SD.h>
#include <EEPROM.h>

File myFile;

void setup() {
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
    String feriados;
    String eventos;

    while (myFile.available()) {

      //Feriados
      feriados = myFile.readStringUntil('|');
      int feriados_len = feriados.length() + 1;
      char char_feriados[feriados_len];
      feriados.toCharArray(char_feriados, feriados_len);

      String feriado = "";
      for (int i = 0; i < feriados.length(); i++) {
        if (feriados[i] == ',') {
          EEPROM.write(feriado.toInt(), 1);
          feriado = "";
        } else {
          feriado += feriados[i];
        }

      }

      //Eventos
      eventos = myFile.readStringUntil('.');
      int eventos_len = eventos.length() + 1;
      char char_eventos[eventos_len];
      eventos.toCharArray(char_eventos, eventos_len);

      char evento[6];
      int current = 0;
      for (int i = 0; i < eventos.length(); i++) {
        if (eventos[i] == ',') {
          char posicionEnMemoriaEvento[3];
          char valorAGuardarEnPosicion[3];

          posicionEnMemoriaEvento[0] = evento[0];
          posicionEnMemoriaEvento[1] = evento[1];
          posicionEnMemoriaEvento[2] = '\0';
          valorAGuardarEnPosicion[0] = evento[3];
          valorAGuardarEnPosicion[1] = evento[4];
          valorAGuardarEnPosicion[2] = '\0';

          EEPROM.write(atoi(posicionEnMemoriaEvento), atoi(valorAGuardarEnPosicion));
          current = 0;
        } else {
          evento[current] = eventos[i];
          current++;
        }

      }
    }


    // Cerrar el archivo
    myFile.close();

  } else {
    //TO DO: Error leyendo feriados.txt
  }
}