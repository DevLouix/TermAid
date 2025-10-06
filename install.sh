#!/bin/bash
# TermAid Installation Script

set -e

INSTALL_DIR="/usr/local/bin"
SCRIPT_NAME="termaid"
CURRENT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

echo "=========================================="
echo "  TermAid Installation"
echo "=========================================="
echo ""

# Check if Python 3 is installed
if ! command -v python3 &> /dev/null; then
    echo "❌ Error: Python 3 is not installed."
    echo "Please install Python 3.6 or higher and try again."
    exit 1
fi

PYTHON_VERSION=$(python3 --version | cut -d' ' -f2 | cut -d'.' -f1,2)
echo "✓ Found Python $PYTHON_VERSION"

# Check Python version
MAJOR=$(echo $PYTHON_VERSION | cut -d'.' -f1)
MINOR=$(echo $PYTHON_VERSION | cut -d'.' -f2)

if [ "$MAJOR" -lt 3 ] || ([ "$MAJOR" -eq 3 ] && [ "$MINOR" -lt 6 ]); then
    echo "❌ Error: Python 3.6 or higher is required."
    echo "Current version: $PYTHON_VERSION"
    exit 1
fi

echo ""
echo "Installation options:"
echo "1. System-wide installation (requires sudo)"
echo "2. Local installation (current directory only)"
echo "3. Cancel"
echo ""
read -p "Choose option [1-3]: " choice

case $choice in
    1)
        echo ""
        echo "Installing TermAid system-wide..."
        
        # Make script executable
        chmod +x "$CURRENT_DIR/termaid.py"
        
        # Create wrapper script
        sudo tee "$INSTALL_DIR/$SCRIPT_NAME" > /dev/null << EOF
#!/bin/bash
# TermAid wrapper script
python3 "$CURRENT_DIR/termaid.py" "\$@"
EOF
        
        # Make wrapper executable
        sudo chmod +x "$INSTALL_DIR/$SCRIPT_NAME"
        
        echo "✓ TermAid installed successfully!"
        echo ""
        echo "You can now use: termaid <command>"
        echo ""
        echo "Examples:"
        echo "  termaid list"
        echo "  termaid search copy"
        echo "  termaid explain 'ls -la'"
        echo "  termaid history"
        ;;
    
    2)
        echo ""
        echo "Setting up local installation..."
        
        # Make script executable
        chmod +x "$CURRENT_DIR/termaid.py"
        
        echo "✓ TermAid is ready to use!"
        echo ""
        echo "Use: ./termaid.py <command>"
        echo ""
        echo "To add to PATH, add this to your ~/.bashrc or ~/.zshrc:"
        echo "  export PATH=\"$CURRENT_DIR:\$PATH\""
        echo "  alias termaid='$CURRENT_DIR/termaid.py'"
        ;;
    
    3)
        echo "Installation cancelled."
        exit 0
        ;;
    
    *)
        echo "Invalid option. Installation cancelled."
        exit 1
        ;;
esac

echo ""
echo "=========================================="
echo "  Quick Start"
echo "=========================================="
echo ""
echo "Try these commands:"
echo "  termaid list              # List all available commands"
echo "  termaid search network    # Search for network commands"
echo "  termaid explain 'ls -la'  # Explain a command"
echo "  termaid history           # View command history"
echo ""
echo "For more information:"
echo "  cat $CURRENT_DIR/ReadMe.md"
echo "  cat $CURRENT_DIR/QUICKSTART.md"
echo ""
echo "Happy learning! 🚀"
