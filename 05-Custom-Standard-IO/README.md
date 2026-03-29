# ⌨️ Custom Standard I/O (`printf` & `scanf` clones)

> A from-scratch implementation of the C Standard Library's formatting functions (`printf` and `scanf`). Built to demonstrate low-level character parsing and variadic function handling.

---

## ✨ Key Features & Technical Highlights

This project reconstructs core standard library functionalities under extremely strict constraints: **no array operators (`[]`), no dynamic memory allocation (`malloc`), and completely banning the `<stdio.h>` high-level formatting functions.**

* **Variadic Functions:** Utilizes the `<stdarg.h>` library (`va_list`, `va_start`, `va_arg`) to handle an arbitrary number of arguments passed to the formatting strings.
* **Low-Level Parsing:** All numbers (integers and floats) are parsed and constructed mathematically character-by-character using raw `getchar()` and `putchar()` stream inputs/outputs.
* **Custom Struct Formatting:** Extends standard formatting rules by introducing a custom `%p` format specifier explicitly designed to parse and print a custom `point_t` coordinate struct `(x y)`.

---

## 🔤 Supported Format Specifiers

Both `my_printf` and `my_scanf` support a customized subset of format specifiers:

* `%s` - Strings (Character pointers). *(printf only)*
* `%d` - 32-bit signed integers.
* `%f` - Double precision floating-point numbers (printed strictly to 5 decimal places).
* `%p` - Custom `point_t` structure, mapped to the format `(x y)`.

---

## 🛠️ Project Structure

* `main.c` - Contains the definition of the `point_t` struct, mathematical helper functions for digit counting, the core `my_printf` and `my_scanf` implementations, and a brief test suite in `main()`.

---

## 🚀 Compilation & Usage

Navigate to this directory and compile the program using `gcc`. 
*Note: The `-lm` flag is required on Linux environments to link the math library for the `pow()` function used in digit calculation.*

```bash
gcc main.c -o custom_io -lm