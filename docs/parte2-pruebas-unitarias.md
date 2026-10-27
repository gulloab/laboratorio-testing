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

## Parte 3: compilación y ejecución

### Comandos

```bash
cd laboratorio-testing
mkdir build
cd build
cmake ..
make
./run_tests
ctest --output-on-failure
```

### Resultado de `./run_tests`

```text
(pegar aquí la salida real)
```

### Resultado de `ctest --output-on-failure`

```text
(pegar aquí la salida real)
```

- Pruebas ejecutadas: _(completar)_
- Pruebas exitosas: _(completar)_

### Explicación de al menos cinco pruebas

_(Se redacta una vez que corra todo.)_

## Parte 4: EXPECT vs ASSERT

_(Se completa al hacer la Parte 4.)_
