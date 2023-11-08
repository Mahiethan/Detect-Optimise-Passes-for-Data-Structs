#!/usr/bin/env bash

# Running unit tests for detection of AoS data structures (using flags)

# setup() {
#     load '../test_helper/bats-support/load'
#     load '../test_helper/bats-assert/load'
# }

@test "Test 1.1: Should detect a single static AoS using flags " {
    # skip "Logs are not implemented yet"
    run $(opt -load-pass-plugin=../../passes/detectAoS/detectAoSFlags/detectAoSFlags.so -passes="detectAoSFlags" -time-passes < testOne.ll > /dev/null 2> outputTestOne.txt)
    
    file_content=$(sed -n "2p" "outputTestOne.txt") #get second line
    [ "$file_content" == "Number of AoS data structures: 1" ] 

}

@test "Test 1.2: Complete test 1.1 within 0.001 seconds" {

    time_val=$(bc <<< $(sed -n "6p" "outputTestOne.txt" | awk '{print $4}')) #get 6th line from file then get the 4th word
    result2=$(bc<<<0.001)
    fin=$(echo "$time_val < $result2" | bc)
    [ "$fin" == "1" ]
}

@test "Test 1.3: Should detect a dynamic AoS using flags" {
    # skip "Logs are not implemented yet"
    run $(opt -load-pass-plugin=../../passes/detectAoS/detectAoSFlags/detectAoSFlags.so -passes="detectAoSFlags" -time-passes < testTwo.ll > /dev/null 2> outputTestTwo.txt)
    
    file_content=$(sed -n "2p" "outputTestTwo.txt") #get second line
    [ "$file_content" == "Number of AoS data structures: 1" ] 

}

@test "Test 1.4: Complete test 1.3 within 0.001 seconds" {

    time_val=$(bc <<< $(sed -n "6p" "outputTestTwo.txt" | awk '{print $4}')) #get 6th line from file then get the 4th word
    result2=$(bc<<<0.001)
    fin=$(echo "$time_val < $result2" | bc)
    [ "$fin" == "1" ]
}


@test "Test 1.5: Should detect multiple static AoS using flags" {
    #skip "Test file not implemented yet"
    run $(opt -load-pass-plugin=../../passes/detectAoS/detectAoSFlags/detectAoSFlags.so -passes="detectAoSFlags" -time-passes  < testThree.ll > /dev/null 2> outputTestThree.txt)

    file_content=$(sed -n "4p" "outputTestThree.txt") #get fourth line
    [ "$file_content" == "Number of AoS data structures: 3" ] 
}

@test "Test 1.6: Complete test 1.5 within 0.001 seconds" {

    time_val=$(bc <<< $(sed -n "8p" "outputTestThree.txt" | awk '{print $4}')) #get 8th line from file then get the 4th word
    result2=$(bc<<<0.001)
    fin=$(echo "$time_val < $result2" | bc)
    [ "$fin" == "1" ]
}

@test "Test 1.7: Should detect multiple dynamic AoS using flags" {
    run $(opt -load-pass-plugin=../../passes/detectAoS/detectAoSFlags/detectAoSFlags.so -passes="detectAoSFlags" -time-passes < testFour.ll > /dev/null 2> outputTestFour.txt)

    file_content=$(sed -n "4p" "outputTestFour.txt") #get fourth line
    [ "$file_content" == "Number of AoS data structures: 3" ] 
}

@test "Test 1.8: Complete test 1.7 within 0.001 seconds" {

    time_val=$(bc <<< $(sed -n "8p" "outputTestFour.txt" | awk '{print $4}')) #get 8th line from file then get the 4th word
    result2=$(bc<<<0.001)
    fin=$(echo "$time_val < $result2" | bc)
    [ "$fin" == "1" ]
}

@test "Test 1.9: Should detect mixture of static and dynamic AoS using flags" {
    run $(opt -load-pass-plugin=../../passes/detectAoS/detectAoSFlags/detectAoSFlags.so -passes="detectAoSFlags" -time-passes  < testFiveOne.ll > /dev/null 2> outputTestFive.txt)

     file_content=$(sed -n "7p" "outputTestFive.txt") #get seventh line
    [ "$file_content" == "Number of AoS data structures: 6" ] 
}

@test "Test 1.10: Complete test 1.9 within 0.001 seconds" {

    time_val=$(bc <<< $(sed -n "11p" "outputTestFive.txt" | awk '{print $4}')) #get 8th line from file then get the 4th word
    result2=$(bc<<<0.001)
    fin=$(echo "$time_val < $result2" | bc)
    [ "$fin" == "1" ]
}

#-------------------------------------------------------------------------------------------------------------------------------------

# Detection of static AoS without flags


@test "Test 2.1: Should detect a single static AoS without flags " {
    # skip "Logs are not implemented yet"
    run $(opt -load-pass-plugin=../../passes/detectAoS/detectAoS.so -passes="detectAoS" -time-passes < testOne.ll > /dev/null 2> outputTestOne.txt)
    
    file_content=$(sed -n "2p" "outputTestOne.txt") #get second line
    [ "$file_content" == "Number of static AoS data structures: 1" ] 

}

@test "Test 2.2: Complete test 2.1 within 0.001 seconds" {
    # skip "Logs are not implemented yet"
    time_val=$(bc <<< $(sed -n "7p" "outputTestOne.txt" | awk '{print $4}')) #get 7th line from file then get the 4th word
    result2=$(bc<<<0.001)
    fin=$(echo "$time_val < $result2" | bc)
    [ "$fin" == "1" ]
}

