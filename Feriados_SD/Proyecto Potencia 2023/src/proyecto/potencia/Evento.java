
package proyecto.potencia;


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
