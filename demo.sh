#!/bin/bash
# TermAid Demo Script
# Demonstrates all major features of TermAid

echo "=========================================="
echo "  TermAid - Terminal Command Aid Demo"
echo "=========================================="
echo ""

# Colors for output
GREEN='\033[0;32m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

pause() {
    echo ""
    echo -e "${YELLOW}Press Enter to continue...${NC}"
    read
    echo ""
}

echo -e "${BLUE}1. Listing all available commands${NC}"
echo "Command: ./termaid.py list"
pause
./termaid.py list | head -30
echo "... (showing first 30 lines)"
pause

echo -e "${BLUE}2. Searching for commands${NC}"
echo "Command: ./termaid.py search copy"
pause
./termaid.py search copy
pause

echo -e "${BLUE}3. Explaining a simple command${NC}"
echo "Command: ./termaid.py explain 'ls -la'"
pause
./termaid.py explain "ls -la"
pause

echo -e "${BLUE}4. Explaining a complex command${NC}"
echo "Command: ./termaid.py explain 'tar -czf backup.tar.gz /home'"
pause
./termaid.py explain "tar -czf backup.tar.gz /home"
pause

echo -e "${BLUE}5. Executing a command with logging${NC}"
echo "Command: ./termaid.py exec 'echo Hello TermAid' --no-confirm"
pause
./termaid.py exec "echo 'Hello TermAid!'" --no-confirm
pause

echo -e "${BLUE}6. Executing another command${NC}"
echo "Command: ./termaid.py exec 'date' --no-confirm"
pause
./termaid.py exec "date" --no-confirm
pause

echo -e "${BLUE}7. Viewing command history${NC}"
echo "Command: ./termaid.py history"
pause
./termaid.py history
pause

echo -e "${BLUE}8. Searching for network commands${NC}"
echo "Command: ./termaid.py search network"
pause
./termaid.py search network
pause

echo -e "${BLUE}9. Searching for compression commands${NC}"
echo "Command: ./termaid.py search compress"
pause
./termaid.py search compress
pause

echo -e "${GREEN}=========================================="
echo "  Demo Complete!"
echo "==========================================${NC}"
echo ""
echo "Try these commands yourself:"
echo "  ./termaid.py search <keyword>"
echo "  ./termaid.py explain '<command>'"
echo "  ./termaid.py exec '<command>'"
echo "  ./termaid.py history"
echo "  ./termaid.py list"
echo ""
echo "For more information, see ReadMe.md"
