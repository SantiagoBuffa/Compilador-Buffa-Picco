# Compilador TDS (`c-tds`)

Compilador para el lenguaje **TDS** (Teoría de Sintaxis y Semántica / Compiladores) desarrollado en C, Flex y Bison. Genera código intermedio de tres direcciones (TAC/3AC) y código ensamblador nativo x86-64 compatible con GCC.

---

## 🛠️ Compilación del Proyecto

Para compilar el proyecto y generar el binario ejecutable `c-tds`:

```bash
make
```

Esto generará el ejecutable principal `c-tds` junto con enlaces simbólicos `compilador_test` y `ctds` para mantener compatibilidad con scripts existentes.

Para limpiar los archivos generados y compilados:

```bash
make clean
```

---

## 🚀 Uso de la Línea de Comandos (CLI)

```bash
./c-tds [opciones] <archivo-fuente>
```

### Tabla de Opciones

| Opción | Descripción |
| :--- | :--- |
| `-o <salida>` | Escribe la salida en un archivo con nombre `<salida>`. Si se omite, se infiere del nombre del archivo fuente (por ejemplo: `fuente.ci`, `fuente.s`, `fuente.out`). |
| `-target <etapa>` | Limita la compilación hasta la etapa indicada: <br>• `scan`: Únicamente análisis léxico. <br>• `parse`: Análisis léxico, sintáctico y chequeo semántico/tipos. <br>• `codinter`: Hasta la generación de código intermedio (TAC). <br>• `assembly`: Hasta la generación de código ensamblador nativo (sin invocar gcc). |
| `-opt [opts...]` | Habilita optimizaciones en el AST mediante una lista de nombres **separados exclusivamente por espacios**. Si se pasa `-opt` sin opciones o con `all`, se activan todas. Si se omite `-opt`, no se ejecuta ninguna optimización. |
| `-debug` | Imprime información detallada de depuración por `stdout` (AST original, AST optimizado, código TAC, tablas de símbolos y avisos de generación). Si no se pasa, la compilación exitosa es completamente silenciosa en `stdout`. |
| `-c`, `--module` | Modo módulo: no requiere la presencia obligatoria de la función `main` y omite el enlace del ejecutable final. |
| `-h`, `--help` | Muestra el menú de ayuda con el resumen de todas las opciones. |

---

## ⚡ Optimizaciones Disponibles (`-opt`)

Las optimizaciones se seleccionan separadas por espacios:

- **`fold`** (o **`const`**): Plegado de constantes en expresiones aritméticas/lógicas y propagación de constantes intra-bloque y a través de estructuras de control (`meet` condicional).
- **`sc`** (o **`shortcircuit`**): Cortocircuito booleano (`true || expr -> true`, `false && expr -> false`, identidades con neutros).
- **`dce`** (o **`deadcode`**): Eliminación de código inalcanzable tras retornos (`return`), poda de ramas muertas (`if (true)`, `if (false)`) y eliminación de bucles muertos (`while (false)`).
- **`alg`** (o **`algebraic`**): Simplificaciones algebraicas y reducción de fuerza (`x * 0 -> 0`, `x * 1 -> x`, `x * 2 -> x + x`, `(x * C) / C -> x`, `(x + x) / 2 -> x`, etc.).
- **`all`**: Habilita todas las optimizaciones anteriores.

---

## 💡 Ejemplos de Invocación

### 1. Compilación Estándar Completa (a binario ejecutable)
```bash
./c-tds programa.ctds
./programa.out
```

### 2. Especificar Nombre de Salida (`-o`)
```bash
./c-tds -o mi_programa programa.ctds
./mi_programa
```

### 3. Detenerse en Etapas Específicas (`-target`)
- **Solo scanning:**
  ```bash
  ./c-tds -target scan programa.ctds
  ```
- **Scanning, parsing y chequeo de tipos:**
  ```bash
  ./c-tds -target parse programa.ctds
  ```
- **Generar solo código intermedio (`.ci`):**
  ```bash
  ./c-tds -target codinter -o programa.ci programa.ctds
  ```
- **Generar solo código ensamblador x86-64 (`.s`):**
  ```bash
  ./c-tds -target assembly -o programa.s programa.ctds
  ```

### 4. Control de Optimizaciones (`-opt`)
- **Sin optimizaciones (por defecto si no se incluye `-opt`):**
  ```bash
  ./c-tds programa.ctds
  ```
- **Todas las optimizaciones habilitadas:**
  ```bash
  ./c-tds -opt programa.ctds
  # O explícitamente:
  ./c-tds -opt all programa.ctds
  ```
- **Optimizaciones específicas (separadas por espacio):**
  ```bash
  ./c-tds -opt fold dce programa.ctds
  ./c-tds -opt sc alg programa.ctds
  ```

### 5. Modo Depuración (`-debug`)
```bash
./c-tds -debug -opt all programa.ctds
```

### 6. Compilación de Módulos (sin función `main`)
```bash
./c-tds -c modulo.ctds
```

---

## 🧪 Ejecución de Tests

El proyecto cuenta con suites de pruebas automatizadas:

```bash
# Tests de front-end, análisis semántico y código intermedio (TAC):
make test

# Tests de generación y ejecución de código ensamblador nativo (x86-64):
make test-asm

# Tests específicos de la interfaz de línea de comandos (CLI):
make test-cli
```
