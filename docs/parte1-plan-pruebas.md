# Parte 1: preparación del proyecto y plan de pruebas

## Qué se hizo

Se creó el repositorio `laboratorio-testing`, con la estructura `include/`, `src/`, `tests/` y `docs/`, y se agregó el archivo `CMakeLists.txt` que configura la compilación y descarga Google Test. El workflow de GitHub Actions se ubicó en `.github/workflows/testing.yml`, en la raíz del repositorio, porque GitHub solo detecta workflows en esa ruta.

## Qué es CMake

CMake es una herramienta que genera los archivos de compilación (por ejemplo, Makefiles) a partir de una descripción del proyecto en `CMakeLists.txt`. Describe qué fuentes forman cada biblioteca o ejecutable y de qué dependen, y la compilación se hace con los mismos comandos en distintas máquinas.

## Para qué sirve Google Test

Google Test es un framework de pruebas unitarias para C++. Aporta macros para declarar pruebas (`TEST`), aserciones para verificar resultados (`EXPECT_EQ`, `ASSERT_EQ`, `EXPECT_THROW`, etc.), un ejecutable que corre todas las pruebas y un reporte claro con lo esperado y lo obtenido cuando una falla.

## Qué significa automatizar pruebas

Automatizar significa que las pruebas se ejecutan con un comando, sin intervención manual y sin que una persona revise a ojo cada resultado. El programa decide solo si pasó o falló. Así se pueden correr después de cada cambio.

## Qué significa que las pruebas sean repetibles

Una prueba es repetible si, ejecutada varias veces en las mismas condiciones y en distintos momentos o ambientes, produce siempre el mismo resultado. Esto exige que no dependa de factores incontrolados como la hora, el orden de ejecución o valores aleatorios sin semilla.

## Archivos y carpetas creados

| Ruta | Propósito |
|---|---|
| `CMakeLists.txt` | Configuración de compilación, Google Test y cobertura |
| `include/` | Encabezados (`calculator.h`, `string_utils.h`, `grade_utils.h`) |
| `src/` | Implementaciones (`.cpp`) |
| `tests/` | Pruebas unitarias con Google Test |
| `docs/` | Documentación en Markdown |
| `.github/workflows/testing.yml` | Workflow de integración continua |

## Preguntas de reflexión

**1. ¿Por qué conviene separar el código fuente de las pruebas?**
Porque el código de producción queda limpio y se puede entregar sin las pruebas, y las pruebas se pueden evolucionar sin tocar la implementación. También deja claro qué es el programa y qué es su verificación.

**2. ¿Qué ventaja tiene usar CMake en un proyecto de C++?**
Estandariza la compilación, gestiona dependencias como Google Test y permite activar opciones (como la cobertura) sin reescribir comandos de `g++`.

**3. ¿Por qué es útil que las pruebas se puedan ejecutar con un solo comando?**
Reduce la fricción, así que se ejecutan más seguido. Además, el mismo comando sirve en la máquina local y en CI.

**4. ¿Qué diferencia hay entre probar manualmente y probar automáticamente?**
Manualmente depende de la persona, es lento y propenso a omisiones. Automáticamente es rápido, consistente y se puede repetir tras cada cambio para detectar regresiones.

## Tabla de casos funcionales

### Requisito

> El sistema debe convertir una nota numérica entre 0 y 100 a una letra de calificación. Las notas entre 90 y 100 son A, entre 80 y 89 son B, entre 70 y 79 son C, entre 60 y 69 son D, y menores a 60 son F. Si la nota está fuera del rango permitido, el sistema debe rechazarla.

Este requisito lo implementa la función `letter_grade` del módulo `grade_utils`, que devuelve la letra o lanza `std::invalid_argument` si la nota está fuera de 0 a 100.

### Tabla de casos

Los cinco primeros casos (TC-001 a TC-005) son los del enunciado. Los demás (TC-006 a TC-018) se agregaron para cubrir cada letra, cada límite entre letras y los extremos del rango, de modo que la tabla cubra completo el requisito.

| ID | Requisito | Entrada | Resultado esperado | Tipo de caso | Prueba que lo cubre (`GradeUtilsTest.`) |
|---|---|---:|---|---|---|
| TC-001 | Convertir nota excelente | 95 | A | Normal | `LetterGradeA` |
| TC-002 | Límite inferior de A | 90 | A | Borde | `GradeBoundaryBetweenBAndA` |
| TC-003 | Límite superior de B | 89 | B | Borde | `GradeBoundaryBetweenBAndA` |
| TC-004 | Nota inválida baja | -1 | Excepción | Inválido | `NegativeGradeThrowsException` |
| TC-005 | Nota inválida alta | 101 | Excepción | Inválido | `GradeAboveMaximumThrowsException` (**agregada**) |
| TC-006 | Límite superior de A | 100 | A | Borde | `MaximumValidGrade` |
| TC-007 | Convertir nota buena | 85 | B | Normal | `LetterGradeB` |
| TC-008 | Límite inferior de B | 80 | B | Borde | `GradeBoundaryBetweenCAndB` |
| TC-009 | Límite superior de C | 79 | C | Borde | `GradeBoundaryBetweenCAndB` |
| TC-010 | Convertir nota aceptable | 75 | C | Normal | `LetterGradeC` |
| TC-011 | Límite inferior de C | 70 | C | Borde | `GradeBoundaryBetweenDAndC` |
| TC-012 | Límite superior de D | 69 | D | Borde | `GradeBoundaryBetweenDAndC` |
| TC-013 | Convertir nota baja | 65 | D | Normal | `LetterGradeD` |
| TC-014 | Límite inferior de D | 60 | D | Borde | `GradeBoundaryBetweenFAndD` |
| TC-015 | Límite superior de F | 59 | F | Borde | `GradeBoundaryBetweenFAndD` |
| TC-016 | Convertir nota reprobada | 50 | F | Normal | `LetterGradeF` |
| TC-017 | Mínimo del rango válido | 0 | F | Borde | `MinimumValidGrade` |
| TC-018 | Nota inválida muy alta | 120 | Excepción | Inválido | `InvalidGradeThrowsException` |

