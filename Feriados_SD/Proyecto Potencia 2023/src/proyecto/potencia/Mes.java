/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package proyecto.potencia;

import java.util.ArrayList;
import java.util.List;

/**
 *
 * @author feder
 */
public class Mes {
    
    public Mes(int indice, String name) {
        this.indice = indice;
        this.name = name;
    }
    
    int indice;
    String name;
    List<Integer> feriados = new ArrayList<>();
    
}
