package proyecto.potencia;

import java.util.ArrayList;
import java.util.List;

public class Dia {
    
        public Dia(int indice, String name) {
        this.indice = indice;
        this.name = name;
    }
    
    int indice;
    String name;
    List<Evento> eventos = new ArrayList<>();
    
}
