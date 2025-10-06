#!/bin/bash
# TermAid Shell Wrapper
# Intercepts commands and provides explanations before execution

TERMAID_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
TERMAID_SCRIPT="$TERMAID_DIR/termaid.py"

# Function to wrap command execution
termaid_exec() {
    local cmd="$*"
    
    # Skip empty commands
    if [ -z "$cmd" ]; then
        return 0
    fi
    
    # Skip TermAid commands to avoid recursion
    if [[ "$cmd" == termaid* ]] || [[ "$cmd" == python*termaid* ]]; then
        eval "$cmd"
        return $?
    fi
    
    # Execute with TermAid
    python3 "$TERMAID_SCRIPT" exec "$cmd"
    return $?
}

# Function to enable TermAid wrapper mode
termaid_enable() {
    echo "TermAid wrapper mode enabled!"
    echo "All commands will be explained before execution."
    echo "Type 'termaid_disable' to turn off."
    echo ""
    
    # Override common commands
    alias cd='termaid_exec cd'
    alias ls='termaid_exec ls'
    alias rm='termaid_exec rm'
    alias cp='termaid_exec cp'
    alias mv='termaid_exec mv'
    alias mkdir='termaid_exec mkdir'
    alias chmod='termaid_exec chmod'
    alias chown='termaid_exec chown'
    
    export TERMAID_ENABLED=1
}

# Function to disable TermAid wrapper mode
termaid_disable() {
    echo "TermAid wrapper mode disabled."
    
    # Remove aliases
    unalias cd ls rm cp mv mkdir chmod chown 2>/dev/null
    
    unset TERMAID_ENABLED
}

# Make functions available
export -f termaid_exec
export -f termaid_enable
export -f termaid_disable

echo "TermAid shell wrapper loaded!"
echo "Commands:"
echo "  termaid_enable  - Enable command interception and explanation"
echo "  termaid_disable - Disable command interception"
echo "  termaid.py      - Use TermAid directly"
