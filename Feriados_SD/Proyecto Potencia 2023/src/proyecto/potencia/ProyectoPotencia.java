/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package proyecto.potencia;

import com.google.gson.Gson;
import java.util.ArrayList;
import java.util.List;
import java.util.OptionalInt;
import java.util.Scanner;
import java.util.stream.Collectors;
import java.util.stream.IntStream;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.HashMap;
import java.util.Map;

/**
 *
 * @author feder
 */
public class ProyectoPotencia {

    private static List<Mes> meses = new ArrayList<>();

    private static Mes enero = new Mes(1, "Enero");
    private static Mes febrero = new Mes(2, "Febrero");
    private static Mes marzo = new Mes(3, "Marzo");
    private static Mes abril = new Mes(4, "Abril");
    private static Mes mayo = new Mes(5, "Mayo");
    private static Mes junio = new Mes(6, "Junio");
    private static Mes julio = new Mes(7, "Julio");
    private static Mes agosto = new Mes(8, "Agosto");
    private static Mes septiembre = new Mes(9, "Septiembre");
    private static Mes octubre = new Mes(10, "Octubre");
    private static Mes noviembre = new Mes(11, "Noviembre");
    private static Mes diciembre = new Mes(12, "Diciembre");
    
    private static HashMap<Integer,Integer> decoMeses = new HashMap<>();

