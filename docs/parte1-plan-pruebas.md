# Parte 1: preparación del proyecto y plan de pruebas

## Qué se hizo

Se creó la carpeta `laboratorio-testing` dentro del repositorio del curso, con la estructura `include/`, `src/`, `tests/` y `docs/`, y se agregó el archivo `CMakeLists.txt` que configura la compilación y descarga Google Test. El workflow de GitHub Actions se ubicó en `.github/workflows/testing.yml`, en la raíz del repositorio, porque GitHub solo detecta workflows en esa ruta.

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
| `laboratorio-testing/CMakeLists.txt` | Configuración de compilación, Google Test y cobertura |
| `laboratorio-testing/include/` | Encabezados (`calculator.h`, `string_utils.h`, `grade_utils.h`) |
| `laboratorio-testing/src/` | Implementaciones (`.cpp`) |
| `laboratorio-testing/tests/` | Pruebas unitarias con Google Test |
| `laboratorio-testing/docs/` | Documentación en Markdown |
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

_(Se completa en la Parte 9.)_
