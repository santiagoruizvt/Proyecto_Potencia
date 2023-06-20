/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package proyecto.potencia;

/**
 *
 * @author feder
 */
public class Evento {

    String horaInicio;
    String horaFin;
    int valorCalculadoInicio;
    int valorCalculadoFinal;

    public Evento(String horaInicio, String horaFin) {
        this.horaInicio = horaInicio;
        this.horaFin = horaFin;

        this.valorCalculadoInicio = (Integer.parseInt(horaInicio.split(":")[0]) * 4) + (Integer.parseInt(horaInicio.split(":")[1]) / 15);
        this.valorCalculadoFinal = (Integer.parseInt(horaFin.split(":")[0]) * 4) + (Integer.parseInt(horaFin.split(":")[1]) / 15);
    }

}
