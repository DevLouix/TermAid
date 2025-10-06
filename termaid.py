#!/usr/bin/env python3
"""
TermAid - Terminal Command Aid and Logger
A comprehensive tool to log, explain, and help with Linux commands
"""

import os
import sys
import json
import subprocess
import argparse
from datetime import datetime
from pathlib import Path
import shlex

class TermAid:
    def __init__(self):
        self.base_dir = Path(__file__).parent
        self.commands_dir = self.base_dir / "commands"
        self.history_file = self.base_dir / ".termaid_history.json"
        self.aliases_file = self.base_dir / ".termaid_aliases.json"
        self.command_db = {}
        self.aliases = {}
        self.load_commands()
        self.load_aliases()
    
    def load_commands(self):
        """Load all command definitions from JSON files"""
        if not self.commands_dir.exists():
            self.commands_dir.mkdir(parents=True)
            return
        
        for json_file in self.commands_dir.rglob("*.json"):
            try:
                with open(json_file, 'r') as f:
                    data = json.load(f)
                    self.command_db.update(data)
            except json.JSONDecodeError:
                print(f"Warning: Could not parse {json_file}", file=sys.stderr)
            except Exception as e:
                print(f"Warning: Error loading {json_file}: {e}", file=sys.stderr)
    
    def load_aliases(self):
        """Load command aliases"""
        if self.aliases_file.exists():
            try:
                with open(self.aliases_file, 'r') as f:
                    self.aliases = json.load(f)
            except:
                self.aliases = {}
        else:
            # Default common aliases
            self.aliases = {
                "ll": "ls -la",
                "la": "ls -A",
                "l": "ls -CF",
                "..": "cd ..",
                "...": "cd ../..",
                "grep": "grep --color=auto",
                "egrep": "egrep --color=auto",
                "fgrep": "fgrep --color=auto"
            }
    
    def save_aliases(self):
        """Save aliases to file"""
        with open(self.aliases_file, 'w') as f:
            json.dump(self.aliases, f, indent=2)
    
    def resolve_alias(self, command_str):
        """Resolve command alias to actual command"""
        parts = command_str.split()
        if parts and parts[0] in self.aliases:
            resolved = self.aliases[parts[0]]
            if len(parts) > 1:
                resolved += ' ' + ' '.join(parts[1:])
            return resolved
        return command_str
    
    def log_command(self, command, explanation=None, executed=False, exit_code=None):
        """Log a command to history"""
        history = []
        if self.history_file.exists():
            try:
                with open(self.history_file, 'r') as f:
                    history = json.load(f)
            except:
                history = []
        
        entry = {
            "timestamp": datetime.now().isoformat(),
            "command": command,
            "explanation": explanation,
            "executed": executed,
            "exit_code": exit_code
        }
        
        history.append(entry)
        
        # Keep only last 1000 entries
        if len(history) > 1000:
            history = history[-1000:]
        
        with open(self.history_file, 'w') as f:
            json.dump(history, f, indent=2)
    
    def parse_command(self, command_str):
        """Parse a command string and extract the base command"""
        # Resolve aliases first
        command_str = self.resolve_alias(command_str)
        
        # Handle piped commands
        if '|' in command_str:
            return self.parse_piped_command(command_str)
        
        try:
            parts = shlex.split(command_str)
            if not parts:
                return None, []
            
            base_cmd = parts[0]
            # Handle sudo, time, etc.
            if base_cmd in ['sudo', 'time', 'nice', 'nohup']:
                if len(parts) > 1:
                    base_cmd = parts[1]
                    args = parts[2:]
                else:
                    args = []
            else:
                args = parts[1:]
            
            return base_cmd, args
        except:
            # Fallback for complex commands
            return command_str.split()[0] if command_str.split() else None, []
    
    def parse_piped_command(self, command_str):
        """Parse piped commands and return first command"""
        pipe_parts = command_str.split('|')
        first_cmd = pipe_parts[0].strip()
        try:
            parts = shlex.split(first_cmd)
            if not parts:
                return None, []
            return parts[0], parts[1:]
        except:
            return first_cmd.split()[0] if first_cmd.split() else None, []
    
    def explain_piped_command(self, command_str):
        """Explain each part of a piped command"""
        pipe_parts = [p.strip() for p in command_str.split('|')]
        
        print(f"\n{'='*60}")
        print(f"Piped Command: {command_str}")
        print(f"{'='*60}\n")
        print(f"This command has {len(pipe_parts)} parts connected by pipes:\n")
        
        for i, part in enumerate(pipe_parts, 1):
            print(f"Part {i}: {part}")
            explanation = self.get_command_explanation(part)
            if explanation:
                print(f"  → {explanation.get('explanation', 'No explanation available')}")
            else:
                base_cmd, _ = self.parse_command(part)
                print(f"  → Command: {base_cmd} (no detailed explanation available)")
            print()
        
        print("How pipes work:")
        print("  • Each command's output becomes the next command's input")
        print("  • Data flows left to right through the pipeline")
        print(f"{'='*60}\n")
    
    def get_command_explanation(self, command_str):
        """Get explanation for a command"""
        base_cmd, args = self.parse_command(command_str)
        
        if not base_cmd:
            return None
        
        best_match = None
        best_score = 0
        
        # Check if we have this command in our database
        for key, cmd_list in self.command_db.items():
            if not isinstance(cmd_list, list):
                continue
            
            for cmd_info in cmd_list:
                response = cmd_info.get('response', '')
                response_parts = response.split()
                
                if not response_parts or response_parts[0] != base_cmd:
                    continue
                
                # Calculate match score
                score = 1  # Base score for matching command
                
                # Check if response matches our arguments
                if len(response_parts) > 1:
                    response_flags = [p for p in response_parts[1:] if p.startswith('-') and '<' not in p]
                    arg_flags = [a for a in args if a.startswith('-')]
                    
                    # Exact flag match
                    if response_flags and arg_flags:
                        if set(response_flags) == set(arg_flags):
                            score += 10
                        elif any(f in arg_flags for f in response_flags):
                            score += 5
                    
                    # Match number of arguments
                    if len(args) > 0 and '<' in response:
                        score += 2
                
                # Prefer exact matches
                if response == command_str:
                    score += 20
                
                # Prefer entries without placeholders if we have no args
                if not args and '<' not in response:
                    score += 3
                
                if score > best_score:
                    best_score = score
                    best_match = cmd_info
        
        return best_match
    
    def explain_command(self, command_str):
        """Explain a command to the user"""
        # Check if it's a piped command
        if '|' in command_str:
            self.explain_piped_command(command_str)
            return
        
        # Resolve aliases
        resolved_cmd = self.resolve_alias(command_str)
        if resolved_cmd != command_str:
            print(f"\nAlias resolved: {command_str} → {resolved_cmd}\n")
            command_str = resolved_cmd
        
        base_cmd, args = self.parse_command(command_str)
        
        if not base_cmd:
            print("Could not parse command")
            return
        
        print(f"\n{'='*60}")
        print(f"Command: {command_str}")
        print(f"{'='*60}")
        
        explanation = self.get_command_explanation(command_str)
        
        if explanation:
            print(f"\nExplanation: {explanation.get('explanation', 'No explanation available')}")
            if 'example' in explanation:
                print(f"Example: {explanation['example']}")
        else:
            print(f"\nBase command: {base_cmd}")
            print("No detailed explanation available in database.")
            print("This command may be:")
            print("  - A system binary")
            print("  - A shell builtin")
            print("  - A custom script or alias")
            print(f"\nTry: man {base_cmd} or {base_cmd} --help")
        
        if args:
            print(f"\nArguments: {' '.join(args)}")
        
        print(f"{'='*60}\n")
    
    def search_commands(self, keyword):
        """Search for commands matching a keyword"""
        results = []
        keyword_lower = keyword.lower()
        
        for key, cmd_list in self.command_db.items():
            if not isinstance(cmd_list, list):
                continue
            
            if keyword_lower in key.lower():
                results.extend(cmd_list)
            else:
                for cmd_info in cmd_list:
                    if (keyword_lower in cmd_info.get('response', '').lower() or
                        keyword_lower in cmd_info.get('explanation', '').lower()):
                        results.append(cmd_info)
        
        return results
    
    def display_search_results(self, keyword):
        """Display search results for a keyword"""
        results = self.search_commands(keyword)
        
        if not results:
            print(f"No commands found for: {keyword}")
            return
        
        print(f"\nTerminal commands for '{keyword}':")
        print("="*60)
        
        for cmd_info in results:
            print(f"\nCommand: {cmd_info.get('response', 'N/A')}")
            print(f"Explanation: {cmd_info.get('explanation', 'N/A')}")
            if 'example' in cmd_info:
                print(f"Example: {cmd_info['example']}")
            print("-"*60)
    
    def execute_with_logging(self, command_str, explain_first=True):
        """Execute a command with logging"""
        if explain_first:
            self.explain_command(command_str)
            response = input("\nExecute this command? [Y/n]: ").strip().lower()
            if response and response not in ['y', 'yes']:
                print("Command execution cancelled.")
                self.log_command(command_str, executed=False)
                return None
        
        print(f"\nExecuting: {command_str}\n")
        
        try:
            result = subprocess.run(
                command_str,
                shell=True,
                capture_output=False,
                text=True
            )
            
            explanation = self.get_command_explanation(command_str)
            exp_text = explanation.get('explanation', '') if explanation else None
            
            self.log_command(command_str, exp_text, executed=True, exit_code=result.returncode)
            
            return result.returncode
        except Exception as e:
            print(f"Error executing command: {e}", file=sys.stderr)
            self.log_command(command_str, executed=True, exit_code=-1)
            return -1
    
    def show_history(self, limit=20):
        """Show command history"""
        if not self.history_file.exists():
            print("No command history found.")
            return
        
        try:
            with open(self.history_file, 'r') as f:
                history = json.load(f)
        except:
            print("Could not read history file.")
            return
        
        print(f"\nCommand History (last {limit} entries):")
        print("="*80)
        
        for entry in history[-limit:]:
            timestamp = entry.get('timestamp', 'N/A')
            command = entry.get('command', 'N/A')
            executed = entry.get('executed', False)
            exit_code = entry.get('exit_code')
            
            status = "✓" if executed and exit_code == 0 else "✗" if executed else "○"
            print(f"{status} [{timestamp}] {command}")
            if entry.get('explanation'):
                print(f"   → {entry['explanation']}")
        
        print("="*80)
    
    def list_available_commands(self):
        """List all available commands in the database"""
        print("\nAvailable Commands in Database:")
        print("="*60)
        
        all_commands = set()
        for key, cmd_list in self.command_db.items():
            if not isinstance(cmd_list, list):
                continue
            
            for cmd_info in cmd_list:
                response = cmd_info.get('response', '')
                if response:
                    base = response.split()[0]
                    all_commands.add(base)
        
        for cmd in sorted(all_commands):
            print(f"  • {cmd}")
        
        print(f"\nTotal: {len(all_commands)} commands")
        print("="*60)
    
    def show_man_page(self, command):
        """Show man page for command"""
        base_cmd, _ = self.parse_command(command)
        if not base_cmd:
            print("Could not parse command")
            return
        
        print(f"\nOpening man page for: {base_cmd}\n")
        try:
            subprocess.run(['man', base_cmd])
        except FileNotFoundError:
            print(f"Man page not available for {base_cmd}")
            print(f"Try: {base_cmd} --help")
        except Exception as e:
            print(f"Error opening man page: {e}")
    
    def manage_aliases(self, action, alias=None, command=None):
        """Manage command aliases"""
        if action == 'list':
            if not self.aliases:
                print("No aliases defined.")
                return
            
            print("\nDefined Aliases:")
            print("="*60)
            for alias, cmd in sorted(self.aliases.items()):
                print(f"  {alias:15} → {cmd}")
            print("="*60)
        
        elif action == 'add':
            if not alias or not command:
                print("Error: Both alias and command required")
                return
            
            self.aliases[alias] = command
            self.save_aliases()
            print(f"✓ Alias added: {alias} → {command}")
        
        elif action == 'remove':
            if not alias:
                print("Error: Alias name required")
                return
            
            if alias in self.aliases:
                del self.aliases[alias]
                self.save_aliases()
                print(f"✓ Alias removed: {alias}")
            else:
                print(f"Alias not found: {alias}")
        
        elif action == 'show':
            if not alias:
                print("Error: Alias name required")
                return
            
            if alias in self.aliases:
                print(f"{alias} → {self.aliases[alias]}")
            else:
                print(f"Alias not found: {alias}")


