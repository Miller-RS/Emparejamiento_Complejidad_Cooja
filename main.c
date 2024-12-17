#include "contiki.h"
#include "net/netstack.h"
#include "random.h"
#include "sys/etimer.h"
#include "dev/watchdog.h"
#include <stdio.h>
#include <stdint.h>

/*
 * Este código simula distintas complejidades computacionales (O(n²), O(n² log n) y O(n³))
 * empleando bucles anidados. Estos bucles sirven como un marcador de posición para
 * aproximar el costo temporal que tendrían operaciones criptográficas avanzadas en una
 * arquitectura IoT de 16 bits (MSP430), tales como:
 *
 * - Generar primos mayores a 32 bits (implicando múltiples multiplicaciones modulares
 *   y test de primalidad).
 * - Implementar emparejamientos bilineales, que requieren operaciones sobre campos
 *   finitos de gran tamaño (multiplicaciones, exponentiaciones, etc.).
 *
 * El objetivo es obtener métricas aproximadas del tiempo de ejecución, demostrando
 * la inviabilidad de estas operaciones sin optimizaciones adicionales.
 */

PROCESS(complexity_simulation_process, "Complexity Simulation Process");
AUTOSTART_PROCESSES(&complexity_simulation_process);

static void simulate_O_n2(int n) {
    // Simulación de complejidad O(n²)
    // Dos bucles anidados: i y j
    for (int i = 0; i < n; i++) {
        watchdog_periodic(); // Evita el reset del watchdog
        for (int j = 0; j < n; j++) {
            watchdog_periodic(); 
            // Aquí se podrían incluir operaciones constantes que simulen trabajo.
        }
    }
}

static void simulate_O_n2_log_n(int n) {
    // Simulación de complejidad O(n² log n)
    // Dos bucles anidados O(n²) y dentro uno O(log n)
    for (int i = 0; i < n; i++) {
        watchdog_periodic();
        for (int j = 0; j < n; j++) {
            watchdog_periodic();
            int m = n;
            // Bucle O(log n)
            while (m > 1) {
                m /= 2;
                watchdog_periodic();
            }
        }
    }
}

static void simulate_O_n3(int n) {
    // Simulación de complejidad O(n³)
    // Tres bucles anidados
    for (int i = 0; i < n; i++) {
        watchdog_periodic();
        for (int j = 0; j < n; j++) {
            watchdog_periodic();
            for (int k = 0; k < n; k++) {
                watchdog_periodic();
                // Aquí se podrían incluir operaciones constantes que simulen trabajo.
            }
        }
    }
}

PROCESS_THREAD(complexity_simulation_process, ev, data) {
    static struct etimer timer;

    PROCESS_BEGIN();
    etimer_set(&timer, CLOCK_SECOND * 2);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    // Probamos las diferentes complejidades con tamaños crecientes, representando
    // el tamaño del "primo" o parámetros criptográficos.
    // Ajustar los rangos según las necesidades.
    for (int primo_size = 16; primo_size <= 64; primo_size += 16) {
        // O(n²)
        for (int run = 0; run < 5; run++) {
            simulate_O_n2(primo_size);
            printf("Complejidad O(n^2) finalizo corrida %d para tamaño %d\n", run, primo_size);
        }

        // O(n² log n)
        for (int run = 0; run < 5; run++) {
            simulate_O_n2_log_n(primo_size);
            printf("Complejidad O(n^2 log n) finalizo corrida %d para tamaño %d\n", run, primo_size);
        }

        // O(n³)
        for (int run = 0; run < 5; run++) {
            simulate_O_n3(primo_size);
            printf("Complejidad O(n^3) finalizo corrida %d para tamaño %d\n", run, primo_size);
        }
    }

    printf("Finalizo todas las simulaciones\n");
    watchdog_periodic();

    PROCESS_END();
}
