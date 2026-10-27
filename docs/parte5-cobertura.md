# Parte 8: cobertura de código

## Qué es cobertura de código

La cobertura de código es una métrica que indica qué partes del código fuente se ejecutaron mientras corrían las pruebas. Se puede medir de distintas formas:

- **Cobertura de líneas:** qué porcentaje de las líneas ejecutables se ejecutó al menos una vez.
- **Cobertura de funciones:** qué porcentaje de las funciones se llamó al menos una vez.
- **Cobertura de ramas:** qué porcentaje de los posibles resultados de cada decisión (`if`, `||`, bucles) se tomó al menos una vez.

Sirve para encontrar código que ninguna prueba ejercita. No mide si las pruebas verifican bien lo que ejecutan.

## Qué se hizo

Se recompiló el proyecto con instrumentación de cobertura (`-DENABLE_COVERAGE=ON`, que agrega `--coverage -O0 -g`), se ejecutaron las 41 pruebas para generar los datos de ejecución, y con `lcov` se capturaron, filtraron y convirtieron esos datos en un reporte HTML.

Entorno: GCC y gcov 15.2.0, lcov 2.0-1 y CMake 4.2.

## Archivos modificados

No se modificó código del proyecto. Los archivos generados (`coverage.info`, `coverage_filtered.info` y `coverage_report/`) quedan dentro de `build/`, que está excluido del repositorio con `.gitignore`. La evidencia se incluye en este documento y en `docs/img/`.

## Comandos utilizados

Compilar con cobertura y ejecutar las pruebas (desde `laboratorio-testing`):

```bash
rm -rf build
mkdir build
cd build
cmake -DENABLE_COVERAGE=ON ..
make
./run_tests
```

Generar el reporte (desde `build`):

```bash
OPTS="--rc branch_coverage=1 --ignore-errors inconsistent,mismatch,unused"
lcov --capture --directory . --output-file coverage.info $OPTS
lcov --remove coverage.info '/usr/*' '*/_deps/*' '*/tests/*' --output-file coverage_filtered.info $OPTS
genhtml coverage_filtered.info --output-directory coverage_report $OPTS
xdg-open coverage_report/index.html
```

Para obtener los datos usados en esta documentación:

```bash
lcov --summary coverage_filtered.info $OPTS
lcov --list coverage_filtered.info $OPTS
awk -F: '/^SF:/{f=$2} /^DA:/{split($2,a,","); if (a[2]==0) print f ":" a[1]}' coverage_filtered.info
```

La opción `--rc branch_coverage=1` activa la medición de ramas, que `lcov` 2.x no incluye por defecto. El filtro `--remove` excluye las bibliotecas del sistema, GoogleTest y los propios archivos de pruebas, para medir solo el código del proyecto (`src/`).

## Problema encontrado y cómo se resolvió

Con los comandos del enunciado, `lcov --capture` falló con este error y no generó `coverage.info`:

```text
lcov: ERROR: (inconsistent) mismatched end line for _ZN43StringUtilsTest_ConvertTextToUppercase_Test8TestBodyEv
at .../tests/test_string_utils.cpp:4: 4 -> 6 while capturing from ...
(use "lcov --ignore-errors inconsistent ..." to bypass this error)
```

Como no se creó `coverage.info`, los comandos siguientes fallaron en cascada (`cannot read file coverage.info`, y `genhtml` y `lcov --summary` no encontraron `coverage_filtered.info`).

La causa es una incompatibilidad entre versiones: `gcov` 15.2.0 reporta rangos de líneas ligeramente distintos para las funciones que genera la macro `TEST` de GoogleTest, y `lcov` 2.0 trata esa incoherencia como un error fatal. No afecta los datos de cobertura del código del proyecto.

Se resolvió agregando `--ignore-errors inconsistent,mismatch,unused` a los comandos. Con eso, los errores pasaron a ser advertencias (103 en total: 98 de tipo `inconsistent` y 5 de tipo `mismatch`) y el reporte se generó correctamente. La categoría `unused` evita que `--remove` falle si algún patrón de exclusión no coincide con ningún archivo.

## Resultado de la cobertura

### Antes y después del filtro

| | Archivos | Líneas | Funciones | Ramas |
|---|---|---|---|---|
| Captura completa (con GoogleTest y bibliotecas del sistema) | 79 | 37.9% (2636 de 6946) | 42.4% (1259 de 2967) | 14.0% (1235 de 8816) |
| Solo el código del proyecto (`src/`) | 3 | **100%** (54 de 54) | **100%** (11 de 11) | **87.5%** (42 de 48) |

El 37.9% de la captura completa no es la cobertura del proyecto: incluye encabezados de GoogleTest y de la biblioteca estándar. El resultado relevante es el de la segunda fila.

### Salida de `lcov --list`

