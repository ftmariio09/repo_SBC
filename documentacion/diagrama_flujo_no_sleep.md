1. Diagrama de bloques

```mermaid

flowchart LR

    init([Inicio])
    main(app_main)
    iftime{"¿reloj en hora?"}
    time("iniciar_wifi
    obtener_tiempo
    parar_wifi")




    datos("get_luz
    get_HR
    get_temp
    get_CO2
    guardar timestamp")

    enviardatos("iniciar_wifi
    enviar_datos
    ¿ ota ?
    parar_wifi")

    iflecturas{"num_lecturas
     == 
     NUM_OF_DATA"}

    
    ruido(get_ruido)


    init --> main

    main --> iftime
    iftime -->|si| ruido
    ruido --> datos
    iftime -->|no| time
    time --> ruido

    datos --> iflecturas
    iflecturas -->|true| enviardatos
    iflecturas -->|false| main
    enviardatos --> main

```