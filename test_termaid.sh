#!/bin/bash
# TermAid Test Suite

echo "=========================================="
echo "  TermAid Test Suite"
echo "=========================================="
echo ""

TESTS_PASSED=0
TESTS_FAILED=0

test_command() {
    local description="$1"
    local command="$2"
    
    echo -n "Testing: $description... "
    
    if eval "$command" > /dev/null 2>&1; then
        echo "✓ PASS"
        ((TESTS_PASSED++))
        return 0
    else
        echo "✗ FAIL"
        ((TESTS_FAILED++))
        return 1
    fi
}

# Test 1: Script is executable
test_command "Script executable" "test -x ./termaid.py"

# Test 2: Python version
test_command "Python 3 available" "python3 --version"

# Test 3: List command
test_command "List command" "./termaid.py list"

# Test 4: Search command
test_command "Search command" "./termaid.py search copy"

# Test 5: Explain command
test_command "Explain command" "./termaid.py explain 'ls -la'"

# Test 6: Execute command
test_command "Execute command" "./termaid.py exec 'echo test' --no-confirm"

# Test 7: History command
test_command "History command" "./termaid.py history"

# Test 8: Command database exists
test_command "Commands directory exists" "test -d ./commands"

# Test 9: JSON files exist
test_command "JSON files exist" "find ./commands -name '*.json' | grep -q ."

# Test 10: Wrapper script exists
test_command "Wrapper script exists" "test -f ./termaid_wrapper.sh"

echo ""
echo "=========================================="
echo "  Test Results"
echo "=========================================="
echo "Passed: $TESTS_PASSED"
echo "Failed: $TESTS_FAILED"
echo ""

if [ $TESTS_FAILED -eq 0 ]; then
    echo "✓ All tests passed!"
    exit 0
else
    echo "✗ Some tests failed"
    exit 1
fi
