# Partes 2 a 4: código base, pruebas unitarias y EXPECT vs ASSERT

## Módulos creados

| Módulo | Encabezado | Implementación | Función |
|---|---|---|---|
| calculator | `include/calculator.h` | `src/calculator.cpp` | Operaciones aritméticas y paridad |
| string_utils | `include/string_utils.h` | `src/string_utils.cpp` | Mayúsculas, palíndromos y conteo de vocales |
| grade_utils | `include/grade_utils.h` | `src/grade_utils.cpp` | Promedio, aprobación y nota con letra |

Los `.h` declaran la interfaz pública de cada módulo (qué funciones existen y con qué firma). Los `.cpp` contienen la lógica. Se compilan juntos en la biblioteca `project_lib`, contra la que se enlaza el ejecutable de pruebas.

## Clasificación de casos por función

| Función | Normal | Borde | Inválido |
|---|---|---|---|
| `divide` | `divide(10, 2)` | negativos, cero como dividendo | divisor `0` (excepción) |
| `is_even` | `8`, `7` | `0`, negativos | — |
| `to_uppercase` | `"hola"` | cadena vacía, dígitos | — |
| `is_palindrome` | `"radar"` | cadena vacía, una letra, espacios | — |
| `count_vowels` | `"murcielago"` | cadena sin vocales, vacía | — |
| `average` | `{80, 90, 100}` | un solo elemento | vector vacío (excepción) |
| `is_passing` | `85` | `70` y `69` | — |
| `letter_grade` | `95`, `85`, `75` | `0`, `100`, `59/60`, `69/70`, `79/80`, `89/90` | `-1`, `101`, `120` |

## Parte 3: primeras pruebas unitarias

### Qué se hizo

Se crearon los tres archivos de pruebas (`tests/test_calculator.cpp`, `tests/test_string_utils.cpp` y `tests/test_grade_utils.cpp`), con 8, 7 y 10 pruebas respectivamente. Luego se compiló el proyecto con CMake y se ejecutaron las pruebas.

### Archivos modificados

- `laboratorio-testing/tests/test_calculator.cpp`
- `laboratorio-testing/tests/test_string_utils.cpp`
- `laboratorio-testing/tests/test_grade_utils.cpp`

### Comandos usados para compilar

```bash
cd laboratorio-testing
mkdir build
cd build
cmake ..
make
```

### Comandos usados para ejecutar las pruebas

```bash
./run_tests
ctest --output-on-failure
```

### Resultado de `./run_tests`

