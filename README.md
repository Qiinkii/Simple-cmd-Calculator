# Simple CMD Calculator

A powerful command-line interface (CLI) calculator written in C++. It parses and evaluates mathematical expressions using the Shunting-yard algorithm, supporting proper operator precedence, floating-point numbers, and parentheses.

## Features
- Expression Evaluation: Handles `+`, `-`, `*`, `/`, and parentheses `()`.
- Float Support: Accurately calculates decimal values.
- File Processing: Reads and calculates mathematical expressions line-by-line from a text file.
- Comment Ignoring: Safely ignores everything after a `#` symbol in strings or files.
- Verbose Mode: Logs the internal stack operations and calculation steps.

## Usage
Compile the project (for example, using g++):
```bash
g++ main.cpp -o calc