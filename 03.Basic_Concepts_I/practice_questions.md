# Unit 3: Basic Concepts I — Practice Questions

**Topics:** Type System, Fundamental Types, and Operators

---

## Section 1 — The C++ Type System

**Q1.** C++ is described as both "strongly typed" and "statically typed." In your own words, what does each of those terms mean, and what guarantee do they together provide to the programmer?

**Q2.** List the two main type categories in C++ and give at least two examples of each.

**Q3.** What is the difference between a **scalar** type and an **object** type? Give one example of a type that is an object but *not* a scalar.

**Q4.** A forward-declared struct like `struct Foo;` is an example of what kind of type property? Why can you not create a variable of that type immediately after the forward declaration?

**Q5.** A type is called **trivial** if it meets certain criteria. What are those criteria, and why does triviality matter (e.g., what does it allow the compiler to do)?

---

## Section 2 — Fundamental Types

**Q6.** Fill in the table with the correct size (in bytes) and fixed-width `<cstdint>` equivalent for each native type:

| Native Type        | Bytes | `<cstdint>` Fixed-Width Equivalent |
|--------------------|-------|-------------------------------------|
| `signed char`      |       |                                     |
| `unsigned short`   |       |                                     |
| `int`              |       |                                     |
| `unsigned long long` |    |                                     |

**Q7.** What is the signedness of a plain `char` — signed or unsigned? What does the standard say about it?

**Q8.** Write the correct literal suffix for each of the following:
- An `unsigned int` with value 10
- A `long long int` with value 400
- An `unsigned long long int` with value 7
- A `float` with value 3.5

**Q9.** Express the decimal value **255** as:
- A binary literal (C++14)
- An octal literal
- A hexadecimal literal

**Q10.** What does the digit separator `'` do in C++14? Rewrite `1000000000` using digit separators for readability.

**Q11.** Why is `sizeof(void)` a compile error in C++ when it is valid (and equals 1) under GCC's C mode?

**Q12.** What type does `nullptr` have in C++11? Why is `nullptr` safer than the `NULL` macro? Give an example of something you can do with `NULL` that you *cannot* do with `nullptr`.

---

## Section 3 — Conversion Rules

**Q13.** List the four implicit type conversion rules (A–D) that apply before any arithmetic operation, in the order they are applied. Give a short concrete example for each.

**Q14.** Predict the type and value of each expression below (assume `float f=1.0f; unsigned u=2; int i=3; short s=4; uint8_t c=5;`):

| Expression | Result Type | Value |
|------------|-------------|-------|
| `f * u`    |             |       |
| `s * c`    |             |       |
| `u * i`    |             |       |
| `+c`       |             |       |

**Q15.** What is the output of the following code, and why?
```cpp
int b = 7;
float a = b / 2;
std::cout << a;
```

**Q16.** Fix the code in Q15 so that `a` holds `3.5` instead of `3.0`. Show at least two ways.

**Q17.** Given `uint8_t a1 = 255; uint8_t b1 = 255;`, what is the value of `a1 + b1`, and why does no overflow occur?

---

## Section 4 — `auto` Keyword

**Q18.** Deduce the type of each `auto` variable:
```cpp
auto a = 1 + 2;
auto b = 1 + 2.0;
auto c = 3u;
auto d = 3.0f;
auto e = 'Z';
```

**Q19.** What is the difference between the three function definitions below in terms of which C++ standard introduced each style?
```cpp
auto g(int x) -> int { return x * 2; }
auto g2(int x) -> decltype(x * 2) { return x * 2; }
auto g3(int x) { return x * 2; }
```

**Q20.** C++20 allows `auto` in function parameters. Write a function `print_double` that takes an `auto` parameter and prints `param * 2`. What is the equivalent template syntax for this function?

**Q21.** Give one advantage and one disadvantage of using `auto` instead of an explicit type.

---

## Section 5 — C++ Operators

**Q22.** Without running the code, add parentheses to make the implicit grouping explicit:
```
a + b * 4
a * b / c % d
a + b < 3 >> 4
a && b && c || d
a | b & c || e && d
```

**Q23.** What are the exact steps performed by the **prefix** increment operator (`++i`) versus the **postfix** increment operator (`i++`)? Which one is generally preferred for iterators in loops, and why?

**Q24.** What is the output of the following code? Explain each line.
```cpp
int x = 5;
std::cout << ++x << "\n";
std::cout << x++ << "\n";
std::cout << x   << "\n";
```

**Q25.** Explain why the following expressions were historically undefined behavior, and whether they are now defined (and in which standard):
```cpp
int i = 0;
i = ++i + 2;      // (1)
i = i++ + 2;      // (2)
f(++i, ++i);      // (3)
```

**Q26.** What does the comma operator do? What is the value of `z` after:
```cpp
int y = 0, x = 4;
int z;
z = y, x;
```
Is this the same as `z = (y, x)`? Why or why not?

**Q27.** The **spaceship operator** `<=>` was introduced in C++20. Answer the following:
- What does `(3 <=> 5) < 0` evaluate to, and why?
- What does `('a' <=> 'a') == 0` evaluate to?
- What return type does `<=>` produce for integer operands?

**Q28.** Explain why the following comparison can produce a surprising result, and how C++20's safe comparison functions fix it:
```cpp
unsigned int a = 4;
int          b = -3;
bool result = (a > b);   // What does this print?
```

**Q29.** Name all six safe comparison functions introduced in C++20's `<utility>` header.

**Q30.** *(Synthesis)* Consider the expression `a & b == c`. Based on operator precedence, how is it actually parsed? Write the parenthesized version, and explain why this is a common bug.

---

## Bonus / Challenge Questions

**B1.** `long int` has a platform-dependent size. On which specific platform is it 4 bytes instead of 8, even in a 64-bit environment?

**B2.** Why does the standard guarantee that `signed char` has range −128 to 127 (two's complement), whereas the signedness of a plain `char` is implementation-defined?

**B3.** Write a small program that demonstrates the sign promotion rule (rule D) causing an unexpected result when comparing a negative `int` with an `unsigned int` using the `<` operator, then fix it with `std::cmp_less`.
