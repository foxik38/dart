# dart
![CI Status](https://github.com/foxik38/dart/actions/workflows/ci.yml/badge.svg)
![C++23](https://img.shields.io/badge/C%2B%2B-23-blue.svg?logo=c%2B%2B)
![License](https://img.shields.io/github/license/foxik38/dart)
![GitHub last commit](https://img.shields.io/github/last-commit/foxik38/dart)
___
Dart is a minimalist yet high-performance database engine written in C++.
The main purpose of this project is to explore and study core systems programming concepts — namely
**Kernel system calls, modern C++ and strict memory management.**

### Advantages
- **Easy to use** ― A key feature of this project is its simplicity when it comes to usage.
It doesn't matter whether you are a beginner or advanced in C++, I'm pretty sure you will
love using Dart!
- **Performance** ― Dart is incredibly fast due to its zero-copy and zero-cost architecture
and cache-friendly design making it a performance beast.
- **Open source** ― Licensed under MIT License, Dart is available for everyone to use
as well as contribute to!

### Features
- Hardware aligned paging and cache compliance
- Zero-copy layout with minimal heap footprint
- Compile-time safety type and value safety
- Kernel system calls usage (`mmap(2)`, `munmap(2)`, `open(2)`, `close(2)` and more)
- Continuous Integration using GitHub Actions

**Planned** ― Table encryption, database query language, CLI mode, remote connection, query builder.