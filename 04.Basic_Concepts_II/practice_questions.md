# Unit 4: Basic Concepts II — Practice Questions

**Topics:** Integer Types, Overflow Semantics, IEEE 754, and Floating-Point Arithmetic

---

## Section 1 — C++ Data Models

**Q1.** Name the three modern C++ data models, the OS each corresponds to, and the key difference between them. Which type's size differs most significantly across platforms?

**Q2.** What is the one type whose size the C++ standard *guarantees* is exactly 1 byte? What does the standard guarantee about the relationship between `short`, `int`, `long`, and `long long`?

**Q3.** A colleague writes code with `long` everywhere because "it's 64 bits." On which platform would this silently break, and why?

---

## Section 2 — Fixed-Width Integers, `size_t`, `ptrdiff_t`, `uintptr_t`

**Q4.** Why does the C++ standard provide `int32_t` and `int64_t` if it already has `int` and `long`?

**Q5.** `int*_t` types are described as typedefs, not "real" types. What concrete problem does this cause with **overload resolution**? (Hint: consider a function overloaded for `char`, `short`, `int`, `long`, and `long long`.)

**Q6.** What does the following code print, and why? What is the fix?
```cpp
int8_t var = 50;
std::cout << var << "\n";
std::cout << var + 0 << "\n";
```

**Q7.** Compare `size_t` and `ptrdiff_t`:
- What header defines each?
- Is each signed or unsigned?
- When should you use each?
- What does C++23 add for each?

**Q8.** What is `uintptr_t`? What two properties does the standard guarantee about it, and what optional caveat does the standard include?

---

## Section 3 — Arithmetic Operation Semantics

**Q9.** Define the three arithmetic operation semantics — **overflow**, **wraparound**, and **saturation** — and state which one C++ uses for signed integers, which for unsigned integers, and which C++26 is adding.

**Q10.** If `unsigned int a = UINT_MAX;`, what is the value of `a + 1u`, and is this undefined behavior? Explain why.

**Q11.** C++26 introduces saturation arithmetic. Name the five functions/cast added in `<numeric>` and describe what `add_sat(T x, T y)` does differently from regular addition.

---

## Section 4 — Signed Integer Behavior and Undefined Behavior

**Q12.** List the four **undefined behavior** categories for signed integer arithmetic described in the slides, and give one concrete code example for each.

**Q13.** Why does `INT_MAX * -1` produce a valid result, but `INT_MIN * -1` is undefined behavior?

**Q14.** Which of the following shift expressions is undefined behavior in C++17, and why? Which become well-defined in C++20?
```cpp
int y = -1 << 12;    // (a)
int z = 1 << -12;    // (b)
unsigned w = 1u << 32u;  // (c)
int v = 1 << 30;     // (d)
```

**Q15.** The slides show a real-world signed-overflow UB example from the Linux kernel. A compiler sees `if (offset + len < 0) return -EINVAL;` and eliminates it. Explain *why* the compiler is permitted to do this, and what the bug is.

**Q16.** Is signed integer overflow well-defined at the **hardware** level? If yes, why does C++ still call it UB?

---

## Section 5 — Unsigned Integer Behavior

**Q17.** Unsigned integers are described as "associative" while signed integers are not. What specific mathematical property makes this true for unsigned but not signed?

**Q18.** What is the output of the following, and is it defined behavior?
```cpp
unsigned int x = 0;
x -= 1;
std::cout << x;
```

**Q19.** Explain the bug in the following loop and how to fix it:
```cpp
std::vector<int> v = getValues();
for (size_t i = 0; i < v.size() - 1; i++)
    process(v[i], v[i+1]);
```

---

## Section 6 — When to Use Signed vs. Unsigned / Numeric Limits

**Q20.** The Google Style Guide calls unsigned container sizes a "historical accident." What does Bjarne Stroustrup recommend instead, and what is the practical maximum value that recommendation provides?

**Q21.** Give two reasons to prefer **signed** integers and two reasons to prefer **unsigned** integers in safety-critical code.

**Q22.** Write the `<limits>` expressions to get: the largest `int`, the smallest (most negative) `float`, the smallest positive normal `float`, and the machine epsilon of `double`.

---

## Section 7 — Promotion and Truncation

**Q23.** What are the values stored in `y` and `w` below, and why?
```cpp
int16_t x = -1;
int     y = x;         // (a)

int     z = 32769;
int16_t w = z;         // (b)
```

**Q24.** Explain why truncation is described as a "modulo operation with respect to the number of bits of the smaller type."

---

## Section 8 — Mixing Signed/Unsigned Errors

**Q25.** Trace the execution of the following and state what index is actually computed. Does this cause undefined behavior?
```cpp
unsigned a = 10;
int      b = -1;
// What is the value of (10ull + a * b)?
```

**Q26.** What does the following function return when called as `f(-1, 5, arr)`, and why?
```cpp
int f(int a, unsigned b, int* arr) {
    if (a > b)
        return arr[a - b];
    return 0;
}
```

**Q27.** A comparison `signed_val > unsigned_val` can silently give the wrong answer. Which implicit conversion rule from Unit 3 causes this, and what C++20 function should be used instead?

---

## Section 9 — Overflow / Underflow Detection

**Q28.** Why must signed integer overflow be detected **before** the operation, while unsigned overflow can be detected **after**?

**Q29.** Complete and explain each function:
```cpp
bool is_add_overflow(unsigned a, unsigned b) {
    return ____;
}

bool is_mul_overflow(unsigned a, unsigned b) {
    unsigned x = a * b;
    return ____;
}
```

