# Complejidad de Generar Primos Grandes y Calcular Emparejamientos Bilineales en MSP430 (16 bits)


## Contexto

- **Arquitectura MSP430 (16 bits):**  
  Los dispositivos con esta arquitectura poseen registros y ancho de datos limitados. Cuando se requieren números mayores a 32 bits, es necesario implementar aritmética multipalabra, dividiendo cada número grande en varias “palabras” de 16 bits.

- **Generación de Primos Grandes:**  
  Involucra tests de primalidad (e.g., Miller-Rabin), que requieren exponentiaciones modulares sobre números grandes. Esto se traduce en una enorme cantidad de operaciones de multiplicación modular y reducción, cada una de las cuales es O(n²) respecto al número de palabras.

- **Emparejamientos Bilineales:**  
  Estas operaciones, fundamentales en criptografía avanzada (e.g. esquemas basados en atributos), requieren cálculos sobre campos finitos grandes, incluyendo multiplicaciones y exponentiaciones modulares múltiples. El costo puede ser aún mayor que el de la simple generación de primos, acercándose a O(n² log n) o más, debido al mayor número de operaciones encadenadas.

## Ejemplo de Código: Multiplicación Multipalabra para Primos Grandes

El siguiente fragmento ilustra la multiplicación de números de 64 bits (4 palabras de 16 bits) en MSP430. Para 128 bits o más, la complejidad se incrementa drásticamente.

```c
#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint16_t words[4]; // 64 bits divididos en 4 palabras de 16 bits
} uint64_multi_t;

static void multiword_mul(const uint64_multi_t *a, const uint64_multi_t *b,
                          uint64_multi_t *low, uint64_multi_t *high) {
    // Multiplicación multipalabra O(n²), donde n es el número de palabras.
    uint32_t temp[8] = {0};
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            uint32_t mul = (uint32_t)a->words[i] * (uint32_t)b->words[j];
            temp[i+j] += mul;
            // Propagación del acarreo
            for (int k = i+j; k < 7 && temp[k] > 0xFFFF; k++) {
                uint32_t carry = temp[k] >> 16;
                temp[k] &= 0xFFFF;
                temp[k+1] += carry;
            }
        }
    }

    // Parte baja (64 bits)
    for (int i = 0; i < 4; i++) {
        low->words[i] = (uint16_t)temp[i];
    }
    // Parte alta (64 bits)
    for (int i = 0; i < 4; i++) {
        high->words[i] = (uint16_t)temp[i+4];
    }
}
```
Para generar un primo grande se necesitan test de primalidad repetidos, cada uno con múltiples exponentiaciones modulares (cada exponentiación es O(n² log n) debido a las multiplicaciones repetidas). Esto vuelve el proceso extremadamente lento.