### Casos que ya estaban cubiertos

17 de los 18 casos ya tenían una prueba en `tests/test_grade_utils.cpp` (de las Partes 3 y 6): TC-001 a TC-004 y TC-006 a TC-018. Algunas pruebas cubren dos casos a la vez, porque verifican los dos lados de un límite (por ejemplo, `GradeBoundaryBetweenBAndA` comprueba 89 y 90).

### Casos que se tuvieron que agregar

Solo uno: **TC-005 (nota 101)**. Aunque existía `InvalidGradeThrowsException`, esa prueba usa la nota `120`, no `101`. El valor 101 es el primero que queda fuera del rango por arriba, y es el que revelaría un error de límite. Si el código tuviera `grade > 101` en lugar de `grade > 100`, la prueba con 120 seguiría pasando y la nota 101 se aceptaría por error. Se agregó:

```cpp
TEST(GradeUtilsTest, GradeAboveMaximumThrowsException) {
    EXPECT_THROW(letter_grade(101), std::invalid_argument);
}
```

La prueba de `120` se conservó, y pasó a ser el caso TC-018.

### Resultado

Prueba nueva ejecutada sola:

```text
[ RUN      ] GradeUtilsTest.GradeAboveMaximumThrowsException
[       OK ] GradeUtilsTest.GradeAboveMaximumThrowsException (0 ms)
[  PASSED  ] 1 test.
```

Suite completa:

```text
[----------] 18 tests from GradeUtilsTest (0 ms total)
[==========] 42 tests from 3 test suites ran. (0 ms total)
[  PASSED  ] 42 tests.
```

`GradeUtilsTest` pasó de 17 a 18 pruebas, y el total de 41 a 42.

### Por qué estas pruebas se consideran funcionales

Porque se diseñaron a partir del **requisito**, es decir, del comportamiento esperado, y no a partir de cómo está escrito el código. Cada caso parte de una frase del requisito (cada rango de letras, sus límites y el rechazo de notas fuera del rango) y comprueba únicamente lo que se puede observar desde fuera: dada una entrada, qué resultado o qué excepción se obtiene. No dependen de las condiciones `if` internas de `letter_grade`: si la implementación se reescribiera con una tabla de rangos, los 18 casos seguirían siendo válidos sin cambios.

Aquí se usa Google Test, como en las pruebas unitarias. Lo que hace funcional a estas pruebas es el punto de vista desde el que se diseñaron, no la herramienta.

### Preguntas de reflexión

**1. ¿Qué diferencia hay entre una prueba unitaria y una prueba funcional?**
Una prueba unitaria verifica una pieza pequeña del código (una función o una clase) de forma aislada, y suele diseñarse mirando cómo está construida. Una prueba funcional verifica que una funcionalidad cumpla su requisito, mirándola desde fuera como una caja negra (entrada, resultado esperado), y puede involucrar varias piezas. En este laboratorio ambas usan la misma función, `letter_grade`; la diferencia es que las pruebas funcionales salen del requisito y no de la implementación.

**2. ¿Por qué una prueba funcional se relaciona con requisitos?**
Porque su razón de ser es comprobar que el sistema hace lo que se pidió. Cada caso de la tabla se puede rastrear hasta una frase del requisito, así se ve qué parte está verificada. Esa trazabilidad ayudó a detectar un hueco: ninguna prueba comprobaba que la nota 101 se rechazara, algo que el requisito exige.

**3. ¿Qué significa pensar desde la perspectiva del usuario o del sistema?**
Significa preguntarse qué entrega y qué espera recibir quien usa la función o el sistema, sin fijarse en cómo se calcula por dentro. Por ejemplo: quien tenga un 89 debe ver una B, y quien ingrese un 101 debe recibir un rechazo. Eso lleva a elegir casos por su importancia para el requisito y no por la estructura del código.

**4. ¿Por qué la documentación de casos de prueba ayuda al equipo?**
Porque deja por escrito qué comportamiento se espera y cuál prueba lo verifica, de modo que cualquiera pueda revisar si algo falta (así apareció el caso de 101) sin leer todo el código. También facilita incorporar a nuevas personas, discutir los requisitos con quien no programa y volver a ejecutar o ampliar las pruebas más adelante con claridad sobre qué cubren.