**Q30.** Write a function `signed_add_safe(int a, int b)` that returns `true` if adding `a + b` would overflow a 32-bit signed integer, without performing the overflowing addition.

---

## Section 10 — IEEE 754 Floating-Point Representation

**Q31.** Fill in the table:

| Format    | Total Bits | Sign | Exponent | Mantissa | C++ Type     |
|-----------|-----------|------|----------|----------|--------------|
| Single    |            |      |          |          |              |
| Double    |            |      |          |          |              |
| float16   |            |      |          |          |              |
| bfloat16  |            |      |          |          |              |

**Q32.** Explain the **exponent bias** in IEEE 754 single-precision. What is the bias value? What stored exponent values are reserved for special meanings?

**Q33.** Given the 32-bit float with bits `0 | 10000111 | 11000000000000000000000`, decode it step-by-step to its decimal value. (This is the example from the slides.)

**Q34.** What is a **denormal** (subnormal) number? Why do they exist? How does the exponent field signal a denormal?

---

## Section 11 — Infinity and NaN

**Q35.** List **four** floating-point operations that generate `inf` and **four** that generate `NaN`.

**Q36.** What are the results of each expression? Explain each.
```cpp
5.0f / 0.0f         // (a)
-5.0f / 0.0f        // (b)
0.0f / 0.0f         // (c)
std::numeric_limits<float>::infinity() * 0.0f  // (d)
```

**Q37.** Why is `(NaN == NaN)` false? Write two ways to correctly test whether a float variable `x` is NaN.

**Q38.** Is `+0.0f == -0.0f`? Do they have the same bit pattern?

---

## Section 12 — Machine Epsilon and ULP

**Q39.** Define **machine epsilon** (ε). What are its approximate values for `float` and `double`?

**Q40.** Define **ULP** (Units at the Last Place). How does ULP relate to machine epsilon at a magnitude of 1.0? How does ULP change as the magnitude grows?

**Q41.** The following code is intended to represent a real number `x` as `fl(x)`. Express the **absolute error** and **relative error** bounds in terms of ULP and ε.

**Q42.** Why does `(int)16777217.0f` print `16777216` and not `16777217`?

---

## Section 13 — Floating-Point Arithmetic Properties

**Q43.** For each property P1–P7, state whether it holds for floating-point arithmetic and give a brief reason or example:
- P1: `a op b == a ⊙ b`
- P2: Reflexive (`a == a`)
- P3: Commutative (`a ⊙ b == b ⊙ a`)
- P4: Associative (`(a ⊕ b) ⊕ c == a ⊕ (b ⊕ c)`)
- P5: Distributive (`(a ⊕ b) ⊗ c == (a ⊗ c) ⊕ (b ⊗ c)`)
- P6: Identity (`(a ⊖ b) ⊕ b == a`)
- P7: Overflow/underflow → ??

**Q44.** What does IEEE 754 guarantee about each individual floating-point operation, and under what condition are results fully deterministic across different machines?

---

## Section 14 — Floating-Point Undefined Behavior

**Q45.** Which of the following is undefined behavior in C++? Explain each.
```cpp
float a = 1.0f / 0.0f;       // (a)
int   b = (int)(1e38f);       // (b)
float c = (float)(1e400);     // (c)
double d = 0.0 / 0.0;        // (d)
```

**Q46.** What does `<cfenv>` provide? Name the five exception macros and the two primary functions. What pragma is typically required, and which major compiler does **not** support it?

---

## Section 15 — Catastrophic Cancellation

**Q47.** Describe the two cases of catastrophic cancellation (C1 and C2). Give a real-world example of each from the slides.

**Q48.** The following `while` loop runs forever for some inputs. Explain why, and identify the underlying floating-point phenomenon.
```cpp
float x = 30000000.0f;
while (x > 0)
    x -= 1.0f;
```

**Q49.** In the quadratic formula for `x² + 5000x + 0.25 = 0`:
- Why does `(-b + √(b²-4ac)) / 2a` produce catastrophic cancellation?
- What algebraically-equivalent formula avoids it for the near-zero root?

---

## Section 16 — Floating-Point Comparison

**Q50.** What are the **three problems** with the simple absolute-error comparison `std::abs(a - b) < epsilon`?

**Q51.** What are the **four problems** with the relative-error comparison `std::abs(a - b) / b < epsilon` (always dividing by `b`)?

**Q52.** Implement the robust `areFloatNearlyEqual(float a, float b)` function from the slides. Explain each step.

---

## Section 17 — Error Propagation

**Q53.** List five strategies from the slides to minimize floating-point error propagation.

**Q54.** Explain the **Kahan summation algorithm**. Why does naively accumulating millions of small floats produce significant error, and how does Kahan's technique reduce it? Write the algorithm in pseudocode.

---

## Bonus / Challenge Questions

**B1.** A firmware bug causes HPE SSDs to fail after exactly 32,768 hours. Express 32,768 as a power of 2. What integer type most likely stores the uptime counter, and what overflow event triggers the failure?

**B2.** The Boeing 787 must be rebooted every 51 days. Compute 51 days in milliseconds. What type might overflow to cause this bug?

**B3.** Write a safe `ceiling_division(int a, int b)` that correctly computes ⌈a/b⌉ for positive integers without using `float` or `ceil()`.

**B4.** The Ariane 5 rocket failed due to converting a 64-bit floating-point value to a 16-bit signed integer. What C++ operation category does this fall into, and what caused the loss of $137 million?
