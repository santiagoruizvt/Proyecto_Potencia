package proyecto.potencia;

import java.util.ArrayList;
import java.util.List;
import java.util.OptionalInt;
import java.util.Scanner;
import java.util.stream.Collectors;
import java.util.stream.IntStream;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Collections;
import java.util.HashMap;

/**
 *
 * @author ELECTRONICA DE POTENCIA - SOFTWARE PARA CARGA DE FERIADOS Y RANGOS EN
 * SD
 *
 * Clase ProyectoPotencia con metodo Main
 *
 */
public class ProyectoPotencia {

    //Listado de Meses
    private static List<Mes> meses = new ArrayList<>();

    //Se inicializan los meses con un id
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

    //Listado de dias de la semana
    private static List<Dia> dias = new ArrayList<>();

    //Se inicializan los dias de la semana con un id
    private static Dia lunes = new Dia(1, "Lunes");
    private static Dia martes = new Dia(2, "Martes");
    private static Dia miercoles = new Dia(3, "Miercoles");
    private static Dia jueves = new Dia(4, "Jueves");
    private static Dia viernes = new Dia(5, "Viernes");
    private static Dia sabado = new Dia(6, "Sábado");
    private static Dia domingo = new Dia(0, "Domingo");

    private static HashMap<Integer, Integer> decoMeses = new HashMap<>();
    private static HashMap<Integer, Integer> decoDias = new HashMap<>();

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

        //INICIALIZACIONES 
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

        dias.add(lunes);
        dias.add(martes);
        dias.add(miercoles);
        dias.add(jueves);
        dias.add(viernes);
        dias.add(sabado);
        dias.add(domingo);

        decoDias.put(0, 0);
        decoDias.put(1, 8);
        decoDias.put(2, 16);
        decoDias.put(3, 24);
        decoDias.put(4, 32);
        decoDias.put(5, 40);
        decoDias.put(6, 48);

        //Se cargan en una lista los items del menu
        List<String> lista = new ArrayList<>();
        lista.add("Cargar Feriados");
        lista.add("Ver Feriados Cargados");
        lista.add("Eliminar todos los feriados");
        lista.add("Eliminar un Feriado");
        lista.add("Cargar Rangos");
        lista.add("Ver Rangos Cargados");
        lista.add("Eliminar todos los rangos");
        lista.add("Eliminar un Rango");
        lista.add("Guardar Feriados en SD y Dias");
        lista.add("Leer SD");
        lista.add("Salir");

        Scanner scanner = new Scanner(System.in);

