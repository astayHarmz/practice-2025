#!/bin/bash

TEST_FILES=("test1.txt" "test2.txt" "test3.txt" "test4.txt" "empty.txt" "empty_string.txt" "space.txt" "tabl.txt")
FLAGS=("" "-b" "-e" "-n" "-s" "-t")
VALGRIND_LOG="valgrind_cat_summary.log"

create_test_files() {
    touch empty.txt
    printf "\n" > empty_string.txt
    printf "      " > space.txt
    printf "\t\n\t\t" > tabl.txt
    printf "Lorem ipsum dolor sit amet, consectetur ad\tipiscing elit. Phasellus fringilla justo vel\n\n\nipsum condimentum, ac volutpat lacus viverra. Sed placerat ultricies neque, sit\namet vulputate metus commodo eget. Cras hendrerit vehicula leo, nec luctus nulla\nconsectetur vitae. Donec vulputate justo at nunc euismod, quis sollicitudin libero\nrutrum." > test1.txt
    printf "Text with\ttabs\n\nMultiple\n\nnewlines" > test2.txt
    printf "Special\n\n\nchars\n\t\there" > test3.txt
    printf "Sed non aliquet orci. Vivamus ac convallis lectus. Nam sed \tenim et diam\n\nnullamcorper scelerisque. Vestibulum at est auctor, euismod ligula at, convallis\njusto. Quisque vehicula pulvinar ante, nec viverra risus pharetra vitae. Proin ac\nlacinia augue. Pellentesque habitant morbi tristique senectus et netus et\nmalesuada fames ac turpis egestas." > test4.txt
}

check_leaks() {
    local command=$1
    local file=$2
    local flag=$3
    
    echo "===== Valgrind check for: $command $flag $file =====" >> "$VALGRIND_LOG"
        
    valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all \
                --errors-for-leak-kinds=all --error-exitcode=1 \
                $command $flag "$file" >> "$VALGRIND_LOG" 2>&1
    
    if [ $? -ne 0 ]; then
        echo "MEMORY LEAK DETECTED: $command $flag $file" | tee -a "$VALGRIND_LOG"
        return 1
    else
        echo "No memory leaks detected" >> "$VALGRIND_LOG"
    fi
    echo >> "$VALGRIND_LOG"

    return 0
}


run_tests() {
    local total=0
    local passed=0
    local failed=0
    local leaks=0

    for file in "${TEST_FILES[@]}"; do
        for flag in "${FLAGS[@]}"; do
            ((total++))

            if ! check_leaks "./cat/s21_cat" "$file" "$flag"; then
                ((leaks++))
            fi
            
            original=$(cat $flag "$file" 2>&1)
            s21_version=$(./cat/s21_cat $flag "$file" 2>&1)

            if [ "$original" = "$s21_version" ]; then
                ((passed++))
            else
                ((failed++))
                echo "FAIL: cat $flag $file"
                echo "Expected: $original"
                echo "Got: $s21_version"
            fi
        done
    done

    echo "===== Cat Test Results ====="
    echo "TOTAL: $total"
    echo "PASSED: $passed"
    echo "FAILED: $failed"
    echo "LEAKS DETECTED: $leaks"
}

create_test_files
run_tests

rm -f "${TEST_FILES[@]}"