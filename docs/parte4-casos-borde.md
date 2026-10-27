# Parte 6: diseño de casos borde

## Qué es un caso borde

Un caso borde es una entrada ubicada en el límite de lo que una función acepta o en el punto donde su comportamiento cambia: el valor mínimo o máximo permitido, el valor exacto en que se pasa de un resultado a otro, o una entrada "vacía" o de tamaño mínimo (cadena vacía, un solo carácter, el número cero). Son los lugares donde es más probable que un error se esconda.

## Casos borde agregados

Se agregaron 13 pruebas, que llevaron el total de 27 a 40.

| Archivo | Prueba | Entrada | Resultado esperado |
|---|---|---|---|
| `test_calculator.cpp` | `DivideNegativeNumbers` | `divide(-10, -2)` | `5` |
| `test_calculator.cpp` | `DividePositiveByNegative` | `divide(10, -2)` | `-5` |
| `test_calculator.cpp` | `ZeroIsEven` | `is_even(0)` | `true` |
| `test_string_utils.cpp` | `EmptyStringToUppercase` | `to_uppercase("")` | `""` |
| `test_string_utils.cpp` | `EmptyStringIsPalindrome` | `is_palindrome("")` | `true` |
| `test_string_utils.cpp` | `SingleLetterIsPalindrome` | `is_palindrome("a")` | `true` |
| `test_grade_utils.cpp` | `MinimumValidGrade` | `letter_grade(0)` | `'F'` |
| `test_grade_utils.cpp` | `MaximumValidGrade` | `letter_grade(100)` | `'A'` |
| `test_grade_utils.cpp` | `NegativeGradeThrowsException` | `letter_grade(-1)` | `std::invalid_argument` |
| `test_grade_utils.cpp` | `GradeBoundaryBetweenFAndD` | `59` y `60` | `'F'` y `'D'` |
| `test_grade_utils.cpp` | `GradeBoundaryBetweenDAndC` | `69` y `70` | `'D'` y `'C'` |
| `test_grade_utils.cpp` | `GradeBoundaryBetweenCAndB` | `79` y `80` | `'C'` y `'B'` |
| `test_grade_utils.cpp` | `GradeBoundaryBetweenBAndA` | `89` y `90` | `'B'` y `'A'` |

## Por qué estos casos son importantes

- **Divisiones con negativos:** el signo del resultado depende de los dos operandos. Una implementación que solo considerara positivos podría devolver un signo equivocado y no se notaría con las pruebas normales.
- **Cero es par:** `0` es un valor especial que no es ni positivo ni negativo. Una implementación que trate el cero como caso aparte podría equivocarse.
- **Cadena vacía y un solo carácter:** son los tamaños mínimos de entrada. Muchos errores aparecen cuando no hay nada que recorrer o solo hay un elemento. Por ejemplo, una implementación que accediera a `text[0]` o `text[text.size() - 1]` sin verificar fallaría con la cadena vacía.
- **Notas 0 y 100:** son los extremos del rango válido. Comprueban que ninguno de los dos se rechace por error.
- **Nota -1:** es el primer valor inválido por debajo del rango. Verifica que el límite inferior se aplica correctamente.
- **Pares 59/60, 69/70, 79/80 y 89/90:** son los puntos exactos donde cambia la letra. Cada par prueba el último valor de una categoría y el primero de la siguiente.

## Resultado de las pruebas

Salida de `./run_tests` (extracto final):

```text
[----------] 13 tests from CalculatorTest (0 ms total)
[----------] 10 tests from StringUtilsTest (0 ms total)
[----------] 17 tests from GradeUtilsTest (0 ms total)

[==========] 40 tests from 3 test suites ran. (1 ms total)
[  PASSED  ] 40 tests.
```

Salida de `ctest --output-on-failure` (extracto final):

```text
100% tests passed, 0 tests failed out of 40

Total Test time (real) =   0.13 sec
```

## Problemas encontrados y cómo se corrigieron

Las pruebas no fallaron por un error del código, pero hubo un problema al incorporarlas:

