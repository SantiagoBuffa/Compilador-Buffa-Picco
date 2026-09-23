#!/bin/bash

# Ir a la raíz del proyecto
cd "$(dirname "$0")/../.."

echo "==============================="
echo "       PARSER TESTS            "
echo "==============================="

# Compilar
flex scanner.lex && bison -d parser.y && gcc lex.yy.c parser.tab.c -o compilador_test
if [ $? -ne 0 ]; then
    echo "Error de compilación"
    exit 1
fi

pass=0
fail=0

echo -e "\n--- Tests Válidos ---"
for file in Tests/parser/valid/*; do
    if [ ! -f "$file" ]; then continue; fi
    out=$(./compilador_test "$file" 2>&1)
    if echo "$out" | grep -q "Parseo exitoso"; then
        echo -e "[\e[32mPASS\e[0m] $file"
        pass=$((pass+1))
    else
        echo -e "[\e[31mFAIL\e[0m] $file"
        echo -e "       \e[33mDetalle: $(echo "$out" | grep -E 'Error|Lexic error' | head -n 1)\e[0m"
        fail=$((fail+1))
    fi
done

echo -e "\n--- Tests Inválidos ---"
for file in Tests/parser/invalid/*; do
    if [ ! -f "$file" ]; then continue; fi
    out=$(./compilador_test "$file" 2>&1)
    if echo "$out" | grep -E -q "Error en la línea|Lexic error"; then
        echo -e "[\e[32mPASS\e[0m] $file"
        pass=$((pass+1))
    else
        echo -e "[\e[31mFAIL\e[0m] $file"
        echo -e "       \e[33mDetalle: El parser reportó 'Parseo exitoso' en un archivo inválido.\e[0m"
        fail=$((fail+1))
    fi
done

echo -e "\nParser Total: $pass PASSED, $fail FAILED\n"
exit $fail
