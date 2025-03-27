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

---

### Examples

**Scalar Operations**

```
>>> 5 + 3
8
>>> -5 * (4 - 8)
20
>>> log(2, 64)
6
>>> -sin(pi/2)
-1
>>> 2 * ln(e ^ 3)
6
>>> fact(5)
120
```

**Matrix Operations**

```
>>> M1 = [1, 2; 3, 4] # Matrix declaration
1       2
3       4
>>> M1 * 3 # Matrix-scalar product
3       6
9       12
>>> M2 = [1 + 2, 3 - 4; 5 * 6, 7 / 8] # Each element inside can hold an expression
3       -1
30      0.875
```

**Vector Operations**

```
>>> V1 = [1, 2, 3]
1       2       3
>>> V2 = [4, 5, 6]
4       5       6
>>> dot(V1, V2)
32
>>> cross(V1, V2)
-3      6       -3
```

**List Operations**

```
>>> L1 = {1, 8; 20, 2} # List declaration
{1|8; 20|2} # Notation: {value | weight}

>>> {1 + 9; 2 * 3; 3 / 6; 4 ^ 0} # If you omit weight, by default it's 1
{10|1; 6|1; 0.5|1; 1|1} # {value | weight, by default is 1}
```

**Storage Operations**

```
>>> a = {1; 2; 3; 4}
{1|1; 2|1; 3|1; 4|1}
>>> a[1]
2
>>> b = [1, 2; 0, 4]
1       2
0       4
>>> b[1][0]
0
>>> b[1][0] = 3
3
>>> b
1       2
3       4
```

---

### Acknowledgments
