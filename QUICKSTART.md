# TermAid Quick Start Guide

Get started with TermAid in 2 minutes!

## Installation

```bash
# Clone the repository
git clone https://github.com/DevLouix/TermAid.git
cd TermAid

# Make executable
chmod +x termaid.py

# Test it works
./termaid.py list
```

## Your First Commands

### 1. Search for a command
```bash
./termaid.py search copy
```
**Output:** Shows all copy-related commands (cp, scp, rsync, etc.)

### 2. Explain a command before using it
```bash
./termaid.py explain "tar -czf backup.tar.gz /home"
```
**Output:** Detailed explanation of what this tar command does

### 3. Execute with safety check
```bash
./termaid.py exec "ls -la"
```
**Output:** Shows explanation, asks for confirmation, then executes

### 4. View your command history
```bash
./termaid.py history
```
**Output:** Shows all commands you've run through TermAid

## Common Use Cases

### Learning a new command
```bash
# Don't know how to compress files?
./termaid.py search compress

# Want to know what a command does?
./termaid.py explain "chmod 755 script.sh"
```

### Safe execution
```bash
# Not sure if this command is safe?
./termaid.py exec "rm -rf /tmp/old_files"
# Shows explanation and asks for confirmation
```

### Quick reference
```bash
# Forgot the syntax?
./termaid.py search network
./termaid.py search permission
./termaid.py search process
```

## Pro Tips

1. **Add to PATH** for easier access:
   ```bash
   sudo ln -s $(pwd)/termaid.py /usr/local/bin/termaid
   # Now use: termaid search copy
   ```

2. **Create an alias** in your `.bashrc` or `.zshrc`:
   ```bash
   alias ta='./termaid.py'
   # Now use: ta search copy
   ```

3. **Use tab completion** - Most shells support it for arguments

4. **Check history regularly** to learn from your command patterns

## What's Available?

TermAid includes **140+ commands** across:
- File operations (ls, cp, mv, rm, cat, etc.)
- System info (df, free, top, ps, etc.)
- Networking (ping, ssh, curl, wget, etc.)
- Text processing (grep, sed, awk, etc.)
- Package management (apt, yum, dnf, etc.)
- And much more!

See full list: `./termaid.py list`

## Need Help?

```bash
./termaid.py --help
```

## Next Steps

1. Read the full [README.md](ReadMe.md) for advanced features
2. Try the shell wrapper mode for automatic command explanation
3. Contribute new commands to help others!

---

**You're ready to go! Start exploring Linux commands with confidence! 🚀**
