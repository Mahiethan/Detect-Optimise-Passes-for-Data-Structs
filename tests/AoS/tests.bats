#!/usr/bin/env bash

setup() {
    load '../test_helper/bats-support/load'
    load '../test_helper/bats-assert/load'
    # load '../test/test_helper/bats-file/load'
    #DIR="$( cd "$( dirname "$BATS_TEST_FILENAME" )" >/dev/null 2>&1 && pwd )"
    #PATH="$DIR/../src:$PATH"
}

@test "Should detect single static AoS" {
    # skip "Logs are not implemented yet"
    run opt -load-pass-plugin=../../passes/detectAoS/detectAoS.so -passes="detectAoS"  < staticAoSWithFlags.ll > /dev/null

    assert_output --partial "Array of Structs (AoS) detected"
}

@test "Should detect single dynamic AoS" {
    # skip "Logs are not implemented yet"
    run opt -load-pass-plugin=../../passes/detectAoS/detectAoS.so -passes="detectAoS"  < dynamicAoSWithFlags.ll > /dev/null

    assert_output --partial "Array of Structs (AoS) detected"
}

@test "Should detect multiple static AoS" {
    skip "Test file not implemented yet"
    run opt -load-pass-plugin=../../passes/detectAoS/detectAoS.so -passes="detectAoS"  < dynamicAoSWithFlags.ll > /dev/null

    assert_output --partial "Array of Structs (AoS) detected"
}

@test "Should detect multiple dynamic AoS" {
    skip "Test file not implemented yet"
    run opt -load-pass-plugin=../../passes/detectAoS/detectAoS.so -passes="detectAoS"  < dynamicAoSWithFlags.ll > /dev/null

    assert_output --partial "Array of Structs (AoS) detected"
}

@test "Should detect mixture of staic and dynamic AoS" {
    skip "Test file not implemented yet"
    run opt -load-pass-plugin=../../passes/detectAoS/detectAoS.so -passes="detectAoS"  < dynamicAoSWithFlags.ll > /dev/null

    assert_output --partial "Array of Structs (AoS) detected"
}