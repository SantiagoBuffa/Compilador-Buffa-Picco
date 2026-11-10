#!/bin/bash
# scripts/test_cli.sh - Test suite for c-tds CLI flags and options

cd "$(dirname "$0")/.."

echo "======================================"
echo "        TESTING c-tds CLI            "
echo "======================================"

cli_pass=0
cli_fail=0

assert_success() {
    local desc="$1"
    shift
    "$@" > /dev/null 2>&1
    if [ $? -eq 0 ]; then
        echo -e "[\e[32mPASS\e[0m] $desc"
        cli_pass=$((cli_pass+1))
    else
        echo -e "[\e[31mFAIL\e[0m] $desc (expected exit code 0)"
        cli_fail=$((cli_fail+1))
    fi
}

assert_failure() {
    local desc="$1"
    shift
    "$@" > /dev/null 2>&1
    if [ $? -ne 0 ]; then
        echo -e "[\e[32mPASS\e[0m] $desc"
        cli_pass=$((cli_pass+1))
    else
        echo -e "[\e[31mFAIL\e[0m] $desc (expected non-zero exit code)"
        cli_fail=$((cli_fail+1))
    fi
}

assert_silent_stdout() {
    local desc="$1"
    shift
    local out
    out=$("$@" 2> /dev/null)
    if [ -z "$out" ]; then
        echo -e "[\e[32mPASS\e[0m] $desc (stdout is silent)"
        cli_pass=$((cli_pass+1))
    else
        echo -e "[\e[31mFAIL\e[0m] $desc (expected silent stdout, got:\n$out)"
        cli_fail=$((cli_fail+1))
    fi
}

assert_output_contains() {
    local desc="$1"
    local pattern="$2"
    shift 2
    local out
    out=$("$@" 2>&1)
    if echo "$out" | grep -q "$pattern"; then
        echo -e "[\e[32mPASS\e[0m] $desc"
        cli_pass=$((cli_pass+1))
    else
        echo -e "[\e[31mFAIL\e[0m] $desc (pattern '$pattern' not found in output)"
        cli_fail=$((cli_fail+1))
    fi
}

# 1. Basic flags and error handling
assert_success "Help flag (-h)" ./c-tds -h
assert_output_contains "Help flag usage info" "Usage:" ./c-tds --help
assert_failure "No arguments fails" ./c-tds
assert_failure "Non-existent file fails" ./c-tds non_existent_file.ctds
assert_failure "Unknown flag fails" ./c-tds -invalidflag Tests/valid/test_ret_simple.ctds
assert_failure "Missing argument for -o fails" ./c-tds -o
assert_failure "Missing argument for -target fails" ./c-tds -target
assert_failure "Invalid target stage fails" ./c-tds -target invalid_stage Tests/valid/test_ret_simple.ctds

# 2. Quiet default execution (silent stdout on success without -debug)
assert_silent_stdout "Default compilation is silent on stdout" ./c-tds Tests/valid/test_ret_simple.ctds

# 3. -debug flag prints debug information
assert_output_contains "-debug prints AST (ORIGINAL)" "=== AST (ORIGINAL) ===" ./c-tds -debug Tests/valid/test_ret_simple.ctds
assert_output_contains "-debug prints Intermediate Code (TAC)" "=== INTERMEDIATE CODE (TAC) ===" ./c-tds -debug Tests/valid/test_ret_simple.ctds

# 4. Target stages
rm -f /tmp/cli_test_*

# Target: scan
assert_success "Target scan" ./c-tds -target scan Tests/valid/test_ret_simple.ctds
assert_failure "Target scan fails on lexical error" ./c-tds -target scan Tests/invalid/test_invalid_char.ctds

# Target: parse
assert_success "Target parse" ./c-tds -target parse Tests/valid/test_ret_simple.ctds
assert_failure "Target parse fails on syntax error" ./c-tds -target parse Tests/invalid/test_syntax_error.ctds
assert_failure "Target parse fails on semantic error" ./c-tds -target parse Tests/invalid/test_semantic_undeclared.ctds

# Target: codinter
assert_success "Target codinter default output" ./c-tds -target codinter Tests/valid/test_ret_simple.ctds
assert_success "Target codinter custom -o" ./c-tds -target codinter -o /tmp/cli_test_out.ci Tests/valid/test_ret_simple.ctds
if [ -f /tmp/cli_test_out.ci ] && grep -q "func begin" /tmp/cli_test_out.ci; then
    echo -e "[\e[32mPASS\e[0m] Target codinter correctly wrote /tmp/cli_test_out.ci"
    cli_pass=$((cli_pass+1))
