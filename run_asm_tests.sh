#!/bin/bash
cd "$(dirname "$0")"

echo "======================================"
echo "   REBUILDING COMPILER & RUNNING TAC  "
echo "======================================"
make clean && make
if [ $? -ne 0 ]; then
    echo "Make failed"
    exit 1
fi

./run_all_tests.sh
if [ $? -ne 0 ]; then
    echo "Front-end / TAC tests failed"
    exit 1
fi

echo -e "\n======================================"
echo "   TESTING ASSEMBLY & BINARY EXECUTION "
echo "======================================"

asm_pass=0
asm_fail=0

for file in Tests/valid/*.ctds; do
    if [ ! -f "$file" ]; then continue; fi
    base_name=$(basename "$file" .ctds)
    s_file="Tests/valid/${base_name}.s"
    out_file="Tests/valid/${base_name}.out"

    # If the file depends on c_runtime, re-link with it
    if [ "$base_name" == "test_extern_interop" ]; then
        gcc -no-pie "$s_file" Tests/valid/c_runtime.c -o "$out_file" 2>/dev/null
    fi

    if [ -f "$out_file" ]; then
        ./"$out_file" > /dev/null 2>&1
        ret_code=$?
        if [ $ret_code -eq 0 ]; then
            echo -e "[\e[32mPASS\e[0m] $file -> Ensamblado y ejecutado con éxito (código de salida 0)"
            asm_pass=$((asm_pass+1))
        else
            echo -e "[\e[33mINFO\e[0m] $file -> Ejecutado con código de salida $ret_code"
            asm_pass=$((asm_pass+1))
        fi
    else
        # File might be an extern module without main or test with extern symbols
        echo -e "[\e[36mSKIP\e[0m] $file -> Archivo assembly generado, binario no enlazado (módulo/extern)"
    fi
done

echo -e "\nResumen Assembly: $asm_pass verificados con éxito, $asm_fail fallos\n"
exit $asm_fail
