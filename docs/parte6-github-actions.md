# Partes 10 y 11: integración continua con GitHub Actions

## Qué es integración continua

La integración continua (CI, por *continuous integration*) es una práctica en la que cada cambio que se sube al repositorio se compila y se prueba automáticamente en un entorno limpio y compartido, en lugar de depender de que cada persona ejecute las pruebas en su máquina. Así los errores se detectan al integrar el cambio y no días después, cuando ya se mezclaron con otros.

GitHub Actions es el servicio de GitHub que permite hacerlo: se describe un *workflow* en un archivo YAML dentro del repositorio, y GitHub lo ejecuta en una máquina virtual cada vez que ocurre un evento definido.

## Qué hace `testing.yml`

El archivo `.github/workflows/testing.yml` define un workflow llamado **C++ testing** con un único trabajo (`build-and-test`) que se ejecuta en una máquina virtual `ubuntu-latest`. Ese trabajo reproduce automáticamente lo que se hace a mano en el laboratorio: instala las herramientas, configura el proyecto con CMake, lo compila y ejecuta las pruebas. Si alguno de esos pasos falla, el workflow se marca como fallido.

Como la raíz del repositorio es directamente `laboratorio-testing`, el workflow no necesita ningún `cd laboratorio-testing` (es el caso que contempla la nota del enunciado), y `.github/` está en esa misma raíz, que es la única ubicación donde GitHub detecta workflows.

```yaml
name: C++ testing

on:
  push:
    branches: [ "main" ]
  pull_request:
    branches: [ "main" ]

jobs:
  build-and-test:
    runs-on: ubuntu-latest

    steps:
      - name: Checkout repository
        uses: actions/checkout@v4

      - name: Install dependencies
        run: |
          sudo apt update
          sudo apt install -y build-essential cmake lcov

      - name: Configure CMake
        run: |
          mkdir -p build
          cd build
          cmake ..

      - name: Build
        run: |
          cd build
          make

      - name: Run tests
        run: |
          cd build
          ./run_tests
```

## Eventos en los que se ejecuta

| Evento | Cuándo se activa |
|---|---|
| `push` a `main` | Cada vez que se suben commits a la rama `main` |
| `pull_request` hacia `main` | Cuando se abre o se actualiza un pull request cuya rama destino es `main` |

Una subida a cualquier otra rama no activa el workflow, a menos que se abra un pull request hacia `main`.

## Pasos que ejecuta

| # | Paso | Qué hace |
|---|---|---|
| 1 | Checkout repository | Descarga el código del repositorio a la máquina virtual (`actions/checkout@v4`) |
| 2 | Install dependencies | Actualiza la lista de paquetes e instala compilador, CMake y `lcov` |
| 3 | Configure CMake | Crea `build/` y ejecuta `cmake ..`, que además descarga GoogleTest |
| 4 | Build | Compila la biblioteca y el ejecutable de pruebas con `make` |
| 5 | Run tests | Ejecuta `./run_tests`, que corre todas las pruebas de Google Test |

La máquina virtual parte limpia en cada ejecución, así que todo se instala y se compila desde cero. Esto asegura que el proyecto no dependa de nada que solo exista en una computadora local. `lcov` se instala pero este workflow no lo usa: se dejó porque el enunciado lo incluye, y serviría para generar cobertura en CI.

## Evidencia del workflow exitoso

La ejecución **#5**, correspondiente al commit "Agrega workflow de GitHub Actions para ejecutar las pruebas" (`dfb20f8`), en la rama `main`, terminó con éxito (check verde) en 41 segundos.

![Lista de ejecuciones del workflow C++ testing](img/actions-exitoso.png)