@test "Test 2.3: Should detect a single dynamic AoS without flags " {
    #  skip "Not implemented yet"
    run $(opt -load-pass-plugin=../../passes/detectAoS/detectAoS.so -passes="detectAoS" -time-passes < testTwo.ll > /dev/null 2> outputTestTwo.txt)
    
    file_content=$(sed -n "3p" "outputTestTwo.txt") #get third line
    [ "$file_content" == "Number of dynamic AoS data structures: 1" ] 

}

@test "Test 2.4: Complete test 2.3 within 0.001 seconds" {
     skip "Not implemented yet"
    time_val=$(bc <<< $(sed -n "7p" "outputTestTwo.txt" | awk '{print $4}')) #get 8th line from file then get the 4th word
    result2=$(bc<<<0.001)
    fin=$(echo "$time_val < $result2" | bc)
    [ "$fin" == "1" ]

}

@test "Test 2.5: Should detect multiple static AoS without flags " {
    # skip "Logs are not implemented yet"
    run $(opt -load-pass-plugin=../../passes/detectAoS/detectAoS.so -passes="detectAoS" -time-passes < testThree.ll > /dev/null 2> outputTestThree.txt)
    
    file_content=$(sed -n "4p" "outputTestThree.txt") #get second line
    [ "$file_content" == "Number of static AoS data structures: 3" ] 

}

@test "Test 2.6: Complete test 2.5 within 0.001 seconds" {
    # skip "Logs are not implemented yet"
    time_val=$(bc <<< $(sed -n "9p" "outputTestThree.txt" | awk '{print $4}')) #get 9th line from file then get the 4th word
    result2=$(bc<<<0.001)
    fin=$(echo "$time_val < $result2" | bc)
    [ "$fin" == "1" ]

}

@test "Test 2.7: Should detect multiple dynamic AoS without flags " {
    skip "Not implemented yet"
    run $(opt -load-pass-plugin=../../passes/detectAoS/detectAoS.so -passes="detectAoS" -time-passes < testFour.ll > /dev/null 2> outputTestFour.txt)
    
    file_content=$(sed -n "5p" "outputTestFour.txt") #get fifth line
    [ "$file_content" == "Number of dynamic AoS data structures: 3" ] 

}

@test "Test 2.8: Complete test 2.7 within 0.001 seconds" {
     skip "Not implemented yet"
    time_val=$(bc <<< $(sed -n "8p" "outputTestFour.txt" | awk '{print $4}')) #get 8th line from file then get the 4th word
    result2=$(bc<<<0.001)
    fin=$(echo "$time_val < $result2" | bc)
    [ "$fin" == "1" ]

}

@test "Test 2.9: Should detect multiple static AoS among other data structures without flags " {
    # skip "Logs are not implemented yet"
    run $(opt -load-pass-plugin=../../passes/detectAoS/detectAoS.so -passes="detectAoS" -time-passes < testFiveTwo.ll > /dev/null 2> outputTestFive.txt)
    file_content=$(sed -n "7p" "outputTestFive.txt") #get second line
    [ "$file_content" == "Number of static AoS data structures: 6" ] 

}

@test "Test 2.10: Complete test 2.9 within 0.001 seconds" {
    # skip "Logs are not implemented yet"
    time_val=$(bc <<< $(sed -n "12p" "outputTestFive.txt" | awk '{print $4}')) #get 12th line from file then get the 4th word
    result2=$(bc<<<0.001)
    fin=$(echo "$time_val < $result2" | bc)
    [ "$fin" == "1" ]

}

@test "Test 2.11: Should detect multiple dynamic AoS among other data structures without flags " {
    skip "Logs are not implemented yet"
     run $(opt -load-pass-plugin=../../passes/detectAoS/detectAoS.so -passes="detectAoS" -time-passes < testSix.ll > /dev/null 2> outputTestSix.txt)
    file_content=$(sed -n "8p" "outputTestSix.txt") 
    [ "$file_content" == "Number of dynamic AoS data structures: 6" ] 


}

@test "Test 2.12: Complete test 2.11 within 0.001 seconds" {
    skip "Logs are not implemented yet"
    time_val=$(bc <<< $(sed -n "12p" "outputTestSix.txt" | awk '{print $4}')) #get 12th line from file then get the 4th word
    result2=$(bc<<<0.001)
    fin=$(echo "$time_val < $result2" | bc)
    [ "$fin" == "1" ]

}

@test "Test 2.13: Should detect mix of static and dynamic AoS without flags " {
    skip "Logs are not implemented yet"
    run $(opt -load-pass-plugin=../../passes/detectAoS/detectAoS.so -passes="detectAoS" -time-passes < static_dynamicMix.ll > /dev/null 2> outputTestSeven.txt)
    file_content_1=$(sed -n "7p" "outputTestSeven.txt") 
    file_content_2=$(sed -n "8p" "outputTestSeven.txt") 
    [ "$file_content_1" == "Number of static AoS data structures: 6" && "$file_content_2" == "Number of dynamic AoS data structures: 6"] 

}

@test "Test 2.14: Complete test 2.13 within 0.001 seconds" {
    skip "Logs are not implemented yet"
    time_val=$(bc <<< $(sed -n "12p" "outputTestSeven.txt" | awk '{print $4}')) #get 12th line from file then get the 4th word
    result2=$(bc<<<0.001)
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
    # rm -f outputTestSix.txt
    #rm -f outputTestSeven.txt
}