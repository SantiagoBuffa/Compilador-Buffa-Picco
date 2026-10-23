#!/bin/bash
cd "$(dirname "$0")"

echo "==============================="
echo "       COMPILING COMPILER      "
echo "==============================="
flex scanner.lex && bison -d parser.y && gcc lex.yy.c parser.tab.c estructuras.c symtab.c semantic.c tac.c codegen.c asm.c -o compilador_test -lm
if [ $? -ne 0 ]; then
    echo "Error de compilación"
    exit 1
fi

pass=0
fail=0

echo -e "\n==============================="
echo "       TESTS VÁLIDOS           "
echo "==============================="
for file in Tests/valid/*.ctds; do
    if [ ! -f "$file" ]; then continue; fi
    echo -e "\n--- Analizando: $file ---"
    out=$(./compilador_test "$file" 2>&1)
    echo "$out"
    if echo "$out" | grep -q "successful parsing" && ! echo "$out" | grep -q "Semantic Error"; then
        base_name=$(basename "$file" .ctds)
        expected_ci="Tests/expected/${base_name}.ci"
        generated_ci="Tests/valid/${base_name}.ci"

        if [ -f "$expected_ci" ]; then
            diff_output=$(diff -u -w -B "$generated_ci" "$expected_ci" 2>&1)
            if [ $? -eq 0 ]; then
                echo -e "[\e[32mPASS\e[0m] $file (TAC verificado con golden file)"
                pass=$((pass+1))
            else
                echo -e "[\e[31mFAIL\e[0m] $file (TAC generado difiere de $expected_ci)"
                echo -e "       \e[33mDiff:\n$diff_output\e[0m"
                fail=$((fail+1))
            fi
        else
            echo -e "[\e[32mPASS\e[0m] $file (Sin golden file)"
            pass=$((pass+1))
        fi
    else
        echo -e "[\e[31mFAIL\e[0m] $file"
        echo -e "       \e[33mDetalle: $(echo "$out" | grep -E 'Error|Lexic error|Semantic Error' | head -n 1)\e[0m"
        fail=$((fail+1))
    fi
done

echo -e "\n==============================="
echo "       TESTS INVÁLIDOS         "
echo "==============================="
for file in Tests/invalid/*.ctds; do
    if [ ! -f "$file" ]; then continue; fi
    echo -e "\n--- Analizando: $file ---"
    out=$(./compilador_test "$file" 2>&1)
    echo "$out"
    if echo "$out" | grep -E -q "Error on line|Lexic error|Semantic Error"; then
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