_(Agregar también una captura de la ejecución #5 con los cinco pasos desplegados, en `docs/img/actions-pasos.png`.)_

![Pasos de la ejecución exitosa](img/actions-pasos.png)

## Ejecuciones anteriores fallidas

La lista muestra que las ejecuciones #1 a #4 fallaron y la #5 pasó:

| Ejecución | Commit | Resultado |
|---|---|---|
| #1 | Creacion del la estructura del repo. (`1b22652`) | Fallo |
| #2 | Nuevos archivos (`baa9eca`) | Fallo |
| #3 | creacion de los md y testeo. (`af2dd8f`) | Fallo |
| #4 | tabla final y prueba. (`0ff1d1d`) | Fallo |
| #5 | Agrega workflow de GitHub Actions para ejecutar las pruebas (`dfb20f8`) | **Éxito** (41 s) |

Esas cuatro ejecuciones corresponden a commits en los que `testing.yml` se había creado como un archivo vacío, junto con el resto de la estructura, y todavía no tenía el contenido del workflow. Un archivo de workflow vacío no define ningún trabajo, así que GitHub lo marca como fallido sin llegar a ejecutar pasos. El workflow con su contenido se agregó recién en el commit de la ejecución #5, y desde ahí pasó.

## Qué pasaría si una prueba falla en GitHub Actions

Si una prueba fallara, `./run_tests` terminaría con un código de salida distinto de cero (Google Test devuelve 1 cuando alguna prueba falla). GitHub interpreta cualquier paso que termine con código distinto de cero como fallido, así que:

- El paso **Run tests** se marcaría en rojo y el trabajo completo, y con él el workflow, se marcaría como fallido (una X roja en la pestaña Actions y junto al commit).
- El log del paso mostraría la misma salida que se ve en local: la prueba que falló, la línea, el valor obtenido y el esperado.
- GitHub notificaría el fallo por correo a quien hizo el push.
- En un pull request, el resultado fallido aparecería como una verificación en rojo, y si el repositorio tiene reglas de protección de rama, impediría integrar el cambio hasta corregirlo.

Un fallo en un paso anterior (por ejemplo, un error de compilación en *Build*) tiene el mismo efecto, y los pasos siguientes ya no se ejecutan.

## Preguntas de reflexión

**1. ¿Por qué es útil ejecutar pruebas automáticamente en GitHub?**
Porque no depende de que alguien se acuerde de ejecutarlas, y se corren siempre en el mismo entorno limpio. Evita el clásico "en mi máquina funciona", y deja un registro visible para todo el equipo de si cada cambio pasó o no las pruebas.

**2. ¿Qué problema resuelve la integración continua?**
El de integrar tarde y descubrir los errores acumulados. Con CI, cada cambio se compila y se prueba apenas se sube, así que un error se detecta cuando todavía es pequeño, está asociado a un commit concreto y es más fácil de localizar y corregir.

**3. ¿Por qué conviene ejecutar pruebas antes de integrar cambios?**
Porque una vez que un cambio defectuoso entra a la rama principal, afecta a todos los que trabajan sobre ella. Ejecutar las pruebas antes, por ejemplo en un pull request, permite rechazar o corregir el cambio sin contaminar la rama principal.

**4. ¿Qué información proporciona GitHub Actions cuando un workflow falla?**
El estado de cada paso (cuál falló y cuáles no se ejecutaron), el log completo con la salida de cada comando, incluido el mensaje de Google Test con lo esperado y lo obtenido, el commit y la persona que lo originó, la duración, y una notificación por correo. Con esto se puede diagnosticar el fallo sin reproducirlo localmente.

**5. ¿Cómo ayuda CI a mejorar la colaboración en equipo?**
Establece un criterio común y objetivo de qué significa que un cambio esté bien: pasa las pruebas. Quien revisa un pull request ve de inmediato si el cambio compila y pasa las pruebas, la discusión se centra en el diseño y no en si funciona, y todos comparten la confianza de que la rama principal está sana.

## Qué se aprendió

Que el workflow es parte del repositorio y que su ubicación importa: tiene que estar en `.github/workflows/` en la raíz del repositorio, y las rutas de los pasos dependen de dónde está esa raíz. También que un archivo de workflow vacío produce ejecuciones fallidas, y que la lista de ejecuciones sirve como historial de cuándo empezó a funcionar la integración continua.

## Fallo intencional en CI (Parte 11)

_(Se completa en la Parte 11.)_