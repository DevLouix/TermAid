# Contributing to TermAid

Thank you for your interest in contributing to TermAid! This guide will help you add new commands and improve the project.

## How to Contribute

### 1. Adding New Commands

Commands are stored in JSON files under the `commands/` directory, organized by category.

#### Step 1: Choose the Right Category

```
commands/
├── file_operations/     # File and directory commands
├── system_info/         # System information commands
├── network/             # Network-related commands
├── process_management/  # Process and service management
├── text_processing/     # Text manipulation commands
├── permissions/         # Permission and user management
├── package_management/  # Package managers (apt, yum, etc.)
├── archive/             # Compression and archiving
├── disk_management/     # Disk and filesystem commands
└── search/              # File search commands
```

If your command doesn't fit any category, create a new one!

#### Step 2: JSON Format

Each command entry should follow this format:

```json
{
  "command_name": [
    {
      "response": "command <arg1> <arg2>",
      "explanation": "Clear, concise explanation of what the command does",
      "example": "command example value"
    },
    {
      "response": "command -flag <arg>",
      "explanation": "Explanation of this variant",
      "example": "command -flag example"
    }
  ]
}
```

**Fields:**
- `response` (required): The command syntax with placeholders in `<angle brackets>`
- `explanation` (required): What the command does (be clear and helpful!)
- `example` (optional but recommended): A real-world example

#### Step 3: Example - Adding a New Command

Let's add the `tree` command to `commands/file_operations/basic.json`:

```json
{
  "tree": [
    {
      "response": "tree",
      "explanation": "Display directory structure in tree format"
    },
    {
      "response": "tree -L <depth>",
      "explanation": "Limit tree depth to specified number of levels",
      "example": "tree -L 2"
    },
    {
      "response": "tree -a",
      "explanation": "Show hidden files in tree",
      "example": "tree -a"
    },
    {
      "response": "tree -d",
      "explanation": "Show directories only",
      "example": "tree -d"
    }
  ]
}
```

#### Step 4: Test Your Addition

```bash
# Test search
./termaid.py search tree

# Test explanation
./termaid.py explain "tree -L 2"

# Verify it appears in the list
./termaid.py list | grep tree
```

### 2. Improving Existing Commands

You can enhance existing commands by:

1. **Adding more examples**
   ```json
   {
     "response": "grep <pattern> <file>",
     "explanation": "Search for pattern in file",
     "example": "grep 'error' logfile.txt"
   }
   ```

2. **Clarifying explanations**
   - Make them more beginner-friendly
   - Add warnings for dangerous commands
   - Include common use cases

3. **Adding common flags and options**
   - Include frequently used variations
   - Document important options

### 3. Code Contributions

#### Areas for Improvement

1. **Command Parsing**
   - Better argument detection
   - Support for piped commands
   - Handle complex command chains

2. **Matching Algorithm**
   - Improve fuzzy matching
   - Better flag detection
   - Context-aware suggestions

3. **Features**
   - Command aliases support
   - Interactive tutorial mode
   - Command recommendations
   - Shell integration improvements

4. **Documentation**
   - More examples
   - Video tutorials
   - Translations

#### Development Setup

```bash
# Clone your fork
git clone https://github.com/YOUR_USERNAME/TermAid.git
cd TermAid

# Create a branch
git checkout -b feature/your-feature-name

# Make changes and test
./termaid.py list
./termaid.py search your_command

# Commit with clear messages
git commit -m "Add: tree command with examples"

# Push and create PR
git push origin feature/your-feature-name
```

### 4. Writing Good Explanations

#### Do's ✅
- Be clear and concise
- Use simple language
- Include practical examples
- Mention important flags
- Warn about dangerous operations
- Explain what the command achieves, not just what it does

#### Don'ts ❌
- Don't use jargon without explanation
- Don't make assumptions about user knowledge
- Don't write overly technical explanations
- Don't forget examples for complex commands

#### Examples

**Good:**
```json
{
  "response": "rm -rf <directory>",
  "explanation": "Force remove directory recursively (use with caution! This permanently deletes everything)",
  "example": "rm -rf old_folder"
}
```

**Bad:**
```json
{
  "response": "rm -rf <directory>",
  "explanation": "Removes directory",
  "example": "rm -rf /"
}
```

### 5. Command Categories Guide

#### File Operations
- File creation, deletion, copying, moving
- Directory operations
- File viewing and editing
- Links and permissions

#### System Info
- System status and information
- Hardware information
- Environment variables
- User information

#### Network
- Network configuration
- Remote connections
- File transfers
- Network diagnostics

#### Process Management
- Process viewing and control
- Service management
- Background jobs
- System services

#### Text Processing
- Text search and manipulation
- File comparison
- Stream editing
- Text transformation

#### Permissions
- File permissions
- User switching
- Privilege escalation
- Ownership changes

#### Package Management
- Package installation/removal
- System updates
- Package search
- Repository management

#### Archive/Compression
- File compression
- Archive creation/extraction
- Multiple compression formats

#### Disk Management
- Disk partitioning
- Filesystem operations
- Mounting/unmounting
- Disk usage

#### Search
- File finding
- Command location
- Pattern searching

### 6. Submission Guidelines

1. **Test thoroughly**
   - Verify JSON syntax is valid
   - Test search functionality
   - Test explanation display
   - Check for typos

2. **Follow existing patterns**
   - Match the style of existing entries
   - Use consistent formatting
   - Follow naming conventions

3. **One command per PR** (for new commands)
   - Makes review easier
   - Faster to merge
   - Easier to track changes

4. **Write clear commit messages**
   ```
   Add: tree command with depth and filter options
   Fix: typo in grep explanation
   Improve: tar command examples
   ```

### 7. Review Process

1. Submit your PR
2. Maintainers will review within a few days
3. Address any feedback
4. Once approved, your contribution will be merged!

### 8. Getting Help

- Open an issue for questions
- Check existing commands for examples
- Ask in discussions
- Review this guide

## Command Priority List

High priority commands to add:
- [ ] `screen` - Terminal multiplexer
- [ ] `tmux` - Terminal multiplexer
- [ ] `iptables` - Firewall configuration
- [ ] `cron` - Job scheduler
- [ ] `at` - One-time job scheduler
- [ ] `journalctl` - Systemd logs
- [ ] `docker` - Container management
- [ ] `git` - Version control
- [ ] `make` - Build automation
- [ ] `awk` - Advanced text processing (more examples)

## Recognition

Contributors will be:
- Listed in the README
- Credited in release notes
- Part of making Linux more accessible!

## Code of Conduct

- Be respectful and inclusive
- Help others learn
- Provide constructive feedback
- Focus on making TermAid better for everyone

---

**Thank you for contributing to TermAid! Every command you add helps someone learn Linux better! 🚀**