1. Las tres pruebas de `StringUtilsTest` se escribieron por error al final de `test_grade_utils.cpp`, en lugar de `test_string_utils.cpp`. Ese archivo no incluye `string_utils.h`, así que las funciones `to_uppercase` e `is_palindrome` no estaban declaradas y la compilación falló.
2. Como `make` no pudo generar un ejecutable nuevo, `ctest` ejecutó el binario anterior y reportó 30 pruebas pasando. Ese resultado estaba desactualizado: no incluía ninguna de las pruebas nuevas de `string_utils` ni de `grade_utils`.
3. Se movieron las tres pruebas a `test_string_utils.cpp`.
4. Después se notó que seguían faltando las tres pruebas de `CalculatorTest` (el total era 37 en lugar de 40). Se verificó el estado de `test_calculator.cpp`, se forzó su recompilación con `touch` y se volvió a ejecutar: el resultado fue 13 + 10 + 17 = 40 pruebas pasando.

La lección práctica es comprobar siempre que `make` termine sin errores antes de interpretar el resultado de `ctest`, y contrastar el número de pruebas reportado con el esperado.

## Preguntas de reflexión

**1. ¿Por qué no basta con probar casos normales?**
Porque los casos normales suelen funcionar incluso cuando el código tiene errores en los límites. Una función puede pasar con `85` y fallar con `80`. Los errores más comunes (`>` en lugar de `>=`, un índice fuera de rango, un valor vacío no considerado) solo se revelan con entradas especiales.

**2. ¿Qué es un caso borde?**
Una entrada en el límite del comportamiento de la función: el mínimo o máximo permitido, el punto exacto donde cambia el resultado, o entradas de tamaño mínimo como una cadena vacía o un solo elemento.

**3. ¿Qué es un caso inválido?**
Una entrada que está fuera de lo que la función acepta y para la cual el comportamiento correcto es rechazarla, normalmente con una excepción. Ejemplos de este laboratorio: `divide(10, 0)`, `average` de un vector vacío y `letter_grade(-1)` o `letter_grade(120)`. Se diferencia de un caso borde en que el borde sigue siendo una entrada válida, mientras que el inválido queda fuera del dominio.

**4. ¿Qué diferencia hay entre probar `85` y probar exactamente `80`, `90` o `70`?**
`85` está en medio del rango de la `B`, así que cualquier implementación razonable devuelve `'B'`. `80` es el valor donde se decide la condición. Si el código dijera `grade > 80` en lugar de `grade >= 80`, `85` pasaría igual, pero `80` devolvería `'C'` y la prueba lo detectaría. Solo los valores exactos de los límites exponen ese tipo de error.

**5. ¿Cómo puede un caso borde revelar errores ocultos?**
Porque obliga al código a ejecutar las ramas y comparaciones que los casos normales nunca ejercitan. Errores como una desigualdad mal escrita, un recorrido que asume al menos un elemento o un caso no contemplado quedan latentes y funcionan "casi siempre"; el caso borde los hace aparecer de forma controlada, en la prueba y no en producción.

## Semillas en pruebas

## Semillas en pruebas

### Qué se hizo

Se agregó `#include <random>` a `tests/test_calculator.cpp` y una prueba que genera pares de enteros pseudoaleatorios y verifica que `add` devuelva su suma. El generador se inicializa con una semilla fija.

### Código agregado

```cpp
#include <random>
```

```cpp
TEST(CalculatorTest, RandomAdditionsWithFixedSeed) {
    std::mt19937 generator(12345);
    std::uniform_int_distribution<int> distribution(-100, 100);

    for (int i = 0; i < 10; i++) {
        int a = distribution(generator);
        int b = distribution(generator);

        EXPECT_EQ(add(a, b), a + b);
    }
}
```

### Qué hace la semilla

`std::mt19937` es un generador de números pseudoaleatorios (Mersenne Twister). No produce números realmente aleatorios sino una secuencia determinista calculada a partir de un estado inicial, y la semilla (`12345`) es el valor que fija ese estado. Dada la misma semilla, el generador produce siempre la misma secuencia. La distribución `uniform_int_distribution<int>(-100, 100)` convierte esa secuencia en enteros dentro del rango `[-100, 100]`.

### Por qué se usa una semilla fija

Para que la prueba sea repetible. Con una semilla fija, cada ejecución usa exactamente los mismos 10 pares de valores, en la misma máquina y con el mismo compilador. Si la prueba falla, falla siempre con los mismos datos, y se puede reproducir el fallo tantas veces como haga falta. Con una semilla distinta en cada ejecución (por ejemplo, tomada de la hora o de `std::random_device`), la prueba podría pasar hoy y fallar mañana sin que cambie el código.