```text
Running main() from /home/gulloab/Diseño de software/Car-Labo-testing/laboratorio-testing/build/_deps/googletest-src/googletest/src/gtest_main.cc
[==========] Running 25 tests from 3 test suites.
[----------] Global test environment set-up.
[----------] 8 tests from CalculatorTest
[ RUN      ] CalculatorTest.AddPositiveNumbers
[       OK ] CalculatorTest.AddPositiveNumbers (0 ms)
[ RUN      ] CalculatorTest.AddNegativeNumbers
[       OK ] CalculatorTest.AddNegativeNumbers (0 ms)
[ RUN      ] CalculatorTest.SubtractNumbers
[       OK ] CalculatorTest.SubtractNumbers (0 ms)
[ RUN      ] CalculatorTest.MultiplyNumbers
[       OK ] CalculatorTest.MultiplyNumbers (0 ms)
[ RUN      ] CalculatorTest.DivideNumbers
[       OK ] CalculatorTest.DivideNumbers (0 ms)
[ RUN      ] CalculatorTest.DivideByZeroThrowsException
[       OK ] CalculatorTest.DivideByZeroThrowsException (0 ms)
[ RUN      ] CalculatorTest.DetectEvenNumber
[       OK ] CalculatorTest.DetectEvenNumber (0 ms)
[ RUN      ] CalculatorTest.DetectOddNumber
[       OK ] CalculatorTest.DetectOddNumber (0 ms)
[----------] 8 tests from CalculatorTest (0 ms total)

[----------] 7 tests from StringUtilsTest
[ RUN      ] StringUtilsTest.ConvertTextToUppercase
[       OK ] StringUtilsTest.ConvertTextToUppercase (0 ms)
[ RUN      ] StringUtilsTest.ConvertMixedTextToUppercase
[       OK ] StringUtilsTest.ConvertMixedTextToUppercase (0 ms)
[ RUN      ] StringUtilsTest.DetectSimplePalindrome
[       OK ] StringUtilsTest.DetectSimplePalindrome (0 ms)
[ RUN      ] StringUtilsTest.DetectPalindromeWithSpaces
[       OK ] StringUtilsTest.DetectPalindromeWithSpaces (0 ms)
[ RUN      ] StringUtilsTest.DetectNonPalindrome
[       OK ] StringUtilsTest.DetectNonPalindrome (0 ms)
[ RUN      ] StringUtilsTest.CountVowels
[       OK ] StringUtilsTest.CountVowels (0 ms)
[ RUN      ] StringUtilsTest.CountVowelsInTextWithoutVowels
[       OK ] StringUtilsTest.CountVowelsInTextWithoutVowels (0 ms)
[----------] 7 tests from StringUtilsTest (0 ms total)

[----------] 10 tests from GradeUtilsTest
[ RUN      ] GradeUtilsTest.CalculateAverage
[       OK ] GradeUtilsTest.CalculateAverage (0 ms)
[ RUN      ] GradeUtilsTest.AverageEmptyVectorThrowsException
[       OK ] GradeUtilsTest.AverageEmptyVectorThrowsException (0 ms)
[ RUN      ] GradeUtilsTest.PassingGrade
[       OK ] GradeUtilsTest.PassingGrade (0 ms)
[ RUN      ] GradeUtilsTest.FailingGrade
[       OK ] GradeUtilsTest.FailingGrade (0 ms)
[ RUN      ] GradeUtilsTest.LetterGradeA
[       OK ] GradeUtilsTest.LetterGradeA (0 ms)
[ RUN      ] GradeUtilsTest.LetterGradeB
[       OK ] GradeUtilsTest.LetterGradeB (0 ms)
[ RUN      ] GradeUtilsTest.LetterGradeC
[       OK ] GradeUtilsTest.LetterGradeC (0 ms)
[ RUN      ] GradeUtilsTest.LetterGradeD
[       OK ] GradeUtilsTest.LetterGradeD (0 ms)
[ RUN      ] GradeUtilsTest.LetterGradeF
[       OK ] GradeUtilsTest.LetterGradeF (0 ms)
[ RUN      ] GradeUtilsTest.InvalidGradeThrowsException
[       OK ] GradeUtilsTest.InvalidGradeThrowsException (0 ms)
[----------] 10 tests from GradeUtilsTest (0 ms total)

[----------] Global test environment tear-down
[==========] 25 tests from 3 test suites ran. (1 ms total)
[  PASSED  ] 25 tests.
```

### Resultado de `ctest --output-on-failure`

```text
(PEGAR AQUÍ LA SALIDA REAL DE ctest)
```

### Cantidad de pruebas

- Pruebas ejecutadas: 25 (8 de `CalculatorTest`, 7 de `StringUtilsTest` y 10 de `GradeUtilsTest`).
- Pruebas exitosas: 25.
- Pruebas fallidas: 0.

### Error que apareció y cómo se corrigió

Al correr `cmake ..` por primera vez apareció este error:

```text
CMake Error: The source directory ".../laboratorio-testing" does not appear to contain CMakeLists.txt.
```

La causa fue que los archivos del proyecto no estaban en la carpeta `laboratorio-testing/` (quedaron en otra ubicación al extraerlos). Se corrigió moviéndolos a su lugar, se eliminó el directorio `build/` que había quedado a medias y se repitió la compilación, que esta vez funcionó. `cmake ..` busca el `CMakeLists.txt` en el directorio padre de `build/`, así que debe estar directamente en `laboratorio-testing/`.

### Explicación de pruebas implementadas

**`CalculatorTest.AddNegativeNumbers`**

```cpp
EXPECT_EQ(add(-2, -3), -5);
```

Verifica que `add` suma correctamente dos números negativos. Cubre un caso distinto al de positivos, donde un error de signo pasaría desapercibido.

**`CalculatorTest.DivideByZeroThrowsException`**

```cpp
EXPECT_THROW(divide(10, 0), std::invalid_argument);
```

Verifica que dividir entre cero lanza `std::invalid_argument` en lugar de provocar un comportamiento indefinido. Prueba el manejo de errores, no el resultado de una operación.

**`StringUtilsTest.DetectPalindromeWithSpaces`**

```cpp
EXPECT_TRUE(is_palindrome("anita lava la tina"));
```

