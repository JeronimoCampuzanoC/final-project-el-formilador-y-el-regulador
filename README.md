# 📌 Project README
```

 ▄█        ▄█             ▄█                  
███       ███            ███                  
███       ███            ███▌                 
███       ███            ███▌                 
███       ███            ███▌                 
███       ███            ███                  
███▌    ▄ ███▌    ▄      ███                  
█████▄▄██ █████▄▄██      █▀                   
▀         ▀                                   
   ▄████████  ▄█          ▄████████       ▄█  
  ███    ███ ███         ███    ███      ███  
  ███    █▀  ███         ███    ███      ███▌ 
  ███        ███        ▄███▄▄▄▄██▀      ███▌ 
▀███████████ ███       ▀▀███▀▀▀▀▀        ███▌ 
         ███ ███       ▀███████████      ███  
   ▄█    ███ ███▌    ▄   ███    ███      ███  
 ▄████████▀  █████▄▄██   ███    ███      █▀   
             ▀           ███    ███           
```
# 🧾 Table of Contents

1. [📖 Description](#-description)
2. [📚 Class Information](#-class-information)
3. [👨‍🏫 Teacher](#-teacher)
4. [👨‍🎓 Students](#-students)
5. [💻 System Information](#-system-information)
6. [🚀 Running](#-running)
    - [Compilation](#compilation)
    - [Execution](#execution)
## 📖 Description
# Final Project: El Formilador y El Regulador

This project implements a parser for formal languages, focusing on **LL(1)** and **SLR(1)** grammars. It provides tools to analyze a grammar, generate parsing tables, and validate strings against the grammar. The project is designed for the implementation of parsing algorithms and grammar analysis.

## Features

- **Grammar Parsing**: Reads grammar rules from a text file and processes them into a usable format.
- **LL(1) Parsing**:
  - Computes First and Follow sets.
  - Generates the LL(1) parsing table.
  - Validates strings against the grammar.
  - Detects if a grammar is LL(1) compliant.
- **SLR(1) Parsing**:
  - Augments the grammar with a start rule.
  - Computes First and Follow sets.
  - Implements state closures and transitions (Goto function).
  - Generates the SLR(1) parsing table.
  - Validates strings against the grammar.
  - Detects conflicts in the SLR(1) table.
- **Error Handling**: Identifies and reports issues such as invalid grammars or parsing conflicts.
- **Debugging Tools**: Prints parsing tables and states for debugging and analysis.

## Input Format

The grammar is provided in a text file with the following format:
1. The first line specifies the number of production rules.
2. Each subsequent line defines a production in the form `NonTerminal -> Production`.

Example input file:

## 📚 Class Information
- **Class Number:** 7308  

## 👨‍🏫 Teacher
- **Adolfo Andrés Castro Sánchez** 

## 👨‍🎓 Students
- **Jeronimo Campuzano Castaño** 
- **Andres Perez Quinchia**  

## 💻 System Information
- **OS:** Ubuntu 24.04.2 LTS x86_64  
- **Programming Language:**C++ 
- **Compiler Version:** g++ (Ubuntu 13.3.0-6ubuntu2~24.04) 13.3.0
- **Build System Generator:** CMake 3.28.1
- **Debugger Version:** GNU gdb (Ubuntu 15.0.50.20240403-0ubuntu1 15.0.50.20240403-git

---

## 🚀 Running 
1. Clone the repository.
2. Execute the execute.sh file that will automatize all the procces, from building the folder, configuring it, compile the source and executing the project
```bash
  mkdir -p build
  cd build
  cmake ..
  cmake --build .
  ./FinalProject
```