### Evidencia: misma semilla, mismos valores

Para ver los valores se agregó temporalmente un `std::cout` dentro del bucle. La prueba se ejecutó dos veces con la semilla `12345` y ambas imprimieron la misma secuencia:

```text
a=86 b=78
a=-37 b=-74
a=-64 b=-93
a=-59 b=66
a=14 b=6
a=19 b=92
a=93 b=-8
a=31 b=85
a=50 b=-25
a=31 b=-69
[       OK ] CalculatorTest.RandomAdditionsWithFixedSeed (0 ms)
```

### Qué pasó al cambiar la semilla

Se cambió temporalmente la línea a `std::mt19937 generator(54321);`, se recompiló y se ejecutó. La prueba siguió pasando, pero con valores completamente distintos:

```text
a=83 b=2
a=25 b=-64
a=59 b=62
a=-14 b=88
a=9 b=-95
a=-17 b=57
a=-83 b=-68
a=56 b=-13
a=-2 b=15
a=-90 b=-57
[       OK ] CalculatorTest.RandomAdditionsWithFixedSeed (0 ms)
```

| | Semilla `12345` | Semilla `54321` |
|---|---|---|
| Primer par | `a=86 b=78` | `a=83 b=2` |
| Último par | `a=31 b=-69` | `a=-90 b=-57` |
| Resultado | Pasa | Pasa |

Pasa con ambas semillas porque `add` es correcta para cualquier par de enteros. Lo que cambió fueron los datos de entrada, no el resultado. Si `add` tuviera un error que solo apareciera con ciertos valores, una semilla podría revelarlo y otra no.

### Restauración

Se devolvió la semilla a `12345` y se eliminó el `std::cout` temporal. La ejecución completa con la semilla original dio:

```text
[==========] 41 tests from 3 test suites ran. (1 ms total)
[  PASSED  ] 41 tests.
```

### Por qué esto ayuda en debugging

Porque permite reproducir un fallo a voluntad. Si una prueba aleatoria fallara, bastaría con conocer la semilla para regenerar exactamente los mismos valores, ponerlos en un depurador o imprimirlos y examinar la causa. Sin semilla fija, el fallo podría no volver a aparecer y sería casi imposible de diagnosticar. Además, otra persona (o el sistema de integración continua) puede reproducir el mismo resultado.

### Preguntas de reflexión

**1. ¿Por qué las pruebas con datos aleatorios pueden ser peligrosas si no se controlan?**
Porque pueden dar resultados distintos en cada ejecución. Una prueba que a veces pasa y a veces falla con el mismo código es no determinista: no se sabe si el fallo es un error real o mala suerte, y si falla no hay forma segura de repetirlo para investigar. Esto además erosiona la confianza en el conjunto de pruebas.

**2. ¿Qué ventaja tiene usar una semilla fija?**
Que la secuencia de valores es siempre la misma, así que la prueba es determinista y repetible. Se conserva la variedad de usar muchos valores distintos sin perder la reproducibilidad.

**3. ¿Cómo ayuda la semilla a reproducir errores?**
La semilla determina por completo los valores generados. Si un fallo ocurre con cierta semilla, volver a ejecutar con esa misma semilla produce los mismos datos y el mismo fallo, lo que permite estudiarlo y confirmar que quedó corregido.

**4. ¿Por qué es importante documentar la semilla usada?**
Porque es la información necesaria para reproducir la ejecución. Si no se registra, nadie sabrá con qué datos falló una prueba ni podrá repetirlo. Documentarla permite que otra persona, o uno mismo más adelante, obtenga exactamente los mismos resultados.

### Qué se aprendió

Que "aleatorio" en una prueba debe significar "pseudoaleatorio con semilla controlada". También que `std::mt19937` produce una secuencia definida por el estándar, pero `uniform_int_distribution` no garantiza los mismos valores entre distintas implementaciones de la biblioteca estándar, por lo que la reproducibilidad exacta de los números es segura en el mismo compilador y la misma biblioteca, como ocurre al repetir las pruebas en la misma máquina.