        //Se despliega el menu en consola
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
                        cargarEventos(scanner);
                        break;
                    case 5:
                        verEentosCargados();
                        break;
                    case 6:
                        eliminarTodoLosEventos();
                        break;
                    case 7:
                        eliminarUnEvento(scanner);
                        break;
                    case 8:
                        guardarCalendarioEnSD(scanner);
                        break;
                    case 9:
                        leersd(scanner);
                        break;
                    case 10:
                        System.exit(0);
                }

            } else {
                System.out.println("Índice inválido. No se seleccionó ningún elemento.");
            }

        }
    }

    //Este Metodo permite cargar feriados en el formato especificado
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

    //Permite visualizar el listado de feriados
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

    //Permite vizualisar los rangos cargados en formato de lista
    private static void verEentosCargados() {
        clearScreen();
        dias.stream().filter((dia) -> (!dia.eventos.isEmpty())).forEachOrdered((dia) -> {
            System.out.println("[" + dia.name + "]");
            System.out.println("|");
            dia.eventos.forEach((evento) -> {
                System.out.println("|-> Inicio:  " + evento.horaInicio + " Fin: " + evento.horaFin);
            });
            System.out.println("");
        });

        System.out.println("");

    }

    //Elimina todos los feriados de la variable meses
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

    //Elimina todos los Rangos de la variable dias
    private static void eliminarTodoLosEventos() {
        System.out.print("\033[H\033[2J");
        System.out.flush();
        for (int i = 0; i < dias.size(); i++) {
            dias.get(i).eventos.clear();
        }
        System.out.println("Se eliminaron todos los rangos.");

        System.out.println("");
        System.out.println("");
    }

    //Permite eliminar un feriado en especifico de la variable meses
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

    //Metodo eliminar un rango en especifico de la variable dias
    private static void eliminarUnEvento(Scanner scanner) {
        clearScreen();
        System.out.println("Seleccione el dia del rango que desea eliminar: ");

        System.out.println("0: Lunes");
        System.out.println("1: Martes");
        System.out.println("2: Miercoles");
        System.out.println("3: Jueves");
        System.out.println("4: Viernes");
        System.out.println("5: Sabado");
        System.out.println("6: Domingo");

        String diaDelEvento = scanner.next();
        int diaDelEventoInt = Integer.parseInt(diaDelEvento);

        //Busco el dia en el array
        Dia diabuscado = dias.stream()
                .filter(d -> d.indice == diaDelEventoInt)
                .collect(Collectors.toList()).get(0);

        //Obtengo el indice del mes del array
        OptionalInt indexDia = IntStream.range(0, dias.size())
                .filter(i -> diaDelEventoInt == dias.get(i).indice)
                .findFirst();

        dias.get(indexDia.getAsInt()).eventos.remove(indexDia.getAsInt());

        System.out.println("Se eliminó el Rango del dia " + diabuscado.name);
        System.out.println("");
        System.out.println("");

    }

    //Metodo que permite cargar rangos y los almacena en la variable dias
    private static void cargarEventos(Scanner scanner) {
        while (true) {
            clearScreen();
            System.out.println("Seleccione un día de la semana: ");

            System.out.println("0: Domingo");
            System.out.println("1: Lunes");
            System.out.println("2: Martes");
            System.out.println("3: Miercoles");
            System.out.println("4: Jueves");
            System.out.println("5: Viernes");
            System.out.println("6: Sabado");

            String diaDelEvento = scanner.next();
            int diaDelEventoInt = Integer.parseInt(diaDelEvento);

            //Busco el dia en el array
            Dia diabuscado = dias.stream()
                    .filter(d -> d.indice == diaDelEventoInt)
                    .collect(Collectors.toList()).get(0);

            int eventoInt = -1;
            if (diabuscado.eventos.size() == 4) {
                System.out.println("Los Eventos de este día estan completos, por favor seleccione un rango a sobrescribir:");

                System.out.println("0: Rango 1");
                System.out.println("1: Rango 2");
                System.out.println("2: Rango 3");
                System.out.println("3: Rango 4");

                String eventoSeleccionado = scanner.next();
                eventoInt = Integer.parseInt(eventoSeleccionado);

            }

            System.out.println("Seleccione el horario inicial del rango en formato hh:mm");
            System.out.println("(Los minutos deben ser múltiplo de 15)");

            String horarioInicial = scanner.next();

            System.out.println("Seleccione el horario final del rango en formato hh:mm");
            System.out.println("(Los minutos deben ser múltiplo de 15)");

            String horarioFinal = scanner.next();

            Evento evento = new Evento(horarioInicial, horarioFinal);

            if (eventoInt < 0) {
                diabuscado.eventos.add(evento);
            } else {
                diabuscado.eventos.get(eventoInt).horaInicio = evento.horaInicio;
                diabuscado.eventos.get(eventoInt).horaFin = evento.horaFin;
                diabuscado.eventos.get(eventoInt).valorCalculadoInicio = evento.valorCalculadoInicio;
                diabuscado.eventos.get(eventoInt).valorCalculadoFinal = evento.valorCalculadoFinal;
            }

            //Obtengo el indice del mes del array
            OptionalInt indexDias = IntStream.range(0, dias.size())
                    .filter(i -> diaDelEventoInt == dias.get(i).indice)
                    .findFirst();

            dias.set(indexDias.getAsInt(), diabuscado);

            System.out.println("Se agregó el rango al día " + diabuscado.name + " [Hora Inicial: " + horarioInicial + " | Hora Final: " + horarioFinal + "]");

            System.out.println("¿Desea agregar otro rango? (s/n)");
            String agregarOtroferiado = scanner.next();
            if (!agregarOtroferiado.equalsIgnoreCase("s")) {
                break;
            }
        }
    }

    //Hace una lectura de los feriados y los rangos cargados en la SD y los mergea con los preexistentes 
    //durante la ejecución del programa
    private static void leersd(Scanner scanner) {
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
        String nombreArchivo = "feriados.txt";

        // Tarjeta SD accesible
        File directorioSD = new File(elementoSeleccionado);
        if (!directorioSD.exists() || !directorioSD.isDirectory()) {
            System.out.println("La tarjeta SD no está disponible.");
            return;
        }

        int mes;
        int dia;

        try {

            File archivo = new File(directorioSD, nombreArchivo);
            Scanner myReader = new Scanner(archivo);
            String data = "";
            while (myReader.hasNextLine()) {
                data = myReader.nextLine();
            }

            myReader.close();

            //Carga de feriados
            String feriadosLeidos = data.split("\\|")[0];
            String eventosLeidos = data.split("\\|")[1];

            String[] feriadosLeidosArray = feriadosLeidos.split(",");
            String[] eventosLeidosArray = eventosLeidos.split("[.]")[0].substring(0, eventosLeidos.length() - 1).split(",");

            //Obtengo Feriados Leidos Array
            for (int i = 0; i < feriadosLeidos.split(",").length; i++) {
                int accum = 55;
                mes = 0;
                dia = 0;

                for (int j = 1; j <= decoMeses.size(); j++) {
                    accum = accum + decoMeses.get(j);
                    if (Integer.parseInt(feriadosLeidosArray[i]) < accum) {
                        mes = j;
                        dia = (accum - decoMeses.get(j) - Integer.parseInt(feriadosLeidosArray[i])) * -1;
                        break;
                    }
                }

                cargarFeriadosForSdMethod(mes, dia);

            }

            //Carga de Eventos
            ArrayList<ArrayList<Integer>> eventosHorarios = new ArrayList<>();

            for (String posicionHorario : eventosLeidosArray) {
                ArrayList<Integer> e = new ArrayList();
                e.add(Integer.parseInt(posicionHorario.split(":")[0]));
                e.add(Integer.parseInt(posicionHorario.split(":")[1]));
                eventosHorarios.add(e);
            }

            Collections.sort(eventosHorarios, (ArrayList<Integer> o1, ArrayList<Integer> o2) -> o1.get(0).compareTo(o2.get(0)));

            //Eventos ordenados
            for (int i = 0; i < eventosHorarios.size(); i = i + 2) {
                ArrayList<Integer> ev = new ArrayList<>();
                ArrayList<Integer> evSiguiente = new ArrayList<>();
                ev = eventosHorarios.get(i);
                evSiguiente = eventosHorarios.get(i + 1);

                //determina dia
                int diaEvento = 0;
                for (int j = 0; j < decoDias.size(); j++) {
                    if (decoDias.get(j) > ev.get(0)) {
                        diaEvento = j;
                        break;
                    }
                }

                for (int z = 0; z < decoDias.size(); z++) {
                    if (decoDias.get(z) > ev.get(0)) {
                        diaEvento = z - 1;
                        break;
                    }
                }

                Dia diabuscado = diaBuscado(diaEvento);
                int eventoAOcupar = diabuscado.eventos.size();

                if (eventoAOcupar <= 4) {
                    //Determina Horario Inicial y Final

                    String horarioInicial = formatNumber(ev.get(1) / 4) + ":" + formatNumber((ev.get(1) % 4) * 15);
                    String horarioFinal = formatNumber(evSiguiente.get(1) / 4) + ":" + formatNumber((evSiguiente.get(1) % 4) * 15);

                    Evento evento = new Evento(horarioInicial, horarioFinal);

                    diabuscado.eventos.add(evento);

                    //Obtengo el indice del mes del array
                    OptionalInt indexDias = indexDias(diaEvento);

                    dias.set(indexDias.getAsInt(), diabuscado);

                    System.out.println("Feriados y Rangos Leidos correctamente de SD");

                } else {
                    System.out.println("Todos los rangos estan ocupados para el dia " + diabuscado.name);
                }

            }

        } catch (IOException e) {
            e.printStackTrace();
            System.out.println("Error al leer el archivo en la tarjeta SD.");
        }

        System.out.println("Feriados y Rangos Leidos correctamente de SD");

    }

    /*
    
        Este Metodo guarda los feriados y los rangos en la SD:
    
        Codifica los feriados a la posición de memoria correspondiente a la EEPROM del
        Atmega328, luego el firmware del dispositivo se encarga de setear un 1 en
        dichas posiciones de memoria indicando que corresponde un feriado para esas fechas.
    
        Para los Rangos se utiliza el formato XX:YY, siendo XX la posición de memoria
        de dentro de la EEPROM e YY el horario a alamacenenar en dicha posicion de memoria.
    
        Los feriados y los rangos estan separados por un PIPE (|) para facilitar el parseo
        y se detecta el fin de trama con un punto.
    
    */

   
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
                if (!mes.feriados.isEmpty()) {
                    for (Integer feriado : mes.feriados) {
                        Integer posicionEnMemoria = 55 + feriado;
                        for (int i = 1; i < mes.indice; i++) {
                            posicionEnMemoria = posicionEnMemoria + decoMeses.get(i);
                        }
                        fileToSd += posicionEnMemoria + ",";
                    }
                }
            }

            fileToSd += "|";
            //Dias
            for (Dia dia : dias) {
                if (!dia.eventos.isEmpty()) {
                    int posicionInicial = decoDias.get(dia.indice);
                    for (int i = 0; i < dia.eventos.size(); i++) {
                        fileToSd += formatNumber(posicionInicial) + ":" + formatNumber(dia.eventos.get(i).valorCalculadoInicio) + ",";
                        posicionInicial = posicionInicial + 1;
                        fileToSd += formatNumber(posicionInicial) + ":" + formatNumber(dia.eventos.get(i).valorCalculadoFinal) + ",";
                        posicionInicial = posicionInicial + 1;
                    }
                }
            }

            fileToSd += ".";

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

    //Metodo para obtener el Objeto Dia por un indice
    private static Dia diaBuscado(int diaEvento) {
        return dias.stream()
                .filter(d -> d.indice == diaEvento)
                .collect(Collectors.toList()).get(0);
    }

    //Metodo para obtener indice en la variable dias
    private static OptionalInt indexDias(int diaEvento) {
        return IntStream.range(0, dias.size())
                .filter(d -> diaEvento == dias.get(d).indice)
                .findFirst();
    }

    //Metodo para cargar feriados dentro de la variable meses
    private static void cargarFeriadosForSdMethod(int mes, int dia) {
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
    }

    //Metodo para borrar pantalla
    private static void clearScreen() {
        System.out.print("\033[H\033[2J");
        System.out.flush();
    }

    //Formatea numero en string para siempre manejar 2 decimales
    private static String formatNumber(int valor) {
        if (valor < 10) {
            return "0" + valor;
        } else {
            return "" + valor;
        }
    }

}