```text
                        |Lines       |Functions  |Branches    
Filename                |Rate     Num|Rate    Num|Rate     Num
==============================================================
[.../laboratorio-testing/src/]
calculator.cpp          | 100%     12| 100%     5|75.0%      4
grade_utils.cpp         | 100%     21| 100%     3|90.0%     20
string_utils.cpp        | 100%     21| 100%     3|87.5%     24
==============================================================
                  Total:| 100%     54| 100%    11|87.5%     48
```

### Archivos con mayor y menor cobertura

En líneas y funciones los tres archivos están empatados al 100%. La diferencia está en las ramas:

- **Mayor cobertura de ramas:** `grade_utils.cpp`, con 90.0% (18 de 20).
- **Intermedio:** `string_utils.cpp`, con 87.5% (21 de 24).
- **Menor cobertura de ramas:** `calculator.cpp`, con 75.0% (3 de 4). Es el porcentaje más bajo, aunque el archivo tiene solo una rama sin cubrir.

### Líneas no cubiertas

Ninguna. El comando `awk` que lista las líneas con cero ejecuciones no imprimió nada.

### Ramas no cubiertas

Hay 6 ramas sin cubrir, y todas son del mismo tipo: **ramas de excepción** (`throw`) que el compilador genera en las llamadas que podrían lanzar una excepción, por ejemplo si falla una reserva de memoria (`std::bad_alloc`). Las decisiones del programa (`if`, `||`, bucles) tienen todas sus ramas cubiertas.

| Archivo | Línea | Código | Rama sin cubrir |
|---|---|---|---|
| `calculator.cpp` | 18 | `throw std::invalid_argument("No se puede dividir entre cero");` | Que falle la construcción de la excepción |
| `grade_utils.cpp` | 6 | `throw std::invalid_argument("La lista de notas no puede estar vacía");` | Que falle la construcción de la excepción |
| `grade_utils.cpp` | 24 | `throw std::invalid_argument("La nota debe estar entre 0 y 100");` | Que falle la construcción de la excepción |
| `string_utils.cpp` | 20 | `normalized += static_cast<char>(std::tolower(...));` | Que `+=` lance una excepción (memoria insuficiente) |
| `string_utils.cpp` | 24 | `std::string reversed = normalized;` | Que la copia lance una excepción (memoria insuficiente) |
| `string_utils.cpp` | 25 | `std::reverse(reversed.begin(), reversed.end());` | Que `std::reverse` lance una excepción |

Estas ramas no se pueden alcanzar con pruebas normales, porque hace falta que una operación interna falle, algo que no ocurre en una ejecución sana.

### Captura del reporte HTML

![Página principal del reporte de cobertura](img/cobertura-index.png)

_(Agregar la captura de `coverage_report/index.html` en `docs/img/cobertura-index.png`.)_

## Pruebas adicionales que podrían aumentar la cobertura

Como líneas y funciones ya están al 100%, lo único que se puede mejorar son estas ramas de excepción. Se alcanzan con **inyección de fallos**: reemplazar `operator new` en el archivo de pruebas por una versión que, cuando se le indica, lanza `std::bad_alloc`. Con eso se fuerza el fallo de memoria y se ejecuta la rama `throw`.

Se comprobó este mecanismo en un programa independiente, sin GoogleTest, compilado con `--coverage` (GCC 13 y gcov 13): las ramas de las líneas indicadas pasaron de "no tomada" a "tomada". Las pruebas siguientes son **propuestas y no se agregaron a la suite del laboratorio**, de modo que no se ejecutaron dentro de GoogleTest.

Infraestructura común, al inicio de un archivo de pruebas (por ejemplo, un `test_fault_injection.cpp` nuevo que se agregue a `add_executable` en `CMakeLists.txt`):

```cpp
#include <gtest/gtest.h>
#include <cstdlib>
#include <new>
#include <string>
#include <vector>
#include "calculator.h"
#include "grade_utils.h"
#include "string_utils.h"

static int g_alloc_count = 0;
static int g_fail_at = 0;   // 0 = no inyectar fallos

// Hace que la n-ésima reserva de memoria a partir de este momento lance std::bad_alloc.
static void fail_allocation_number(int n) {
    g_alloc_count = 0;
    g_fail_at = n;
}

void* operator new(std::size_t size) {
    if (g_fail_at != 0 && ++g_alloc_count == g_fail_at) {
        g_fail_at = 0;
        throw std::bad_alloc();
    }
    void* p = std::malloc(size);
    if (!p) throw std::bad_alloc();
    return p;
}
void operator delete(void* p) noexcept { std::free(p); }
void operator delete(void* p, std::size_t) noexcept { std::free(p); }
```

**Propuesta 1: fallo de memoria al lanzar las excepciones de validación.** La construcción de `std::invalid_argument` reserva memoria para el mensaje; si esa reserva falla, se toma la rama `throw` de la propia construcción. Cubre `calculator.cpp:18`, `grade_utils.cpp:6` y `grade_utils.cpp:24`.