    public static void main(String[] args) {

        String logo = " \n"
                + "                                                                                                    \n"
                + "                                                                                                    \n"
                + "                                          ..:::^^^^^^:::..                                          \n"
                + "                                    .:^~!77777?????????7777!~^:.                                    \n"
                + "                                .^!77??JJY55PPPPPPP55YJJ??77???77!^.                                \n"
                + "                             :~77???JY555PPPPGGBB#&@@@@@&#GPY?777??77~:                             \n"
                + "                          .^!???777777777777777777??J5PB&@@@@&#PY?77???7^.                          \n"
                + "                        .~7??77?JY5PGB######BBGP5J?77777?JP#@@@@@&GJ777??7~.                        \n"
                + "                      .~7????Y5PPGPPPPPPGBB#&@@@@@&#G5J77777JP&@@@@@BY777??7~.                      \n"
                + "                     :7??77????7777777777777?JYPB&@@@@@#5?7777?5#@@@@@BJ777??7:                     \n"
                + "                    ~??777777?JY5PGGGGP55J?777777?5B&@@@@&G?7777?5&@@@@@57777??~                    \n"
                + "                   ~??777?YPB&@@@@@@@@@@@@&#GY?77777JG&@@@@&577777?B@@@@@G7777??~                   \n"
                + "                  ~??77?P#@@@@@@@@@@@@@@@@@@@@&PJ77777JB@@@@@B?77777P@@@@@G77777?~                  \n"
                + "                 ^??7?P&@@@@@@@@@@@@@@@@@@@@@@@@&P?77777P@@@@@B?77777G@@@@@57777??^                 \n"
                + "                .7?7J#@@@@@@@@@@@@@@@@@@@@@@@@@@@@B?77777P@@@@@G77777?&@@@@&?7777?7.                \n"
                + "                ~?7?#@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@B?7777?#@@@@@J77777G@@@@@577777?~                \n"
                + "                7?7G@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@5777775@@@@@P777775@@@@@G77777?7.               \n"
                + "               .77J@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@#77777J@@@@@B77777J@@@@@B77777?7.               \n"
                + "               :?7JGGGP#@@@@@@@@@@@@@@@@@GGGGGG@@@@@&PPPP5P@@@@@#55555G@@@@@&555PJ7?:               \n"
                + "               .7?77777P@@@@@@@@@@@@@@@@&?7777?&@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@Y7?:               \n"
                + "               .7?77777Y@@@@@@@@@@@@@@@@@Y77777P@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@B??7.               \n"
                + "                ~?77777?#@@@@@@@@@@@@@@@@B77777?B@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@&J7?!                \n"
                + "                :7?77777Y@@@@@@@@@@@@@@@@@577777?G@@@@@@@@@@@@@@@@@@@@@@@@@@@@#J7??:                \n"
                + "                 ~?777777P@@@@@@@@@@@@@@@@@Y777777Y#@@@@@@@@@@@@@@@@@@@@@@@@&P?77?~                 \n"
                + "                  !?777777P@@@@@@@@@@@@@@@@@P?777777YB&@@@@@@@@@@@@@@@@@@@#P?777?!.                 \n"
                + "                  .!??777775&@@@@@@@@@@@@@@@@#Y?777777?YG#&@@@@@@@@@@@&BPJ?7777?!.                  \n"
                + "                    ~??77777JB@@@@@@@@@@@@@@@@@#5J77777777?JY55PPP55YJ?777777??!.                   \n"
                + "                     ^7??77777Y#@@@@@@@@@@@@@@@@@&BPY?77777777777777777??77??7^                     \n"
                + "                      .!???77777YB&@@@@@@@@@@@@@@@@@@&#BP5YJJJJJJJYY5GG5?7??!.                      \n"
                + "                        :!???77777?5B&@@@@@@@@@@@@@@@@@@@@@@@@@@@@@&#PJ7??!:                        \n"
                + "                          :~7???77777?YPB&@@@@@@@@@@@@@@@@@@@@@&BGY?7??7~:                          \n"
                + "                            .^!7????77777?JYPGBB##&&&&&&##BGPYJ?77??7!^.                            \n"
                + "                               .:~!7?????7777777?????????7777???7!~:.                               \n"
                + "                                   .:^~!77????????????????77!~^:.                                   \n"
                + "                                         .::^^^~~~~~~^^^::.                                         \n"
                + "                                                                                                    \n"
                + " ------------------------------------------------------------------------------------------------   \n"
                + " UNLAM - Electronica de Potencia - 1° Cuatrimestre 2023                                                     \n"
                + " ------------------------------------------------------------------------------------------------   \n"
                + "                                                                                                    ";

        System.out.println(logo);
        System.out.println("MENU: ");
        System.out.println("");

        meses.add(enero);
        meses.add(febrero);
        meses.add(marzo);
        meses.add(abril);
        meses.add(mayo);
        meses.add(junio);
        meses.add(julio);
        meses.add(agosto);
        meses.add(septiembre);
        meses.add(octubre);
        meses.add(noviembre);
        meses.add(diciembre);
        
        decoMeses.put(1, 31);
        decoMeses.put(2, 29);
        decoMeses.put(3, 31);
        decoMeses.put(4, 30);
        decoMeses.put(5, 31);
        decoMeses.put(6, 30);
        decoMeses.put(7, 31);
        decoMeses.put(8, 31);
        decoMeses.put(9, 30);
        decoMeses.put(10, 31);
        decoMeses.put(11, 30);
        decoMeses.put(12, 31);
        

        //System.out.println(gson.toJson(meses));
        List<String> lista = new ArrayList<>();
        lista.add("Cargar Feriados");
        lista.add("Ver Feriados Cargados");
        lista.add("Eliminar todos los feriados");
        lista.add("Eliminar un Feriado");
        lista.add("Guardar Feriados en SD");
        lista.add("Salir");

        Scanner scanner = new Scanner(System.in);

        while (true) {
            System.out.println("Selecciona un elemento de la lista (ingresa el índice):");

            for (int i = 0; i < lista.size(); i++) {
                System.out.println(i + ": " + lista.get(i));
            }

            int indiceSeleccionado = scanner.nextInt();

            if (indiceSeleccionado >= 0 && indiceSeleccionado < lista.size()) {
                switch (indiceSeleccionado) {
                    case 0:
                        cargarFeriados(scanner);
                        break;

                    case 1:
                        verFeriadosCargados();
                        break;

                    case 2:
                        eliminarTodoLosFeriados();
                        break;

                    case 3:
                        eliminarUnFeriado(scanner);
                        break;
                    case 4:
                        guardarCalendarioEnSD(scanner);
                        break;
                    case 5:
                        System.exit(0);
                }

            } else {
                System.out.println("Índice inválido. No se seleccionó ningún elemento.");
            }

        }
    }

    private static void cargarFeriados(Scanner scanner) {
        while (true) {
            clearScreen();
            System.out.println("Escriba el feriado en formato dd/mm: ");
            String feriado = scanner.next();
            int mes = Integer.parseInt(feriado.split("/")[1]);
            int dia = Integer.parseInt(feriado.split("/")[0]);

            //Busco el mes en el array
            Mes mesBuscado = meses.stream()
                    .filter(m -> m.indice == mes)
                    .collect(Collectors.toList()).get(0);

            //Cargo el feriado al array de dias en el mes
            mesBuscado.feriados.add(dia);

            //Obtengo el indice del mes del array
            OptionalInt indexMeses = IntStream.range(0, meses.size())
                    .filter(i -> mes == meses.get(i).indice)
                    .findFirst();

            meses.set(indexMeses.getAsInt(), mesBuscado);

            System.out.println("Se agregó el feriado: " + dia + " de " + mesBuscado.name);
            System.out.println("¿Desea agregar otro feriado? (s/n)");
            String agregarOtroferiado = scanner.next();
            if (!agregarOtroferiado.equalsIgnoreCase("s")) {
                break;
            }
        }
    }

