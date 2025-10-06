# TermAid Quick Reference Card

## Installation
```bash
git clone https://github.com/DevLouix/TermAid.git
cd TermAid
./install.sh
```

## Basic Commands

| Command | Description | Example |
|---------|-------------|---------|
| `termaid list` | List all available commands | `termaid list` |
| `termaid search <keyword>` | Search for commands | `termaid search network` |
| `termaid explain '<command>'` | Explain a command | `termaid explain 'ls -la'` |
| `termaid exec '<command>'` | Execute with logging | `termaid exec 'echo test'` |
| `termaid history` | View command history | `termaid history --limit 20` |

## Common Searches

```bash
termaid search copy          # File copying commands
termaid search network       # Network commands
termaid search compress      # Compression commands
termaid search permission    # Permission commands
termaid search process       # Process management
termaid search text          # Text processing
termaid search disk          # Disk management
termaid search package       # Package management
termaid search find          # File search commands
termaid search monitor       # System monitoring
```

## Usage Patterns

### Learning Mode
```bash
# Don't know how to do something?
termaid search <what_you_want_to_do>

# Example: How to compress files?
termaid search compress
```

### Safety Mode
```bash
# Not sure if command is safe?
termaid explain '<command>'

# Example: What does this do?
termaid explain 'rm -rf /tmp/old'
```

### Execution Mode
```bash
# Execute with explanation and confirmation
termaid exec '<command>'

# Execute without confirmation
termaid exec '<command>' --no-confirm
```

### History Mode
```bash
# View recent commands
termaid history

# View more history
termaid history --limit 50
```

## Command Categories (154 total)

| Category | Count | Examples |
|----------|-------|----------|
| File Operations | 40+ | ls, cp, mv, rm, cat, nano, tree |
| System Info | 30+ | df, free, top, ps, uname, watch |
| Network | 20+ | ping, ssh, curl, wget, ip |
| Process Mgmt | 20+ | kill, systemctl, bg, fg, nice |
| Text Processing | 15+ | grep, sed, awk, sort, wc |
| Permissions | 10+ | chmod, chown, sudo, su |
| Package Mgmt | 15+ | apt, yum, dnf, pacman, snap |
| Archive | 10+ | tar, zip, gzip, bzip2, 7z |
| Disk Mgmt | 10+ | mount, fdisk, dd, lsof |
| Search | 8+ | find, locate, which, whereis |

## Tips & Tricks

### Add Alias
```bash
# Add to ~/.bashrc or ~/.zshrc
alias ta='termaid'

# Now use: ta search copy
```

### Shell Wrapper
```bash
# Load wrapper
source termaid_wrapper.sh

# Enable interception
termaid_enable

# All commands now explained before execution
ls -la
rm file.txt

# Disable when done
termaid_disable
```

### Quick Help
```bash
termaid --help              # Show help
termaid search --help       # Command-specific help
```

## Most Useful Commands

### File Operations
```bash
termaid explain 'ls -la'           # List files
termaid explain 'cp -r src dst'    # Copy directory
termaid explain 'rm -rf folder'    # Remove directory
termaid explain 'find . -name "*.txt"'  # Find files
```

### System Info
```bash
termaid explain 'df -h'            # Disk usage
termaid explain 'free -h'          # Memory usage
termaid explain 'top'              # Process monitor
termaid explain 'ps aux'           # List processes
```

### Network
```bash
termaid explain 'ping google.com'  # Test connectivity
termaid explain 'ssh user@host'    # Remote login
termaid explain 'curl -O url'      # Download file
termaid explain 'ip addr'          # Show IP addresses
```

### Text Processing
```bash
termaid explain "grep 'error' log.txt"     # Search in file
termaid explain "sed 's/old/new/g' file"   # Replace text
termaid explain "awk '{print $1}' file"    # Extract column
```

### Compression
```bash
termaid explain 'tar -czf backup.tar.gz /home'  # Create archive
termaid explain 'tar -xzf backup.tar.gz'        # Extract archive
termaid explain 'zip -r backup.zip folder'      # Create zip
```

## Keyboard Shortcuts

When executing commands:
- `Enter` - Confirm execution
- `n` + `Enter` - Cancel execution
- `Ctrl+C` - Abort

## Files & Locations

| File | Purpose |
|------|---------|
| `termaid.py` | Main application |
| `commands/` | Command database |
| `.termaid_history.json` | Command history |
| `termaid_wrapper.sh` | Shell integration |

## Getting Help

```bash
# Built-in help
termaid --help

# Documentation
cat ReadMe.md
cat QUICKSTART.md
cat CONTRIBUTING.md

# Demo
./demo.sh

# Tests
./test_termaid.sh
```

## Common Issues

**Command not found**
- Make sure you installed: `./install.sh`
- Or use: `./termaid.py` directly

**Python not found**
- Install Python 3.6+: `sudo apt install python3`

**Permission denied**
- Make executable: `chmod +x termaid.py`
- Use sudo for system commands: `sudo termaid exec 'apt update'`

## Pro Tips

1. **Use search liberally** - Don't remember syntax? Search!
2. **Explain before executing** - Avoid mistakes
3. **Check history** - Learn from patterns
4. **Contribute** - Add commands you use
5. **Create aliases** - Make it faster to use

## Quick Examples

```bash
# Learn about a command
termaid search tar

# Understand before running
termaid explain 'chmod 755 script.sh'

# Execute safely
termaid exec 'rm -rf /tmp/old_files'

# Review what you've done
termaid history

# See everything available
termaid list | less
```

---

**Print this card and keep it handy!**

For full documentation: [ReadMe.md](ReadMe.md)
