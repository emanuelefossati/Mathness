# Mathness

Mathness is a lightweight REPL (Read-Eval-Print Loop) application written in C++ for solving mathematical expressions interactively.  
It supports scalar and matrix operations, elementary math functions (e.g. trigonometric, logarithmic, exponential), and basic aggregation functions.

You can define variables, reuse them in expressions, and perform computations step by step in a simple command-line interface.

---

### Features

- **Scalar Operations**:
  - `a + b`
  - `a - b`
  - `-b`
  - `a * b`
  - `a / b`
  - `a % b`
  - `a ^ b`
- **Matrix Operations**:
  - `M1 + M2`
  - `M1 - M2`
  - `-M1` (meant as negation of each element in the matrix)
  - `M1 * M2`
  - `a * M` (and vice versa)
  - `M1 / M2` (meant as the product of the first matrix with the inverse of the latter one )
  - `M ^ a` (meant as the iterated product of a square matrix with itself)
- **Math functions**
  - `sqrt(a)`
  - `exp(a)`
  - `ln(a)`
  - `abs(a)`
  - `abs(M)` (meant as the Frobenius norm of the matrix)
  - `fact(a)` (only for integer values)
  - `log(a, b)` (log of b to base a)
- **Trigonometric functions**
  - `sin(a)`
  - `cos(a)`
  - `tan(a)`
  - `asin(a)`
  - `acos(a)`
  - `atan(a)`
- **Vectorial functions**
  both only valid for one-dimensional matrices (a.k.a vectors):

  - `dot(V1, V2)` (scalar product)
  - `cross(V1, V2)` (vectorial product, only defined for vectors with size 3)
  - `abs(V)` (just the classic norm of a vector)

- **Aggregation Operations**
  operations performed on lists of (weighted) scalar values

  - `max(L)`
  - `min(L)`
  - `count(L)`
  - `sum(L)`
  - `avg(L)`
  - `var(L)`
  - `distinct(L)` (returns a version of the original list without duplicated elements)

---
