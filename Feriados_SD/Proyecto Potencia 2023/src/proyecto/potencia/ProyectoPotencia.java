/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
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
import java.util.HashMap;

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

    private static List<Dia> dias = new ArrayList<>();

    private static Dia lunes = new Dia(1, "Lunes");
    private static Dia martes = new Dia(2, "Martes");
    private static Dia miercoles = new Dia(3, "Miercoles");
    private static Dia jueves = new Dia(4, "Jueves");
    private static Dia viernes = new Dia(5, "Viernes");
    private static Dia sabado = new Dia(6, "Sábado");
    private static Dia domingo = new Dia(0, "Domingo");

    private static HashMap<Integer, Integer> decoMeses = new HashMap<>();
    private static HashMap<Integer, Integer> decoDias = new HashMap<>();
    private static HashMap<Integer, String> decoEventosHora = new HashMap<>();

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

        decoEventosHora.put(0, "00:00");
        decoEventosHora.put(1, "00:15");
        decoEventosHora.put(2, "00:30");
        decoEventosHora.put(3, "00:45");
        decoEventosHora.put(4, "01:00");
        decoEventosHora.put(5, "01:15");
        decoEventosHora.put(6, "01:30");
        decoEventosHora.put(7, "01:45");
        decoEventosHora.put(8, "02:00");
        decoEventosHora.put(9, "02:15");
        decoEventosHora.put(10, "02:30");
        decoEventosHora.put(11, "02:45");
        decoEventosHora.put(12, "03:00");
        decoEventosHora.put(13, "03:15");
        decoEventosHora.put(14, "03:30");
        decoEventosHora.put(15, "03:45");
        decoEventosHora.put(16, "04:00");
        decoEventosHora.put(17, "04:15");
        decoEventosHora.put(18, "04:30");
        decoEventosHora.put(19, "04:45");
        decoEventosHora.put(20, "05:00");
        decoEventosHora.put(21, "05:15");
        decoEventosHora.put(22, "05:30");
        decoEventosHora.put(23, "05:45");
        decoEventosHora.put(24, "06:00");
        decoEventosHora.put(25, "06:15");
        decoEventosHora.put(26, "06:30");
        decoEventosHora.put(27, "06:45");
        decoEventosHora.put(28, "07:00");
        decoEventosHora.put(29, "07:15");
        decoEventosHora.put(30, "07:30");
        decoEventosHora.put(31, "07:45");
        decoEventosHora.put(32, "08:00");
        decoEventosHora.put(33, "08:15");
        decoEventosHora.put(34, "08:30");
        decoEventosHora.put(35, "08:45");
        decoEventosHora.put(36, "09:00");
        decoEventosHora.put(37, "09:15");
        decoEventosHora.put(38, "09:30");
        decoEventosHora.put(39, "09:45");
        decoEventosHora.put(40, "10:00");
        decoEventosHora.put(41, "10:15");
        decoEventosHora.put(42, "10:30");
        decoEventosHora.put(43, "10:45");
        decoEventosHora.put(44, "11:00");
        decoEventosHora.put(45, "11:15");
        decoEventosHora.put(46, "11:30");
        decoEventosHora.put(47, "11:45");
        decoEventosHora.put(48, "12:00");
        decoEventosHora.put(49, "12:15");
        decoEventosHora.put(50, "12:30");
        decoEventosHora.put(51, "12:45");
        decoEventosHora.put(52, "13:00");
        decoEventosHora.put(53, "13:15");
        decoEventosHora.put(54, "13:30");
        decoEventosHora.put(55, "13:45");
        decoEventosHora.put(56, "14:00");
        decoEventosHora.put(57, "14:15");
        decoEventosHora.put(58, "14:30");
        decoEventosHora.put(59, "14:45");
        decoEventosHora.put(60, "15:00");
        decoEventosHora.put(61, "15:15");
        decoEventosHora.put(62, "15:30");
        decoEventosHora.put(63, "15:45");
        decoEventosHora.put(64, "16:00");
        decoEventosHora.put(65, "16:15");
        decoEventosHora.put(66, "16:30");
        decoEventosHora.put(67, "16:45");
        decoEventosHora.put(68, "17:00");
        decoEventosHora.put(69, "17:15");
        decoEventosHora.put(70, "17:30");
        decoEventosHora.put(71, "17:45");
        decoEventosHora.put(72, "18:00");
        decoEventosHora.put(73, "18:15");
        decoEventosHora.put(74, "18:30");
        decoEventosHora.put(75, "18:45");
        decoEventosHora.put(76, "19:00");
        decoEventosHora.put(77, "19:15");
        decoEventosHora.put(78, "19:30");
        decoEventosHora.put(79, "19:45");
        decoEventosHora.put(80, "20:00");
        decoEventosHora.put(81, "20:15");
        decoEventosHora.put(82, "20:30");
        decoEventosHora.put(83, "20:45");
        decoEventosHora.put(84, "21:00");
        decoEventosHora.put(85, "21:15");
        decoEventosHora.put(86, "21:30");
        decoEventosHora.put(87, "21:45");
        decoEventosHora.put(88, "22:00");
        decoEventosHora.put(89, "22:15");
        decoEventosHora.put(90, "22:30");
        decoEventosHora.put(91, "22:45");
        decoEventosHora.put(92, "23:00");
        decoEventosHora.put(93, "23:15");
        decoEventosHora.put(94, "23:30");
        decoEventosHora.put(95, "23:45");

        //System.out.println(gson.toJson(meses));
        List<String> lista = new ArrayList<>();
        lista.add("Cargar Feriados");
        lista.add("Ver Feriados Cargados");
        lista.add("Eliminar todos los feriados");
        lista.add("Eliminar un Feriado");
        lista.add("Cargar Eventos");
        lista.add("Ver Eventos Cargados");
        lista.add("Eliminar todos los eventos");
        lista.add("Eliminar un evento");
        lista.add("Guardar Feriados en SD y Dias");
        lista.add("Leer SD");
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

    private static void eliminarTodoLosEventos() {
        System.out.print("\033[H\033[2J");
        System.out.flush();
        for (int i = 0; i < dias.size(); i++) {
            dias.get(i).eventos.clear();
        }
        System.out.println("Se eliminaron todos los eventos.");

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

    private static void eliminarUnEvento(Scanner scanner) {
        clearScreen();
        System.out.println("Seleccione el dia del evento que desea eliminar: ");

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

        System.out.println("Se eliminó el Evento del dia " + diabuscado.name);
        System.out.println("");
        System.out.println("");

    }

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
                System.out.println("Los Eventos de este día estan completos, por favor seleccione un evento a sobrescribir:");

                System.out.println("0: Evento 1");
                System.out.println("1: Evento 2");
                System.out.println("2: Evento 3");
                System.out.println("3: Evento 4");

                String eventoSeleccionado = scanner.next();
                eventoInt = Integer.parseInt(eventoSeleccionado);

            }

            System.out.println("Seleccione el horario inicial del evento en formato hh:mm");
            System.out.println("(Los minutos deben ser múltiplo de 15)");

            String horarioInicial = scanner.next();

            System.out.println("Seleccione el horario final del evento en formato hh:mm");
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

            System.out.println("Se agregó el evento al día " + diabuscado.name + " [Hora Inicial: " + horarioInicial + " | Hora Final: " + horarioFinal + "]");
            System.out.println("Hora Inicial Calculada: " + evento.valorCalculadoInicio);
            System.out.println("Hora Final Calculada: " + evento.valorCalculadoFinal);
            System.out.println("¿Desea agregar otro evento? (s/n)");
            String agregarOtroferiado = scanner.next();
            if (!agregarOtroferiado.equalsIgnoreCase("s")) {
                break;
            }
        }
    }

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
            String[] eventosLeidosArray = eventosLeidos.split(",");

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
            for (int i = 0; i < eventosLeidosArray.length; i++) {
                int diaEvento = 0;
                int eventoPosicion = Integer.parseInt(eventosLeidosArray[i].split(":")[0]);
                for (int j = 0; j < decoDias.size(); j++) {
                    if (decoDias.get(j) > eventoPosicion) {
                        diaEvento = j;
                        break;
                    }

                }
                int fechaHora = Integer.parseInt(eventosLeidosArray[i].split(":")[1]);
                decoEventosHora.get(fechaHora);
                
                
                
                
            }

        } catch (IOException e) {
            e.printStackTrace();
            System.out.println("Error al guardar el archivo en la tarjeta SD.");
        }

    }

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
                        fileToSd += formatNumber(posicionInicial) + ":" + dia.eventos.get(i).valorCalculadoInicio + ",";
                        posicionInicial = posicionInicial + 1;
                        fileToSd += formatNumber(posicionInicial) + ":" + dia.eventos.get(i).valorCalculadoFinal + ",";
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

    private static void clearScreen() {
        System.out.print("\033[H\033[2J");
        System.out.flush();
    }

    private static String formatNumber(int valor) {
        if (valor < 10) {
            return "0" + valor;
        } else {
            return "" + valor;
        }
    }

}