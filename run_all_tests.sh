#!/bin/bash

# Este script corre todos los tests usando los scripts individuales.

# Ir a la raíz del proyecto
cd "$(dirname "$0")"

bash Tests/scanner/run_tests.sh
scanner_status=$?

bash Tests/parser/run_tests.sh
parser_status=$?

echo "==============================="
echo "       RESUMEN FINAL           "
echo "==============================="

if [ $scanner_status -eq 0 ] && [ $parser_status -eq 0 ]; then
    echo -e "[\e[32mÉXITO\e[0m] Todos los componentes pasaron exitosamente sus pruebas."
else
    echo -e "[\e[31mFALLO\e[0m] Algunos tests fallaron. Revisa los logs arriba."
fi

exit $((scanner_status + parser_status))
