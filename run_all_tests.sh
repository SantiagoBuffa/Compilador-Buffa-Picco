#!/bin/bash
cd "$(dirname "$0")"

echo "==============================="
echo "       COMPILING COMPILER      "
echo "==============================="
flex scanner.lex && bison -d parser.y && gcc lex.yy.c parser.tab.c -o compilador_test
if [ $? -ne 0 ]; then
    echo "Error de compilación"
    exit 1
fi

pass=0
fail=0

echo -e "\n==============================="
echo "       TESTS VÁLIDOS           "
echo "==============================="
for file in Tests/valid/*; do
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

echo -e "\n==============================="
echo "       TESTS INVÁLIDOS         "
echo "==============================="
for file in Tests/invalid/*; do
    if [ ! -f "$file" ]; then continue; fi
    out=$(./compilador_test "$file" 2>&1)
    if echo "$out" | grep -E -q "Error en la línea|Lexic error"; then
        echo -e "[\e[32mPASS\e[0m] $file"
        pass=$((pass+1))
    else
        echo -e "[\e[31mFAIL\e[0m] $file"
        echo -e "       \e[33mDetalle: El compilador aceptó el código inválido o no reportó error.\e[0m"
        fail=$((fail+1))
    fi
done

echo -e "\nResumen Total: $pass PASSED, $fail FAILED\n"
exit $fail