else
    echo -e "[\e[31mFAIL\e[0m] Target codinter did not write /tmp/cli_test_out.ci correctly"
    cli_fail=$((cli_fail+1))
fi

# Target: assembly
assert_success "Target assembly default output" ./c-tds -target assembly Tests/valid/test_ret_simple.ctds
assert_success "Target assembly custom -o" ./c-tds -target assembly -o /tmp/cli_test_out.s Tests/valid/test_ret_simple.ctds
if [ -f /tmp/cli_test_out.s ] && grep -q "\.globl" /tmp/cli_test_out.s; then
    echo -e "[\e[32mPASS\e[0m] Target assembly correctly wrote /tmp/cli_test_out.s"
    cli_pass=$((cli_pass+1))
else
    echo -e "[\e[31mFAIL\e[0m] Target assembly did not write /tmp/cli_test_out.s correctly"
    cli_fail=$((cli_fail+1))
fi

# Target: default (full compilation) with custom binary output
assert_success "Full compilation with custom binary -o" ./c-tds -o /tmp/cli_test_bin Tests/valid/test_ret_simple.ctds
if [ -x /tmp/cli_test_bin ]; then
    /tmp/cli_test_bin > /dev/null 2>&1
    ret=$?
    if [ $ret -eq 0 ]; then
        echo -e "[\e[32mPASS\e[0m] Generated binary executed with expected return code 0"
        cli_pass=$((cli_pass+1))
    else
        echo -e "[\e[31mFAIL\e[0m] Generated binary exited with code $ret, expected 0"
        cli_fail=$((cli_fail+1))
    fi
else
    echo -e "[\e[31mFAIL\e[0m] Custom binary /tmp/cli_test_bin was not created or not executable"
    cli_fail=$((cli_fail+1))
fi

# 5. Granular Optimization flags (-opt)
assert_failure "Invalid -opt option fails" ./c-tds -opt invalid_opt Tests/valid/test_ret_simple.ctds
assert_success "-opt alone (all optimizations)" ./c-tds -opt Tests/valid/test_ret_simple.ctds
assert_success "-opt all" ./c-tds -opt all Tests/valid/test_ret_simple.ctds
assert_success "-opt fold" ./c-tds -opt fold Tests/valid/test_ret_simple.ctds
assert_success "-opt fold dce" ./c-tds -opt fold dce Tests/valid/test_ret_simple.ctds
assert_success "-opt sc alg" ./c-tds -opt sc alg Tests/valid/test_ret_simple.ctds

# Verify that without -opt, optimizations are NOT applied
./c-tds -target codinter -o /tmp/no_opt.ci Tests/valid/test_opt_ast.ctds
# With -opt all, optimizations ARE applied
./c-tds -opt all -target codinter -o /tmp/with_opt.ci Tests/valid/test_opt_ast.ctds

if grep -q "a_1 + t3" /tmp/no_opt.ci && grep -q "b_2 \* t5" /tmp/no_opt.ci; then
    echo -e "[\e[32mPASS\e[0m] Compilation without -opt preserved unoptimized code"
    cli_pass=$((cli_pass+1))
else
    echo -e "[\e[31mFAIL\e[0m] Compilation without -opt unexpectedly modified/optimized code"
    cli_fail=$((cli_fail+1))
fi

if grep -q "t4 = 30" /tmp/with_opt.ci && grep -q "t5 = 23" /tmp/with_opt.ci; then
    echo -e "[\e[32mPASS\e[0m] Compilation with -opt all performed constant folding (15*2=30, 30-10+3=23)"
    cli_pass=$((cli_pass+1))
else
    echo -e "[\e[31mFAIL\e[0m] Compilation with -opt all did not fold expected constants"
    cli_fail=$((cli_fail+1))
fi

# 6. Compatibility symlinks
assert_success "Symlink compilador_test works" ./compilador_test -h
assert_success "Symlink ctds works" ./ctds -h

# Cleanup temp files
rm -f /tmp/cli_test_* /tmp/no_opt.ci /tmp/with_opt.ci

echo -e "\n======================================"
echo -e "CLI Tests Summary: $cli_pass PASSED, $cli_fail FAILED"
echo -e "======================================\n"

exit $cli_fail