def main():
    parser = argparse.ArgumentParser(
        description="TermAid - Terminal Command Aid and Logger",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
  termaid.py search copy          # Search for copy-related commands
  termaid.py explain "ls -la"     # Explain a command
  termaid.py exec "ls -la"        # Execute with explanation and logging
  termaid.py history              # Show command history
  termaid.py list                 # List all available commands
  termaid.py man ls               # Show man page for command
  termaid.py alias list           # List all aliases
  termaid.py alias add ll "ls -la" # Add alias
        """
    )
    
    parser.add_argument('action', 
                       choices=['search', 'explain', 'exec', 'history', 'list', 'man', 'alias'],
                       help='Action to perform')
    parser.add_argument('command', nargs='*', help='Command or keyword')
    parser.add_argument('--no-confirm', action='store_true',
                       help='Execute without confirmation')
    parser.add_argument('--limit', type=int, default=20,
                       help='Number of history entries to show')
    
    args = parser.parse_args()
    
    termaid = TermAid()
    
    if args.action == 'search':
        if not args.command:
            print("Error: Please provide a keyword to search")
            sys.exit(1)
        keyword = ' '.join(args.command)
        termaid.display_search_results(keyword)
    
    elif args.action == 'explain':
        if not args.command:
            print("Error: Please provide a command to explain")
            sys.exit(1)
        command = ' '.join(args.command)
        termaid.explain_command(command)
    
    elif args.action == 'exec':
        if not args.command:
            print("Error: Please provide a command to execute")
            sys.exit(1)
        command = ' '.join(args.command)
        exit_code = termaid.execute_with_logging(command, explain_first=not args.no_confirm)
        sys.exit(exit_code if exit_code is not None else 0)
    
    elif args.action == 'history':
        termaid.show_history(args.limit)
    
    elif args.action == 'list':
        termaid.list_available_commands()
    
    elif args.action == 'man':
        if not args.command:
            print("Error: Please provide a command")
            sys.exit(1)
        command = ' '.join(args.command)
        termaid.show_man_page(command)
    
    elif args.action == 'alias':
        if not args.command:
            termaid.manage_aliases('list')
        elif args.command[0] == 'list':
            termaid.manage_aliases('list')
        elif args.command[0] == 'add' and len(args.command) >= 3:
            alias_name = args.command[1]
            alias_cmd = ' '.join(args.command[2:])
            termaid.manage_aliases('add', alias_name, alias_cmd)
        elif args.command[0] == 'remove' and len(args.command) >= 2:
            termaid.manage_aliases('remove', args.command[1])
        elif args.command[0] == 'show' and len(args.command) >= 2:
            termaid.manage_aliases('show', args.command[1])
        else:
            print("Usage: termaid alias [list|add <name> <command>|remove <name>|show <name>]")


if __name__ == "__main__":
    main()
