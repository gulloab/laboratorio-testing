# Reflexión final y problemas encontrados

## Mini reto de testing

_(Se completa en la Parte 12.)_

## Reflexión final

**1. ¿Qué es software testing?**
Es el conjunto de actividades para comprobar que un software hace lo que debe y para encontrar defectos antes de que lleguen al usuario. Incluye diseñar casos de prueba a partir de los requisitos, ejecutarlos y comparar el resultado obtenido con el esperado. En este laboratorio se hizo con pruebas automatizadas escritas con Google Test.

**2. ¿Por qué el testing mejora la calidad del software?**
Porque detecta errores pronto, cuando son más fáciles de localizar y corregir, y porque protege el comportamiento que ya funciona: al cambiar el código, las pruebas avisan si algo se rompió. Un ejemplo del laboratorio fue la Parte 5, donde el cambio de un solo carácter en `is_even` (`== 0` por `== 1`) hizo fallar dos pruebas de inmediato. Las pruebas también documentan el comportamiento esperado de cada función.

**3. ¿Cuál es la diferencia entre verificación y validación?**
La verificación responde a "¿estamos construyendo el producto correctamente?": comprueba que el software cumple su especificación. La validación responde a "¿estamos construyendo el producto correcto?": comprueba que satisface la necesidad real de quien lo usa. En el laboratorio, comprobar que `letter_grade(89)` devuelve `'B'` verifica el código contra la regla dada. Revisar que esa regla de conversión sea la que el usuario necesita sería validación.

**4. ¿Qué es una prueba unitaria?**
Una prueba que verifica una unidad pequeña del código (una función o una clase) de forma aislada, con una entrada concreta y un resultado esperado. Es rápida y localiza el error con precisión. Todas las pruebas de `calculator`, `string_utils` y `grade_utils` de este laboratorio son unitarias.

**5. ¿Qué es una prueba funcional?**
Una prueba que verifica que el sistema cumple un requisito, observándolo desde fuera: se da una entrada y se comprueba la salida esperada, sin depender de la implementación interna. En la Parte 9 se diseñaron 18 casos a partir del requisito de conversión de notas, con el mismo tipo de herramienta, pero partiendo del comportamiento esperado y no del código.

**6. ¿Qué diferencia hay entre `EXPECT_` y `ASSERT_`?**
`EXPECT_` registra el fallo y deja que la prueba continúe, así se ven todos los errores de una vez. `ASSERT_` registra el fallo y termina la prueba en ese punto. En el experimento de la Parte 4, con `divisor` en 0, el `ASSERT_NE` detuvo la prueba antes de llamar a `divide(10, 0)`. `ASSERT_` conviene para precondiciones sin las cuales lo siguiente no tiene sentido, y `EXPECT_` para el resto de las verificaciones.

**7. ¿Por qué las pruebas deben ser deterministas?**
Porque una prueba que da resultados distintos con el mismo código no permite saber si el fallo es real o casual, y un fallo que no se puede repetir no se puede diagnosticar. Si el resultado solo depende del código y de la entrada, un fallo siempre significa algo y siempre se puede reproducir.

**8. ¿Por qué puede ser útil una semilla en pruebas con valores aleatorios?**
Porque convierte lo aleatorio en reproducible: con la misma semilla el generador produce la misma secuencia. En la Parte 7, `mt19937(12345)` generó los mismos diez pares en dos ejecuciones, y al cambiar a `54321` produjo otros. Se conserva la variedad de datos sin perder la repetibilidad, y si una prueba fallara bastaría con conocer la semilla para reproducirla.

**9. ¿Qué es cobertura de código?**
Una métrica que indica qué parte del código se ejecutó mientras corrían las pruebas, medida en líneas, funciones o ramas. Sirve para encontrar código que ninguna prueba ejercita. En este laboratorio el código de `src/` quedó con 100% de líneas, 100% de funciones y 87.5% de ramas.

**10. ¿Por qué una cobertura alta no garantiza ausencia de errores?**
Porque mide qué se ejecutó, no si el resultado se verificó correctamente ni si se probaron las entradas importantes. Un caso real de este laboratorio: con 100% de cobertura de líneas, ninguna prueba comprobaba que la nota `101` se rechazara; ese hueco apareció al diseñar los casos funcionales de la Parte 9. Una prueba sin aserciones, o con aserciones débiles, también aporta cobertura sin detectar errores.

**11. ¿Qué ventaja tiene ejecutar pruebas en GitHub Actions?**
Que las pruebas se ejecutan solas, en cada push o pull request, en un entorno limpio y compartido, sin depender de que alguien se acuerde de correrlas ni de la configuración de su máquina. El resultado queda visible para todo el equipo y con historial. En la Parte 11 se vio cómo un fallo intencional puso el workflow en rojo y cómo la corrección lo devolvió a verde.

**12. ¿Qué parte del laboratorio le pareció más útil?**
Provocar fallos a propósito (Partes 5 y 11). Ver una prueba fallar, leer el mensaje con el valor obtenido y el esperado, y comprobar que el mismo fallo aparece en GitHub Actions, mostró de forma concreta para qué sirven las pruebas y la integración continua. También resultó muy útil la Parte 9, porque diseñar los casos desde el requisito hizo aparecer el caso 101, que las pruebas anteriores no cubrían.