Verifica que `is_palindrome` ignora los espacios al comparar. Sin esa normalización, la frase no se detectaría como palíndromo.

**`StringUtilsTest.CountVowels`**

```cpp
EXPECT_EQ(count_vowels("murcielago"), 5);
```

Verifica el conteo de vocales en una palabra que tiene las cinco (u, i, e, a, o), así que comprueba que ninguna se omite.

**`GradeUtilsTest.CalculateAverage`**

```cpp
std::vector<int> grades = {80, 90, 100};
EXPECT_DOUBLE_EQ(average(grades), 90.0);
```

Verifica el promedio de tres notas. Se usa `EXPECT_DOUBLE_EQ` y no `EXPECT_EQ` porque el resultado es un `double`, y comparar números de punto flotante exige tolerar errores de redondeo.

**`GradeUtilsTest.InvalidGradeThrowsException`**

```cpp
EXPECT_THROW(letter_grade(120), std::invalid_argument);
```

Verifica que una nota fuera del rango 0 a 100 se rechaza con una excepción en vez de devolver una letra sin sentido.

### Preguntas de reflexión

**1. ¿Qué significa que una prueba pase?**
Que, para la entrada y condiciones de esa prueba, el código produjo exactamente el resultado que se esperaba. No demuestra que el código esté libre de errores, solo que ese comportamiento concreto es correcto.

**2. ¿Qué significa que una prueba falle?**
Que el resultado obtenido difiere del esperado, o que se lanzó (o no) una excepción que no correspondía. Puede deberse a un error en el código o a un error en la propia prueba, así que hay que revisar ambos.

**3. ¿Qué diferencia hay entre probar una función manualmente y probarla con Google Test?**
Manualmente hay que escribir un programa o ejecutar casos a mano y comparar a ojo, y eso no queda guardado ni se puede repetir fácilmente. Con Google Test los casos quedan escritos como código, la comparación es automática y el reporte indica qué se esperaba y qué se obtuvo, de modo que se pueden ejecutar de nuevo tras cada cambio.

**4. ¿Por qué las pruebas unitarias deben ser rápidas?**
Porque se ejecutan con mucha frecuencia (en cada compilación y en cada push). Si tardaran, se dejarían de correr y se perdería el aviso temprano de los errores. Aquí las 25 pruebas tomaron 1 ms en total.

**5. ¿Por qué las pruebas unitarias deben ser deterministas?**
Porque una prueba que a veces pasa y a veces falla con el mismo código no permite saber si hay un error real. Para que un fallo signifique algo, el resultado solo debe depender del código y de la entrada, no de la hora, el orden de ejecución o valores aleatorios sin semilla.

### Qué se aprendió

Que `cmake ..` espera encontrar el `CMakeLists.txt` justo en el directorio padre de `build/`, que Google Test se descarga solo durante la configuración, y que cada `TEST` queda registrado y reportado individualmente. También que conviene elegir la aserción según el tipo: `EXPECT_EQ` para enteros, `EXPECT_DOUBLE_EQ` para `double` y `EXPECT_THROW` para excepciones.

## Parte 4: EXPECT vs ASSERT

### Qué se hizo

Se agregaron dos pruebas a `tests/test_calculator.cpp`: `MultipleExpectChecks`, que usa varias verificaciones `EXPECT_EQ` seguidas, y `AssertBeforeDivision`, que usa `ASSERT_NE` como condición previa antes de dividir. Después se hizo un experimento cambiando temporalmente `divisor` de `2` a `0` para observar la diferencia entre `EXPECT_` y `ASSERT_`, y se devolvió al valor correcto.

### Archivo modificado

- `laboratorio-testing/tests/test_calculator.cpp`

### Código agregado

```cpp
TEST(CalculatorTest, MultipleExpectChecks) {
    EXPECT_EQ(add(1, 1), 2);
    EXPECT_EQ(add(2, 2), 4);
    EXPECT_EQ(add(3, 3), 6);
}

TEST(CalculatorTest, AssertBeforeDivision) {
    int divisor = 2;

    ASSERT_NE(divisor, 0);

    EXPECT_EQ(divide(10, divisor), 5);
}
```

### Comandos usados

```bash
make
./run_tests --gtest_filter=CalculatorTest.AssertBeforeDivision
./run_tests
```

El filtro `--gtest_filter` ejecuta solo la prueba indicada, lo que permite ver su resultado aislado.

