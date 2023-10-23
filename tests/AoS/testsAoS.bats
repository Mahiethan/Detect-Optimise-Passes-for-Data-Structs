#!/usr/bin/env bash

# Running unit tests for detection of AoS data structures (using flags)

setup() {
    load '../test_helper/bats-support/load'
    load '../test_helper/bats-assert/load'
}

@test "Test 1.1: Should detect a single static AoS " {
    # skip "Logs are not implemented yet"
    run $(opt -load-pass-plugin=../../passes/detectAoS/detectAoS.so -passes="detectAoS" -time-passes < testOne.ll > /dev/null 2> outputTestOne.txt)
    
    file_content=$(sed -n "2p" "outputTestOne.txt") #get second line
    [ "$file_content" == "Number of AoS data structures: 1" ] 

}

@test "Test 1.2: Should detect a single static AoS within 0.5 seconds" {

    time_val=$(bc <<< $(sed -n "6p" "outputTestOne.txt" | awk '{print $4}')) #get 6th line from file then get the 4th word
    result2=0.5
    fin=$(echo "$time_val < $result2" | bc)
    [ "$fin" == "1" ]
}

@test "Test 2.1: Should detect a dynamic AoS " {
    # skip "Logs are not implemented yet"
    run $(opt -load-pass-plugin=../../passes/detectAoS/detectAoS.so -passes="detectAoS" -time-passes < testTwo.ll > /dev/null 2> outputTestTwo.txt)
    
    file_content=$(sed -n "2p" "outputTestTwo.txt") #get second line
    [ "$file_content" == "Number of AoS data structures: 1" ] 

}

@test "Test 2.2: Should detect a single dynamic AoS within 1 second" {

    time_val=$(bc <<< $(sed -n "6p" "outputTestTwo.txt" | awk '{print $4}')) #get 6th line from file then get the 4th word
    result2=0.5
    fin=$(echo "$time_val < $result2" | bc)
    [ "$fin" == "1" ]
}


@test "Test 3.1: Should detect multiple static AoS" {
    #skip "Test file not implemented yet"
    run $(opt -load-pass-plugin=../../passes/detectAoS/detectAoS.so -passes="detectAoS" -time-passes  < testThree.ll > /dev/null 2> outputTestThree.txt)

    file_content=$(sed -n "4p" "outputTestThree.txt") #get fourth line
    [ "$file_content" == "Number of AoS data structures: 3" ] 
}

@test "Test 3.2: Should detect multiple dynamic AoS within 1 second" {

    time_val=$(bc <<< $(sed -n "8p" "outputTestThree.txt" | awk '{print $4}')) #get 8th line from file then get the 4th word
    result2=0.5
    fin=$(echo "$time_val < $result2" | bc)
    [ "$fin" == "1" ]
}

@test "Test 4.1: Should detect multiple dynamic AoS" {
    run $(opt -load-pass-plugin=../../passes/detectAoS/detectAoS.so -passes="detectAoS" -time-passes < testFour.ll > /dev/null 2> outputTestFour.txt)

    file_content=$(sed -n "4p" "outputTestFour.txt") #get fourth line
    [ "$file_content" == "Number of AoS data structures: 3" ] 
}

@test "Test 4.2: Should detect multiple dynamic AoS within 1 second" {

    time_val=$(bc <<< $(sed -n "8p" "outputTestFour.txt" | awk '{print $4}')) #get 8th line from file then get the 4th word
    result2=0.5
    fin=$(echo "$time_val < $result2" | bc)
    [ "$fin" == "1" ]
}

@test "Test 5.1: Should detect mixture of static and dynamic AoS" {
    run $(opt -load-pass-plugin=../../passes/detectAoS/detectAoS.so -passes="detectAoS" -time-passes  < testFive.ll > /dev/null 2> outputTestFive.txt)

     file_content=$(sed -n "7p" "outputTestFive.txt") #get seventh line
    [ "$file_content" == "Number of AoS data structures: 6" ] 
}

@test "Test 5.2: Should detect multiple dynamic AoS within 1 second" {

    time_val=$(bc <<< $(sed -n "11p" "outputTestFive.txt" | awk '{print $4}')) #get 8th line from file then get the 4th word
    result2=0.5
    fin=$(echo "$time_val < $result2" | bc)
    [ "$fin" == "1" ]
}

#removing output files
teardown_file() {
    rm -f outputTestOne.txt
    rm -f outputTestTwo.txt
    rm -f outputTestThree.txt
    rm -f outputTestFour.txt
    rm -f outputTestFive.txt
}