**13. ¿Qué parte le pareció más difícil?**
La cobertura de código (Parte 8). `lcov` 2.0 se detuvo con un error de inconsistencia al usarse con `gcov` 15, y hubo que entender el mensaje y la opción `--ignore-errors` para continuar. Además, interpretar los resultados exigió distinguir entre cobertura de líneas y de ramas, y entender que las 6 ramas sin cubrir eran rutas de excepción generadas por el compilador. También costó, en varios momentos, tener claro desde qué carpeta ejecutar cada comando.

**14. ¿Cómo aplicaría pruebas automatizadas en un proyecto futuro?**
Desde el inicio del proyecto y no al final: configurar CMake con Google Test y el workflow de integración continua desde la primera versión, de modo que cada cambio se pruebe automáticamente. Escribiría pruebas unitarias con casos normales, borde e inválidos para cada módulo, y pruebas funcionales a partir de los requisitos. En un proyecto como el motor de telemetría multihilo que lee archivos JSON, empezaría por probar el parser con entradas válidas, vacías y mal formadas, y la lógica de cálculo de forma aislada, porque son las partes más fáciles de volver deterministas. Mediría la cobertura para encontrar código sin probar, sabiendo que no es una garantía de calidad.

## Problemas encontrados y cómo se resolvieron

| # | Parte | Problema | Causa | Solución | Lección |
|---|---|---|---|---|---|
| 1 | Estructura | El comando `touch` con llaves `{a,b}` no creó los archivos | La expansión de llaves no funciona en `sh`, o falló al pegar las líneas con `\` | Un `touch` por archivo | Probar los comandos en el mismo intérprete en que se usan |
| 2 | 3 | `cmake ..` falló: no se encontró `CMakeLists.txt` | Los archivos no estaban directamente en `laboratorio-testing/` | Mover los archivos a su lugar y rehacer `build/` | `cmake ..` busca el `CMakeLists.txt` en el directorio padre de `build/` |
| 3 | 4, 5 y 9 | Comandos `sed`, `cat >>` y `cd build` sin efecto | Se ejecutaron desde `build/` y no desde `laboratorio-testing/`; así, `divisor` quedó en 0 y fallaba una prueba ajena a la Parte 5 | Empezar cada bloque con `cd` a la ruta completa y verificar con `grep` | Comprobar siempre el directorio actual antes de editar archivos |
| 4 | 6 | Solo se veían 30 pruebas y ninguna de las nuevas | Las pruebas de `string_utils` se pegaron en `test_grade_utils.cpp`, que no incluye `string_utils.h`, y la compilación falló; `ctest` ejecutó el binario viejo | Moverlas a `test_string_utils.cpp` y recompilar | Confirmar que `make` termina sin errores y contrastar la cantidad de pruebas con la esperada |
| 5 | 6 | Después de la corrección, salían 37 pruebas y no 40 | `make` no había recompilado `test_calculator.cpp` | Forzar la recompilación con `touch` | Contar las pruebas por suite ayuda a ubicar qué falta |
| 6 | 7 | Seguían apareciendo líneas `a=... b=...` y la semilla seguía en 54321 | No se ejecutó el paso de restauración tras el experimento | Restaurar la semilla y quitar el `std::cout` | Los cambios temporales deben revertirse y verificarse con `grep` |
| 7 | 8 | `lcov --capture` falló con `(inconsistent) mismatched end line` y no se generó `coverage.info` | Incompatibilidad entre `lcov` 2.0 y `gcov` 15.2.0 con las funciones que genera la macro `TEST` | Usar `--ignore-errors inconsistent,mismatch,unused` | Leer el mensaje de error completo: indicaba la opción para resolverlo |
| 8 | 8 | La cobertura inicial parecía muy baja (37.9%) | La captura incluía GoogleTest y la biblioteca estándar | Filtrar con `lcov --remove` para medir solo `src/` | Un número de cobertura debe interpretarse según lo que mide |
| 9 | 9 | La prueba con 101 no existía, aunque la cobertura de líneas era 100% | Solo se probaba el valor 120 para el límite superior | Agregar `GradeAboveMaximumThrowsException` | La cobertura alta no sustituye el diseño de casos borde |
| 10 | 9 | `libgcov profiling error: overwriting an existing profile data` | Quedó un archivo `.gcda` de una compilación anterior con otra suma de verificación | Ignorarlo, o borrar los `.gcda` | Es un aviso de la compilación con cobertura, sin efecto en los resultados |
| 11 | 10 | El workflow podía quedar fuera del repositorio | Se asumió como raíz la carpeta de arriba; la raíz real del repo es `laboratorio-testing/` | Ubicar `.github/` en esa raíz y quitar los `cd laboratorio-testing` del workflow | La ubicación del workflow y las rutas dependen de dónde está la raíz del repositorio |
| 12 | 10 | Las ejecuciones #1 a #4 de GitHub Actions fallaron | El archivo `testing.yml` se había subido vacío en esos commits | Agregar el contenido del workflow (ejecución #5, exitosa) | Un workflow vacío no es válido y aparece como fallo |

### Fallas que se provocaron a propósito

Además de los problemas anteriores, se provocaron dos fallos intencionales, y ambos se documentaron con su corrección: el cambio de `is_even` (`== 0` por `== 1`), que hizo fallar 2 pruebas (Parte 5), y el cambio del valor esperado `5` por `999` en `AddPositiveNumbers`, que puso en rojo el workflow de GitHub Actions (Parte 11).