```cpp
TEST(FaultInjectionTest, AllocationFailureWhileBuildingException) {
    fail_allocation_number(1);
    EXPECT_THROW(letter_grade(120), std::bad_alloc);

    fail_allocation_number(1);
    EXPECT_THROW(divide(10, 0), std::bad_alloc);

    fail_allocation_number(1);
    EXPECT_THROW(average({}), std::bad_alloc);
}
```

**Propuesta 2: fallo de memoria en `is_palindrome` con una cadena larga.** Las cadenas de hasta 15 caracteres usan el buffer interno de `std::string` y no reservan memoria, así que hace falta una cadena más larga. La 1.ª reserva ocurre cuando `normalized` crece (línea 20) y la 2.ª al copiarla a `reversed` (línea 24). Cubre `string_utils.cpp:20` y `string_utils.cpp:24`.

```cpp
TEST(FaultInjectionTest, AllocationFailureInPalindromeCheck) {
    const std::string long_text = "abcdefghijklmnopqrstuvwxyz";   // 26 caracteres

    for (int n = 1; n <= 2; n++) {
        fail_allocation_number(n);
        EXPECT_THROW(is_palindrome(long_text), std::bad_alloc);
    }
}
```

En el programa independiente, ambas reservas (n igual a 1 y a 2) provocaron `std::bad_alloc` y marcaron como tomadas las ramas de las líneas 20 y 24. El número exacto de reservas depende de cómo crece `std::string` en la biblioteca estándar usada, así que si se implementa conviene confirmarlo con el reporte de cobertura.

Con estas pruebas se estima que la cobertura de ramas pasaría de 42 de 48 (87.5%) a 47 de 48 (97.9%), con `calculator.cpp` y `grade_utils.cpp` al 100%. La rama restante, la de `std::reverse` en la línea 25, no se puede cubrir de esta manera, porque `std::reverse` no reserva memoria ni lanza excepciones en condiciones normales.

Estas pruebas son una técnica avanzada. Para un proyecto real conviene valorar si el esfuerzo vale la pena, porque protegen rutas de error poco probables y complican el archivo de pruebas.

## Preguntas de reflexión

**1. ¿Qué significa tener 100% de cobertura?**
Que, según la métrica usada, todo lo medido se ejecutó al menos una vez durante las pruebas: todas las líneas, funciones o ramas, según el caso. En este laboratorio hay 100% de líneas y de funciones, pero 87.5% de ramas, así que "100%" siempre debe ir acompañado de qué se está midiendo.

**2. ¿Tener 100% de cobertura garantiza que el programa no tiene errores?**
No. La cobertura indica que el código se ejecutó, no que sus resultados se hayan verificado ni que se hayan probado todas las entradas posibles. Una prueba sin aserciones, o con una aserción débil, aporta cobertura sin detectar ningún error.

**3. ¿Qué diferencia hay entre cobertura de líneas y cobertura de ramas?**
La cobertura de líneas solo pregunta si cada línea se ejecutó. La de ramas pregunta si cada resultado posible de cada decisión se tomó. Una línea como `if (b == 0) throw ...;` cuenta como cubierta si se ejecuta con cualquier valor de `b`, pero para cubrir sus dos ramas hacen falta pruebas con `b == 0` y con `b != 0`. Por eso en este laboratorio las líneas están al 100% y las ramas no.

**4. ¿Por qué una línea ejecutada no necesariamente significa que fue bien probada?**
Porque ejecutar una línea no es lo mismo que comprobar su resultado. Una prueba podría llamar a `letter_grade(80)` y no verificar la letra devuelta: la línea quedaría cubierta, pero un error como `>` en lugar de `>=` pasaría sin ser detectado. La calidad depende de las aserciones y de los casos elegidos, como los casos borde de la Parte 6.

**5. ¿Cómo puede ayudar la cobertura a mejorar las pruebas?**
Señala qué código ninguna prueba ha ejercitado, y eso indica dónde faltan pruebas. En este laboratorio mostró que todas las decisiones del código tienen sus dos ramas cubiertas y que lo pendiente son únicamente rutas de excepción. Sirve como guía para decidir qué probar, pero no sustituye el criterio sobre qué casos son importantes.

## Qué se aprendió

Que una herramienta nueva puede ser incompatible con el compilador instalado (`lcov` 2.0 con `gcov` 15), y que hay que leer el mensaje de error, que en este caso indicaba la solución. También que la captura sin filtrar da un número engañoso (37.9%), porque mide también las bibliotecas, y que lo relevante es filtrar y medir solo el código propio. Y que el 100% de líneas no implica 100% de ramas: las ramas de excepción generadas por el compilador son las más difíciles de cubrir y exigen técnicas especiales como la inyección de fallos.