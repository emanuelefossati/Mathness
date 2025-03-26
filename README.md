# Mathness

Mathness is a lightweight REPL (Read-Eval-Print Loop) application written in C++ for solving mathematical expressions interactively.  
It supports scalar and matrix operations, elementary math functions (e.g. trigonometric, logarithmic, exponential), and basic aggregation functions.

You can define variables, reuse them in expressions, and perform computations step by step in a simple command-line interface.

---

## Features

### **Scalar Operations**

- `a + b` (addition)
- `a - b` (subtraction)
- `-a` (negation)
- `a * b` (multiplication)
- `a / b` (division)
- `a % b` (modulo, only for integers)
- `a ^ b` (exponentiation)

### **Matrix Operations**

- `M1 + M2` (element-wise addition)
- `M1 - M2` (element-wise subtraction)
- `-M1` (negation of each element)
- `M1 * M2` (matrix multiplication)
- `a * M` (scalar-matrix multiplication)
- `M1 / M2` (multiplication of `M1` with the inverse of `M2`)
- `M ^ a` (matrix exponentiation, only for square matrices)

### **Mathematical Functions**

- `sqrt(a)` (square root)
- `exp(a)` (exponential function)
- `ln(a)` (natural logarithm)
- `abs(a)` (absolute value)
- `abs(M)` (Frobenius norm of matrix `M`)
- `fact(a)` (factorial, only for integers)
- `log(a, b)` (logarithm of `b` to base `a`)

### **Trigonometric Functions**

- `sin(a)`, `cos(a)`, `tan(a)`
- `asin(a)`, `acos(a)`, `atan(a)`

### **Vector Functions**

(Only applicable to **vectors**, i.e., one-dimensional matrices)

- `dot(V1, V2)` (dot product)
- `cross(V1, V2)` (cross product, only for 3D vectors)
- `abs(V)` (Euclidean norm of vector `V`)

### **Aggregation Functions**

(Operations performed on lists of scalar values)

- `max(L)`, `min(L)` (maximum and minimum)
- `count(L)` (number of elements)
- `sum(L)`, `avg(L)`, `var(L)` (sum, average, variance)
- `distinct(L)` (removes duplicate elements from the list)

### **Storage Operations**

- `var = a`, `var = L`, `var = M` – Define or overwrite a stored variable (`var`) with a scalar (`a`), list (`L`), or matrix (`M`).
- `var`, `var[i]`, `var[i][j]` – Retrieve stored values:
  - `var` retrieves the entire value.
  - `var[i]` accesses the `i`-th element (for lists and vectors).
  - `var[i][j]` accesses the element at row `i`, column `j` (for matrices).
    **Convention**: Indexing starts at 0 (zero-based indexing).

---

## Installation

### Prerequisites

- **C++ Compiler**: A modern C++ compiler that supports C++23.
- **CMake**: Version 3.10 or higher.

### Steps to Install

1. Clone the repository:

```bash
git clone https://github.com/emanuelefossati/Mathness.git
cd Mathness
```

2. Clone vcpkg inside the root of the project:

```bash
git clone https://github.com/microsoft/vcpkg.git
```

3. Configure the project with CMAKE

```bash
cmake -B build -DCMAKE_TOOLCHAIN_FILE=./vcpkg/scripts/buildsystems/vcpkg.cmake
```

4. Compile the project

```bash
cmake --build build
```

5. After building the project, you can run the executable from the terminal:

##### On Windows:

```bash
cd build/Debug  # or build/Release if built in Release mode
.\Mathness.exe
```

##### On Linux/MacOS

```bash
cd build
./Mathness
```

**Note**: If you're using cmd.exe or PowerShell, ANSI color codes may not display correctly.
For the best experience, use Windows Terminal or another ANSI-compatible terminal.
