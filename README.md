# mystring-core
### High-Performance Dynamic String Management for C

## 💡 Overview
C strings are notoriously difficult to manage safely. `mystring-core` is a standalone library built to provide a "Java-like" string experience in C, focusing on manual heap optimization and safety.

## 🛠️ Key Features
- **Optimized Concatenation:** Uses `memmove` and `realloc` to manage buffers efficiently, minimizing fragmentation.
- **Advanced Trimming:** Custom algorithms to handle whitespace and special characters at the byte level.
- **Reference Guarding:** Designed with a focus on preventing "off-by-one" errors and buffer overflows.

## 🏗️ Build System
This library uses a specialized **Makefile** designed for:
- Static library generation (`.a`).
- Automated memory leak testing via **Valgrind**.
- Modular compilation with **GCC/Clang**.

## 🎯 Technical Origins
Developed as a deep-dive into C memory models. By building this separately from my utility suite, I was able to focus strictly on **Memory Pointers** and **Heap Allocation** without outside dependencies.
