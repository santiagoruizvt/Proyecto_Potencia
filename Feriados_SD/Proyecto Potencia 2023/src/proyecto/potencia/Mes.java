package proyecto.potencia;

import java.util.ArrayList;
import java.util.List;

public class Mes {
    
    public Mes(int indice, String name) {
        this.indice = indice;
        this.name = name;
    }
    
    int indice;
    String name;
    List<Integer> feriados = new ArrayList<>();
    
}
