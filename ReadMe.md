# TermAid - Terminal Command Aid and Logger

A comprehensive terminal assistant that logs, explains, and helps you understand Linux commands before and after execution.

**Available in Python and C++ for maximum portability!**

## Features

✅ **140+ Linux Commands** - Comprehensive database covering:
- File operations (ls, cp, mv, rm, etc.)
- System information (df, free, uname, etc.)
- Network commands (ping, ssh, curl, wget, etc.)
- Process management (ps, top, kill, systemctl, etc.)
- Text processing (grep, sed, awk, etc.)
- Permissions (chmod, chown, sudo, etc.)
- Package management (apt, yum, dnf, pacman, etc.)
- Archive/compression (tar, zip, gzip, etc.)
- Disk management (mount, fdisk, dd, etc.)
- And much more!

✅ **Command Explanation** - Get detailed explanations before running commands

✅ **Command History** - Track all commands with timestamps and exit codes

✅ **Smart Search** - Find commands by keyword or functionality

✅ **Safe Execution** - Optional confirmation before running commands

✅ **Command Logging** - Automatic logging of all executed commands

## Installation

### Choose Your Version

**Python Version** (Recommended for full features):
- Full feature set
- Easy to modify
- Cross-platform command execution

**C++ Version** (Recommended for performance):
- 10x faster
- Single binary
- No dependencies
- Cross-platform (Linux, macOS, Windows)

### Python Installation

1. Clone the repository:
   ```bash
   git clone https://github.com/DevLouix/TermAid.git
   cd TermAid
   ```

2. Run installation script:
   ```bash
   ./install.sh
   ```

Or manually:
   ```bash
   chmod +x termaid.py
   sudo ln -s $(pwd)/termaid.py /usr/local/bin/termaid
   ```

**Requirements**: Python 3.6 or higher

### C++ Installation

1. Clone the repository (if not already done)
2. Build:
   ```bash
   ./build.sh
   ```

Or manually:
   ```bash
   mkdir build && cd build
   cmake .. -DCMAKE_BUILD_TYPE=Release
   cmake --build .
   sudo cmake --install .
   ```

**Requirements**: CMake 3.15+, C++17 compiler

See [CPP_BUILD.md](CPP_BUILD.md) for detailed C++ build instructions.

## Usage

### Basic Commands

#### Search for Commands
```bash
./termaid.py search copy
./termaid.py search network
./termaid.py search compress
```

#### Explain a Command
```bash
./termaid.py explain "ls -la"
./termaid.py explain "tar -czf backup.tar.gz /home"
./termaid.py explain "grep -r 'error' /var/log"
```

#### Execute with Explanation
```bash
./termaid.py exec "ls -la"
# Shows explanation and asks for confirmation before executing

./termaid.py exec "echo 'Hello'" --no-confirm
# Execute without confirmation
```

#### View Command History
```bash
./termaid.py history
./termaid.py history --limit 50
```

#### List All Available Commands
```bash
./termaid.py list
```

### Shell Wrapper Mode (Advanced)

For automatic command interception and explanation:

```bash
# Load the wrapper
source termaid_wrapper.sh

# Enable interception mode
termaid_enable

# Now all commands will be explained before execution
ls -la
rm file.txt

# Disable when done
termaid_disable
```

## Examples

### Example 1: Learning a New Command
```bash
$ ./termaid.py search compress

Terminal commands for 'compress':
============================================================

Command: tar -czf <archive.tar.gz> <files>
Explanation: Create compressed tar.gz archive
Example: tar -czf backup.tar.gz /home/user
------------------------------------------------------------

Command: gzip <file>
Explanation: Compress file (creates .gz, removes original)
Example: gzip largefile.txt
------------------------------------------------------------
```

### Example 2: Understanding Before Executing
```bash
$ ./termaid.py explain "rm -rf /tmp/old_files"

============================================================
Command: rm -rf /tmp/old_files
============================================================

Explanation: Force remove directory recursively (use with caution!)
Example: rm -rf old_folder

Arguments: -rf /tmp/old_files
============================================================
```

### Example 3: Tracking Command History
```bash
$ ./termaid.py history --limit 5

Command History (last 5 entries):
================================================================================
✓ [2025-10-05T20:01:53] echo 'Testing TermAid'
   → Display text or variable value
✓ [2025-10-05T19:58:42] ls -la
   → List in long format with detailed information
✓ [2025-10-05T19:55:12] grep -r 'error' /var/log
   → Recursively search for pattern in directory
================================================================================
```

## Command Database Structure

Commands are organized in JSON files under the `commands/` directory:

```
commands/
├── file_operations/
│   ├── basic.json
│   └── editors.json
├── system_info/
│   ├── system.json
│   ├── environment.json
│   └── users.json
├── network/
│   └── network.json
├── process_management/
│   └── processes.json
├── text_processing/
│   └── text.json
├── permissions/
│   └── permissions.json
├── package_management/
│   └── apt.json
├── archive/
│   └── compression.json
├── disk_management/
│   └── disk.json
└── search/
    └── find.json
```

## Contributing

Contributions are welcome! Here's how you can help:

### Adding New Commands

1. Navigate to the appropriate category in `commands/` or create a new one
2. Add your command in JSON format:

```json
{
  "command_name": [
    {
      "response": "command <args>",
      "explanation": "What the command does",
      "example": "command example usage"
    }
  ]
}
```

3. Test your additions:
```bash
./termaid.py search your_command
./termaid.py explain "your_command --flag"
```

### Improving Existing Commands

- Add more examples
- Clarify explanations
- Add common use cases
- Fix errors or typos

### Code Contributions

- Improve command parsing logic
- Enhance matching algorithms
- Add new features
- Fix bugs

## Project Structure

```
TermAid/
├── termaid.py              # Main Python script
├── termaid_wrapper.sh      # Shell wrapper for command interception
├── commands/               # Command database (JSON files)
├── .termaid_history.json   # Command history (auto-generated)
├── ReadMe.md              # This file
└── bin/                   # Legacy C++ binary (deprecated)
```

## History File

TermAid maintains a history file at `.termaid_history.json` containing:
- Timestamp of execution
- Full command string
- Explanation (if available)
- Execution status
- Exit code

The history is automatically limited to the last 1000 entries.

## Tips and Best Practices

1. **Use search liberally** - If you're unsure about a command, search for it first
2. **Review explanations** - Always read the explanation before executing unfamiliar commands
3. **Check history** - Review your command history to learn patterns
4. **Contribute** - Add commands you use frequently to help others

## Troubleshooting

### Command not found in database
- The command might not be in the database yet - consider contributing it!
- Try searching with different keywords
- Use `man <command>` or `<command> --help` as fallback

### Python version issues
- Ensure you're using Python 3.6 or higher: `python3 --version`

### Permission denied
- Make sure the script is executable: `chmod +x termaid.py`
- Some commands require sudo privileges

## License

This project is open source. Feel free to use, modify, and distribute.

## Acknowledgments

- Original concept by DevLouix
- Enhanced and expanded with comprehensive command database
- Community contributions welcome!

## Support

For issues, questions, or suggestions:
- Open an issue on GitHub
- Submit a pull request
- Check existing documentation

---

**Happy Learning! 🚀**

Remember: Understanding commands before running them makes you a better system administrator!