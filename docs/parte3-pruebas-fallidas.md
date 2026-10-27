# Parte 5: pruebas fallidas y corrección de errores

## Cambio incorrecto realizado

En `src/calculator.cpp` se modificó temporalmente la función `is_even`, cambiando la comparación `== 0` por `== 1`:

```cpp
// Versión correcta
bool is_even(int number) {
    return number % 2 == 0;
}

// Versión con error intencional
bool is_even(int number) {
    return number % 2 == 1;
}
```

Con este cambio la función queda invertida: devuelve `true` para los impares y `false` para los pares.

## Comandos usados

```bash
cd build
make
./run_tests
```

## Pruebas que fallaron

Fallaron 2 de las 27 pruebas, las dos de `CalculatorTest` que usan `is_even`:

- `CalculatorTest.DetectEvenNumber`
- `CalculatorTest.DetectOddNumber`

Las otras 25 pasaron, lo que muestra que el error quedó localizado en la función modificada y no afectó a los demás módulos.

## Mensaje de error mostrado por Google Test

Extracto de la salida de `./run_tests` con el error introducido:

```text
[ RUN      ] CalculatorTest.DetectEvenNumber
.../tests/test_calculator.cpp:29: Failure
Value of: is_even(8)
  Actual: false
Expected: true
[  FAILED  ] CalculatorTest.DetectEvenNumber (0 ms)
[ RUN      ] CalculatorTest.DetectOddNumber
.../tests/test_calculator.cpp:33: Failure
Value of: is_even(7)
  Actual: true
Expected: false
[  FAILED  ] CalculatorTest.DetectOddNumber (0 ms)

[==========] 27 tests from 3 test suites ran. (0 ms total)
[  PASSED  ] 25 tests.
[  FAILED  ] 2 tests, listed below:
[  FAILED  ] CalculatorTest.DetectEvenNumber
[  FAILED  ] CalculatorTest.DetectOddNumber

 2 FAILED TESTS
```

## Qué esperaba la prueba y qué obtuvo realmente

| Prueba | Expresión evaluada | Esperado | Obtenido |
|---|---|---|---|
| `DetectEvenNumber` (línea 29) | `is_even(8)` | `true` | `false` |
| `DetectOddNumber` (línea 33) | `is_even(7)` | `false` | `true` |

Con `% 2 == 1`: `8 % 2` vale `0`, así que la comparación con `1` da `false` (debía ser `true`); y `7 % 2` vale `1`, así que la comparación da `true` (debía ser `false`).

## Corrección realizada

Se restauró la comparación original en `src/calculator.cpp`:

```cpp
bool is_even(int number) {
    return number % 2 == 0;
}
```

Luego se recompiló y se ejecutaron nuevamente las pruebas:

```bash
make
./run_tests
```

## Resultado final

```text
[==========] 27 tests from 3 test suites ran. (0 ms total)
[  PASSED  ] 27 tests.
```

Además, `git diff src/calculator.cpp` no mostró diferencias, lo que confirma que la función quedó idéntica a la versión original.

## Qué se aprendió

Que una prueba bien escrita señala con precisión dónde está el problema: el reporte indicó la línea de la prueba, la expresión evaluada y los valores esperado y obtenido, lo que permitió deducir la causa sin depurar. También que las pruebas protegen contra cambios accidentales: un cambio de un solo carácter (`0` por `1`) se detectó de inmediato. Y que la condición `% 2 == 0` es la más robusta para paridad, ya que en C++ el resto de un número negativo impar es `-1` (por ejemplo `-3 % 2`), de modo que comparar con `1` fallaría también para los negativos.

## Preguntas de reflexión

**1. ¿Cómo ayudó Google Test a identificar el error?**
Ejecutó todas las pruebas automáticamente y marcó exactamente cuáles fallaban. Como solo fallaron las dos pruebas que usan `is_even`, el error quedó acotado a esa función sin revisar el resto del código.

**2. ¿Qué información útil muestra una prueba fallida?**
El archivo y la línea de la verificación que falló, la expresión evaluada (`is_even(8)`), el valor obtenido (`Actual`) y el valor esperado (`Expected`). También el resumen final con la cantidad de pruebas que pasaron y fallaron.

**3. ¿Por qué es importante ejecutar pruebas después de modificar código?**
Porque cualquier cambio puede introducir errores, incluso uno pequeño o hecho con buena intención. Ejecutar las pruebas inmediatamente permite detectarlo cuando el cambio todavía está fresco y es fácil de ubicar y corregir.

**4. ¿Qué riesgo existe si se cambia código y no se ejecutan las pruebas?**
Que el error pase sin detectarse y llegue a otras partes del sistema o a producción, donde es más difícil y costoso de encontrar. También que se acumulen varios cambios y, cuando algo falle, no se sepa cuál lo causó.