    private static void verFeriadosCargados() {
        clearScreen();
        List<String> listadoFeriados = new ArrayList<>();
        meses.stream().filter((mes) -> (!mes.feriados.isEmpty())).forEachOrdered((mes) -> {
            mes.feriados.forEach((feriado) -> {
                listadoFeriados.add(feriado + "/" + mes.indice);
            });
        });
        System.out.println("Feriados Cargados:");
        listadoFeriados.forEach((feriado) -> {
            System.out.println(feriado);
        });

        System.out.println("");

    }

    private static void eliminarTodoLosFeriados() {
        System.out.print("\033[H\033[2J");
        System.out.flush();
        for (int i = 0; i < meses.size(); i++) {
            meses.get(i).feriados.clear();
        }
        System.out.println("Se eliminaron todos los feriados.");

        System.out.println("");
        System.out.println("");
    }

    private static void eliminarUnFeriado(Scanner scanner) {
        clearScreen();
        System.out.println("Escriba el feriado a eliminar en formato dd/mm: ");
        String feriado = scanner.next();
        int mes = Integer.parseInt(feriado.split("/")[1]);
        int dia = Integer.parseInt(feriado.split("/")[0]);

        //Obtengo el indice del mes del array
        OptionalInt indexMeses = IntStream.range(0, meses.size())
                .filter(i -> mes == meses.get(i).indice)
                .findFirst();

        //Obtengo el indice del mes del array
        OptionalInt indexDia = IntStream.range(0, meses.get(indexMeses.getAsInt()).feriados.size())
                .filter(i -> dia == meses.get(indexMeses.getAsInt()).feriados.get(i))
                .findFirst();

        meses.get(indexMeses.getAsInt()).feriados.remove(indexDia.getAsInt());

        System.out.println("Se eliminó el feriado " + dia + "/" + mes);
        System.out.println("");
        System.out.println("");

    }

    private static void guardarCalendarioEnSD(Scanner scanner) {
        clearScreen();

        List<String> lista = new ArrayList<>();

        for (File sysDrive : File.listRoots()) {
            lista.add(sysDrive.toString());
        }

        System.out.println("Seleccione la unidad donde se encuentra la SD (ingresa el índice):");

        for (int i = 0; i < lista.size(); i++) {
            System.out.println(i + ": " + lista.get(i));
        }

        int indiceSeleccionado = scanner.nextInt();
        String elementoSeleccionado = lista.get(indiceSeleccionado);

        /*
        GUARDA EN SD
         */
        Gson gson = new Gson();

        String nombreArchivo = "feriados.txt";

        // Tarjeta SD accesible
        File directorioSD = new File(elementoSeleccionado);
        if (!directorioSD.exists() || !directorioSD.isDirectory()) {
            System.out.println("La tarjeta SD no está disponible.");
            return;
        }

        File archivo = new File(directorioSD, nombreArchivo);

        try {
            String fileToSd = "";
            for (Mes mes : meses) {
                if(!mes.feriados.isEmpty()) {
                    for (Integer feriado : mes.feriados) {
                        Integer posicionEnMemoria = 55 + feriado;
                        for (int i = 1; i < mes.indice; i++) {
                            posicionEnMemoria = posicionEnMemoria + decoMeses.get(i);
                        }
                        fileToSd += posicionEnMemoria + ",";
                    }
                }
            }
            
            // Escribir el contenido en el archivo
            FileWriter escritor = new FileWriter(archivo);
            escritor.write(fileToSd);
            escritor.close();
            

            System.out.println("Los feriados se guardaron correctamente en la tarjeta SD.");
        } catch (IOException e) {
            e.printStackTrace();
            System.out.println("Error al guardar el archivo en la tarjeta SD.");
        }

    }

    private static void clearScreen() {
        System.out.print("\033[H\033[2J");
        System.out.flush();
    }

}
