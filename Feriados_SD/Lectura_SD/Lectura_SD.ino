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

      String evento = "";
      for (int i = 0; i < eventos.length(); i++) {
        if (eventos[i] == ',') {
          String posicionEnMemoriaEvento = evento.substring(0,2);
          String valorAGuardarEnPosicion = evento.substring(3);
          EEPROM.write(posicionEnMemoriaEvento.toInt(), valorAGuardarEnPosicion.toInt());
          evento = "";
        } else {
          evento += eventos[i];
        }

      }
    }


    // Cerrar el archivo
    myFile.close();

  } else {
    //TO DO: Error leyendo feriados.txt
  }
}