### Qué pasó cuando `divisor` era `2`

La prueba pasó. `ASSERT_NE(divisor, 0)` se cumplió, la ejecución continuó y `divide(10, 2)` devolvió `5`, igual que el valor esperado.

```text
[ RUN      ] CalculatorTest.AssertBeforeDivision
[       OK ] CalculatorTest.AssertBeforeDivision (0 ms)
[==========] 1 test from 1 test suite ran. (0 ms total)
[  PASSED  ] 1 test.
```

Al ejecutar toda la suite con las dos pruebas nuevas, se corrieron 27 pruebas y las 27 pasaron.

### Qué pasó cuando `divisor` era `0`

La prueba falló en la línea del `ASSERT_NE`:

```text
[ RUN      ] CalculatorTest.AssertBeforeDivision
.../tests/test_calculator.cpp:44: Failure
Expected: (divisor) != (0), actual: 0 vs 0
[  FAILED  ] CalculatorTest.AssertBeforeDivision (0 ms)
[==========] 1 test from 1 test suite ran. (0 ms total)
[  PASSED  ] 0 tests.
[  FAILED  ] 1 test, listed below:
[  FAILED  ] CalculatorTest.AssertBeforeDivision

 1 FAILED TEST
```

### Por qué `ASSERT_NE` detuvo la prueba

`ASSERT_NE(divisor, 0)` es una verificación fatal: cuando falla, Google Test termina de inmediato la prueba actual. Como `divisor` valía `0`, la condición `divisor != 0` no se cumplió y la ejecución no llegó a la línea siguiente, `EXPECT_EQ(divide(10, divisor), 5)`. El reporte solo muestra el fallo del `ASSERT_NE`, y no aparece ningún error relacionado con `divide`, porque `divide(10, 0)` nunca se llamó. Así, el `ASSERT_NE` actúa como guarda: si la precondición no se cumple, no tiene sentido evaluar el resto de la prueba.

### Restauración

Se devolvió `int divisor = 2;`, se recompiló y se ejecutó toda la suite:

```text
[==========] 27 tests from 3 test suites ran. (0 ms total)
[  PASSED  ] 27 tests.
```

### Diferencia entre `EXPECT_` y `ASSERT_`

| | `EXPECT_` | `ASSERT_` |
|---|---|---|
| Tipo de fallo | No fatal | Fatal |
| Si la verificación falla | Registra el fallo y la prueba continúa | Registra el fallo y la prueba termina ahí |
| Uso típico | Verificar varios resultados independientes en una misma prueba | Comprobar precondiciones sin las cuales lo siguiente no tiene sentido |

### Preguntas de reflexión

**1. ¿Cuándo conviene usar `EXPECT_`?**
Cuando las verificaciones son independientes entre sí y conviene ver todos los fallos de una vez. Por ejemplo, en `MultipleExpectChecks`, si falla `add(1, 1)` igual interesa saber si `add(2, 2)` y `add(3, 3)` también fallan. Es la opción por defecto.

**2. ¿Cuándo conviene usar `ASSERT_`?**
Cuando continuar después de un fallo no tiene sentido o es peligroso: comprobar que un puntero no es nulo antes de usarlo, que un vector tiene elementos antes de acceder a un índice, o que un divisor no es cero antes de dividir.

**3. ¿Qué podría pasar si se usa `EXPECT_` cuando en realidad se necesitaba detener la prueba?**
La prueba seguiría ejecutándose con un estado inválido. En el experimento, con `EXPECT_NE` y `divisor = 0`, se habría llamado a `divide(10, 0)`, lo que lanza una excepción y produce un segundo fallo derivado del primero. En casos peores, como desreferenciar un puntero nulo, el programa podría terminar abruptamente y ocultar el resto de los resultados.

**4. ¿Qué podría pasar si se usa `ASSERT_` en exceso?**
Cada fallo cortaría la prueba y se perdería información: solo se vería el primer problema, y los demás quedarían ocultos hasta corregirlo y volver a ejecutar. Esto alarga el ciclo de depuración.

### Qué se aprendió

Que la elección entre `EXPECT_` y `ASSERT_` depende de si las verificaciones posteriores tienen sentido tras un fallo. `ASSERT_` sirve para proteger precondiciones y `EXPECT_` para comprobar resultados. También que el filtro `--gtest_filter` permite aislar una prueba para estudiarla.