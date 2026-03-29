# 📐 Vector Mathematics & Dynamic Arrays

> A C-based linear algebra engine built on top of dynamically allocated float arrays. It safely handles memory management while performing complex mathematical operations on multidimensional vectors.

---

## ✨ Key Features & Technical Highlights

This project combines low-level memory management with high-level mathematics, heavily utilizing the `<math.h>` standard library.

* **Linear Algebra Engine:** Implements core vector operations including Addition, Subtraction, Dot Product, Magnitude (Length), Angle calculation, and Normalization.
* **Strict Dimensionality Checks:** Before performing any operations between two vectors (like addition or dot product), the program strictly verifies that both vectors have the exact same dimensionality (`a->size == b->size`).
* **Safe Math Execution:** Prevents mathematical anomalies such as division by zero during normalization or taking the arccosine of invalid domains when calculating angles.
* **Dynamic Memory Safety:** Every resulting vector (e.g., from addition or normalization) is dynamically allocated on the heap. The program ensures complete memory cleanup even if intermediate allocations fail.

---

## 🧮 Supported Operations

The following mathematical formulas are strictly implemented and executed element-wise:

* **Addition & Subtraction:** $c_i = a_i \pm b_i$
* **Dot Product:** $a \cdot b = \sum_{i=0}^{n-1} (a_i \times b_i)$
* **Vector Length (Magnitude):** $\|a\| = \sqrt{\sum_{i=0}^{n-1} a_i^2}$
* **Angle Between Vectors (in degrees):** $\theta = \arccos\left(\frac{a \cdot b}{\|a\| \|b\|}\right) \times \frac{180}{\pi}$
* **Normalization:** $\hat{a} = \frac{a}{\|a\|}$

---

## 🎮 How to Use the Program

This program does not use a looped menu. Instead, it processes a single advanced mathematical query sequentially:

1. **Input Vector A:** * `Podaj rozmiar tablicy:` -> Enter the number of dimensions (e.g., `3` for a 3D vector).
   * `Podaj liczby:` -> Enter the float values separated by spaces.
2. **Input Vector B:**
   * Enter the size and values for the second vector in the same manner.
3. **Execution & Results:**
   The program will automatically calculate and print the results in the following order:
   * Vector $A + B$
   * Vector $A - B$
   * Dot Product $A \cdot B$
   * Angle between vectors
   * Length of Vector $A$
   * Length of Vector $B$
   * Normalized Vector $A$
   * Normalized Vector $B$

*Note: If you provide vectors of different sizes, the program will safely print `Operation not permitted` for incompatible operations instead of crashing.*

---

## 🛠️ Project Structure

* `array.h` - Contains the `array_t` struct definition and prototypes for 12 functions (both structural and mathematical).
* `array.c` - Core implementation. Uses `malloc`/`free` for memory and `acosf`, `sqrtf`, `pow` for vector calculus.
* `main.c` - Sequential Command Line Interface to input two multidimensional vectors and compute all properties.

---

## 🚀 Compilation

Navigate to this directory and compile the program using `gcc`. 
**Critical Note for Linux users:** You must link the math library by appending `-lm` at the end of the compilation command.

```bash
gcc main.c array.c -o vector_math -lm