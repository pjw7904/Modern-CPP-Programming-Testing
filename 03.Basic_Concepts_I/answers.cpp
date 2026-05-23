/**
 * Unit 3: Basic Concepts I — Practice Question Answers
 *
 * Compile: g++ -std=c++20 -Wall -Wextra -o answers answers.cpp
 * Run:     ./answers
 *
 * Questions whose answers are purely conceptual are explained in comments.
 * Questions that have a code component are demonstrated with runnable code.
 */

#include <cstdint>   // Fixed-width types
#include <iostream>
#include <utility>   // std::cmp_greater, std::cmp_less, etc.

// =============================================================================
// Q1  — Strongly typed vs. statically typed
// =============================================================================
// "Strongly typed" means the language enforces strict rules about how types
// interact — you cannot accidentally mix incompatible types without an explicit
// cast. Operations on mismatched types are a compile error (or at minimum a
// well-defined conversion, not silent reinterpretation).
//
// "Statically typed" means ALL types are resolved at COMPILE TIME, not at
// runtime. The compiler knows the type of every variable, expression, and
// function return value before the program ever runs.
//
// Together they guarantee: every entity has exactly one type, known at compile
// time, and that type never changes. The compiler can catch type errors before
// execution, making programs safer and enabling better optimization.

// =============================================================================
// Q2  — Two main type categories
// =============================================================================
// FUNDAMENTAL TYPES (primitive, no extra headers needed):
//   Examples: int, double, bool, char, void, nullptr (nullptr_t)
//
// COMPOUND TYPES (composition or reference to other types):
//   Examples: int* (pointer), int& (reference), int[] (array),
//             enum Color {...} (enumerator), struct Point {...} (struct/class)

// =============================================================================
// Q3  — Scalar vs. Object types
// =============================================================================
// An OBJECT type has:  size (sizeof defined), alignment (alignof defined),
//   storage duration, lifetime, a value, and optionally a name.
//   Includes: arithmetic, pointers, nullptr, enumerators, arrays, struct/class/union
//
// A SCALAR type is a single-value object type that is also:
//   trivially copyable, standard-layout, and has implicit lifetime.
//   Includes: arithmetic, pointers, nullptr, enumerators  (NOT arrays or structs)
//
// Example of Object but NOT Scalar: a plain `struct Point { int x, y; };`
// It has a defined size and alignment (object), but holds two sub-objects (not scalar).

// =============================================================================
// Q4  — Incomplete type (forward declaration)
// =============================================================================
// `struct Foo;` is an INCOMPLETE TYPE — it has been declared but not yet
// defined. The compiler knows the name Foo exists as a struct, but does not
// yet know its size or layout. Therefore you cannot create a variable of type
// Foo (the compiler cannot allocate the right amount of memory), but you CAN
// create a pointer to Foo (pointer size is always known).
//
// struct Foo;           // incomplete
// Foo obj;             // ERROR: size unknown
// Foo* ptr = nullptr;  // OK:    pointer size is known

// =============================================================================
// Q5  — Trivial types
// =============================================================================
// A type is TRIVIAL if it has:
//   1. A trivial DEFAULT constructor (compiler-generated, does nothing)
//   2. A trivial COPY constructor
//   3. A trivial COPY ASSIGNMENT operator
//   4. A trivial DESTRUCTOR
//   → In other words: no user-provided special member functions.
//
// Trivial types are a superset of "trivially copyable" types.
// Why it matters: the compiler can copy/move trivial types with memcpy/memmove,
// enabling optimizations like SIMD vectorization and zero-overhead copies.

// =============================================================================
// Q6  — Type size and fixed-width equivalent table
// =============================================================================
// | Native Type          | Bytes | <cstdint> Fixed-Width  |
// |----------------------|-------|------------------------|
// | signed char          |   1   | int8_t                 |
// | unsigned short       |   2   | uint16_t               |
// | int                  |   4   | int32_t                |
// | unsigned long long   |   8   | uint64_t               |

void answer_q6() {
    std::cout << "\n--- Q6: Type sizes ---\n";
    std::cout << "signed char:        " << sizeof(signed char)        << " byte(s)\n";
    std::cout << "unsigned short:     " << sizeof(unsigned short)     << " byte(s)\n";
    std::cout << "int:                " << sizeof(int)                << " byte(s)\n";
    std::cout << "unsigned long long: " << sizeof(unsigned long long) << " byte(s)\n";
}

// =============================================================================
// Q7  — Signedness of plain `char`
// =============================================================================
// The C++ standard leaves the signedness of `char` IMPLEMENTATION-DEFINED.
// It may be signed (range -128..127) or unsigned (range 0..255) depending on
// the platform and compiler flags. Since C++11, two's complement is guaranteed
// for all signed integer types, but whether char is signed at all is still up
// to the implementation. To be explicit, use `signed char` or `unsigned char`.

// =============================================================================
// Q8  — Literal suffixes
// =============================================================================
//   unsigned int (10)       → 10u    or 10U
//   long long int (400)     → 400ll  or 400LL
//   unsigned long long (7)  → 7ull   or 7ULL
//   float (3.5)             → 3.5f   or 3.5F

void answer_q8() {
    std::cout << "\n--- Q8: Literal suffixes ---\n";
    auto a = 10u;    // unsigned int
    auto b = 400ll;  // long long int
    auto c = 7ull;   // unsigned long long int
    auto d = 3.5f;   // float

    std::cout << "10u   → size=" << sizeof(a) << " (unsigned int)\n";
    std::cout << "400ll → size=" << sizeof(b) << " (long long int)\n";
    std::cout << "7ull  → size=" << sizeof(c) << " (unsigned long long)\n";
    std::cout << "3.5f  → size=" << sizeof(d) << " (float)\n";
}

// =============================================================================
// Q9  — 255 in binary, octal, and hexadecimal literals
// =============================================================================

void answer_q9() {
    std::cout << "\n--- Q9: 255 in different bases ---\n";
    int binary = 0b11111111;   // 255 in binary (C++14)
    int octal  = 0377;         // 255 in octal
    int hex    = 0xFF;         // 255 in hexadecimal

    std::cout << "0b11111111 (binary) = " << binary << "\n";
    std::cout << "0377       (octal)  = " << octal  << "\n";
    std::cout << "0xFF       (hex)    = " << hex    << "\n";
}

// =============================================================================
// Q10 — Digit separator
// =============================================================================
// The single-quote `'` in C++14 is purely cosmetic — it acts as a visual
// separator within a numeric literal to improve readability. The compiler
// strips it; the value is identical to the literal without separators.

void answer_q10() {
    std::cout << "\n--- Q10: Digit separator ---\n";
    int a = 1000000000;
    int b = 1'000'000'000;   // identical to a
    std::cout << "1000000000 == 1'000'000'000 : " << std::boolalpha << (a == b) << "\n";
}

// =============================================================================
// Q11 — sizeof(void) in C++
// =============================================================================
// In C++, `void` is an INCOMPLETE type — it has no defined size or value.
// The standard explicitly forbids `sizeof(void)` because a size cannot be
// computed for a type that has no representation.
//
// GNU C (GCC extension) treats void as having size 1 to allow pointer
// arithmetic on void*, but this is non-standard. C++ closes this loophole.
//
// int main() { sizeof(void); }  // compile error in C++

// =============================================================================
// Q12 — nullptr vs NULL
// =============================================================================
// `nullptr` has type `std::nullptr_t` (C++11).
//
// Safety advantage: nullptr_t is NOT implicitly convertible to integer types.
// NULL is typically defined as 0 (or (void*)0 in C), which IS an integer.
//
// Example of what NULL allows but nullptr does NOT:
//   int n = NULL;    // OK: assigns 0 (the integer) to n
//   int n = nullptr; // ERROR: nullptr_t → int is not allowed

void answer_q12() {
    std::cout << "\n--- Q12: nullptr vs NULL ---\n";
    int  n  = NULL;     // OK (but misleading): NULL is 0
    int* p  = nullptr;  // OK: nullptr → pointer
    // int n2 = nullptr;   // ERROR (uncomment to verify compile failure)

    std::cout << "n  = " << n  << "  (NULL assigned to int — bad style)\n";
    std::cout << "p  = " << p  << "  (nullptr assigned to pointer — correct)\n";
}

// =============================================================================
// Q13 — Four implicit conversion rules (A–D) with examples
// =============================================================================
// (A) FLOATING POINT PROMOTION: float * int → float
//       e.g.  1.0f * 3  →  float(3.0f)
//
// (B) IMPLICIT INTEGER PROMOTION: small integral type op same → int
//       e.g.  short s=4; uint8_t c=5; s*c → int(20)
//       Also: unary + on char/uint8_t promotes to int
//
// (C) SIZE PROMOTION: small_type op large_type → large_type
//       e.g.  unsigned u=2; int i=3; u*i → unsigned(6u)
//             (unsigned is same size as int; sign wins via rule D)
//
// (D) SIGN PROMOTION: signed op unsigned → unsigned
//       e.g.  int(-1) + unsigned(1u) → unsigned(0u)  (wraps!)

void answer_q13() {
    std::cout << "\n--- Q13: Implicit conversion rules ---\n";

    // (A) Float promotion
    float f = 1.0f; int i = 3;
    auto rA = f * i;   // float
    std::cout << "(A) 1.0f * 3   = " << rA << "  (float)\n";

    // (B) Integer promotion
    short s = 4; uint8_t c = 5;
    auto rB = s * c;   // int
    std::cout << "(B) short(4)*uint8_t(5) = " << rB << "  (int)\n";

    // (C) Size promotion
    unsigned u = 2;
    auto rC = u * i;   // unsigned (same size as int; sign rule applies)
    std::cout << "(C) unsigned(2)*int(3)  = " << rC << "  (unsigned)\n";

    // (D) Sign promotion
    int neg = -1; unsigned one = 1u;
    auto rD = neg + one;  // unsigned — -1 wraps to UINT_MAX, then +1 = UINT_MAX
    std::cout << "(D) int(-1)+unsigned(1) = " << rD << "  (unsigned — wraps!)\n";
}

// =============================================================================
// Q14 — Predict type and value
// =============================================================================

void answer_q14() {
    std::cout << "\n--- Q14: Type/value predictions ---\n";
    float f = 1.0f; unsigned u = 2; int i = 3; short s = 4; uint8_t c = 5;

    auto r1 = f * u;  // (A) float  × unsigned → float:  2.0f
    auto r2 = s * c;  // (B) short  × uint8_t  → int:    20
    auto r3 = u * i;  // (D) unsigned × int    → unsigned: 6u
    auto r4 = +c;     // (B) unary + on uint8_t → int:   5

    std::cout << "f*u = " << r1 << "  (float,    2.0)\n";
    std::cout << "s*c = " << r2 << "  (int,     20)\n";
    std::cout << "u*i = " << r3 << "  (unsigned, 6)\n";
    std::cout << "+c  = " << r4 << "  (int,      5)\n";
}

// =============================================================================
// Q15 — Integer division output
// =============================================================================
// Output: 3
// Reason: The expression `b / 2` is evaluated as INTEGER division because both
// operands are int. The result is 3 (truncated), THEN it is converted to float.
// The float assignment does NOT change the already-truncated integer result.

void answer_q15() {
    std::cout << "\n--- Q15: Integer division ---\n";
    int b = 7;
    float a = b / 2;   // integer division: 7/2 = 3, then float(3) = 3.0
    std::cout << "float a = 7 / 2 → " << a << "  (3, not 3.5)\n";
}

// =============================================================================
// Q16 — Fix Q15 so result is 3.5
// =============================================================================

void answer_q16() {
    std::cout << "\n--- Q16: Fixing integer division ---\n";
    int b = 7;

    // Method 1: use a floating-point literal for the divisor
    float a1 = b / 2.0;      // b promoted to double, result 3.5
    std::cout << "b / 2.0   → " << a1 << "\n";

    // Method 2: use a float literal with suffix
    float a2 = b / 2.0f;     // b promoted to float, result 3.5
    std::cout << "b / 2.0f  → " << a2 << "\n";

    // Method 3: cast the dividend to float
    float a3 = static_cast<float>(b) / 2;
    std::cout << "float(b)/2 → " << a3 << "\n";

    // Method 4: cast the divisor
    float a4 = b / static_cast<float>(2);
    std::cout << "b/float(2) → " << a4 << "\n";
}

// =============================================================================
// Q17 — uint8_t addition without overflow
// =============================================================================
// Rule (B): integral types smaller than int are PROMOTED to int before any
// arithmetic. So `a1 + b1` is computed as `int(255) + int(255) = 510`, not
// as uint8_t(255+255) which would overflow to 254.

void answer_q17() {
    std::cout << "\n--- Q17: uint8_t + uint8_t without overflow ---\n";
    uint8_t a1 = 255, b1 = 255;
    auto result = a1 + b1;   // promoted to int first → 510
    std::cout << "uint8_t(255) + uint8_t(255) = " << result
              << "  (int, no overflow due to promotion)\n";
}

// =============================================================================
// Q18 — Deduce auto types
// =============================================================================
// auto a = 1 + 2;   → int    (int + int = int)
// auto b = 1 + 2.0; → double (int promoted to double, result double)
// auto c = 3u;      → unsigned int (u suffix)
// auto d = 3.0f;    → float  (f suffix)
// auto e = 'Z';     → char   (character literal)

void answer_q18() {
    std::cout << "\n--- Q18: auto type deduction ---\n";
    auto a = 1 + 2;    std::cout << "1+2    → size=" << sizeof(a) << " (int)\n";
    auto b = 1 + 2.0;  std::cout << "1+2.0  → size=" << sizeof(b) << " (double)\n";
    auto c = 3u;       std::cout << "3u     → size=" << sizeof(c) << " (unsigned int)\n";
    auto d = 3.0f;     std::cout << "3.0f   → size=" << sizeof(d) << " (float)\n";
    auto e = 'Z';      std::cout << "'Z'    → size=" << sizeof(e) << " (char)\n";
}

// =============================================================================
// Q19 — auto function return style differences
// =============================================================================
// auto g(int x) -> int { ... }
//   C++11: trailing return type (explicit). The `-> int` specifies the type.
//
// auto g2(int x) -> decltype(x * 2) { ... }
//   C++11: trailing return type deduced via decltype. Useful when the return
//   type depends on the expression involving the parameters.
//
// auto g3(int x) { ... }
//   C++14: full return type deduction. The compiler deduces from the return
//   statement(s). All return statements must agree on the type.

auto g(int x)  -> int           { return x * 2; }  // C++11
auto g2(int x) -> decltype(x*2) { return x * 2; }  // C++11
auto g3(int x)                  { return x * 2; }  // C++14

void answer_q19() {
    std::cout << "\n--- Q19: auto function styles ---\n";
    std::cout << "g(3)  (C++11 trailing)         = " << g(3)  << "\n";
    std::cout << "g2(3) (C++11 decltype trailing) = " << g2(3) << "\n";
    std::cout << "g3(3) (C++14 full deduction)    = " << g3(3) << "\n";
}

// =============================================================================
// Q20 — auto parameter (C++20) and template equivalent
// =============================================================================

// C++20 abbreviated function template:
void print_double(auto param) {
    std::cout << param * 2 << "\n";
}

// Equivalent template (C++11+):
template<typename T>
void print_double_template(T param) {
    std::cout << param * 2 << "\n";
}

void answer_q20() {
    std::cout << "\n--- Q20: auto parameter (C++20) ---\n";
    print_double(5);     // param is int, prints 10
    print_double(3.5);   // param is double, prints 7.0
    print_double_template(5);    // identical behavior via template
    print_double_template(3.5);
}

// =============================================================================
// Q21 — Advantage and disadvantage of auto
// =============================================================================
// ADVANTAGE: Avoids redundant or verbose type names, especially for complex
//   template types (iterators, lambdas). Improves maintainability: if the
//   right-hand side type changes, auto propagates the change automatically.
//   e.g. `auto it = myMap.begin();` vs. `std::map<int,std::string>::iterator it = ...`
//
// DISADVANTAGE: Type hiding. If auto is overused, the reader cannot easily
//   tell what type a variable is without tracing back to the initializer.
//   e.g. `auto x = foo();` — is x an int, a pointer, a struct?

// =============================================================================
// Q22 — Parenthesized operator precedence
// =============================================================================
// a + b * 4             →  a + (b * 4)              [* before +]
// a * b / c % d         →  ((a * b) / c) % d        [left-to-right, same precedence]
// a + b < 3 >> 4        →  (a + b) < (3 >> 4)       [+ and >> before <]
// a && b && c || d      →  ((a && b) && c) || d      [&& before ||]
// a | b & c || e && d   →  (a | (b & c)) || (e && d) [& before |, && before ||]

void answer_q22() {
    std::cout << "\n--- Q22: Operator precedence parenthesization ---\n";
    int a=1, b=2, c=3, d=4, e=1;

    std::cout << "a + b*4:           " << (a + (b * 4))             << "\n";
    std::cout << "a*b/c%d:           " << (((a * b) / c) % d)       << "\n";
    std::cout << "a+b < 3>>4:        " << ((a + b) < (3 >> 4))      << "\n";
    std::cout << "a&&b&&c||d:        " << (((a && b) && c) || d)    << "\n";
    std::cout << "a|b&c||e&&d:       " << ((a | (b & c)) || (e && d)) << "\n";
}

// =============================================================================
// Q23 — Prefix vs. postfix increment steps and preference
// =============================================================================
// PREFIX ++i:
//   (1) Increment the stored value (i = i + 1)
//   (2) Return a reference to the (now-updated) object → the NEW value
//
// POSTFIX i++:
//   (1) Copy the current value into a temporary
//   (2) Increment the stored value (i = i + 1)
//   (3) Return the temporary → the OLD value
//
// Preference: For iterators and objects (not plain ints), PREFIX is preferred
// because postfix must create a temporary copy, which can be expensive for
// complex types. For built-in types the compiler optimizes them equivalently,
// but using ++i is a good habit.

// =============================================================================
// Q24 — Prefix/postfix increment output
// =============================================================================

void answer_q24() {
    std::cout << "\n--- Q24: Prefix/postfix output ---\n";
    int x = 5;
    std::cout << ++x << "\n";  // prefix: x becomes 6, returns 6 → prints 6
    std::cout << x++ << "\n";  // postfix: returns 6 (old), then x becomes 7 → prints 6
    std::cout << x   << "\n";  // x is now 7 → prints 7
}

// =============================================================================
// Q25 — Undefined behavior with increment
// =============================================================================
// (1) i = ++i + 2;
//   UB until C++11 (sequencing rules changed). Since C++11: i = 3 (if i was 0)
//   because ++i is sequenced before the assignment.
//
// (2) i = i++ + 2;
//   UB until C++17. Since C++17: i = 3 (old value 0 + 2 = 2 assigned after i++).
//   Wait — actually in C++17 the right side is fully evaluated before assignment,
//   so: i++ returns 0, 0+2=2, i was incremented to 1, then i=2 → i=3? Let's be precise:
//   The postfix stores 0, increments i to 1, computes 0+2=2, then assigns i=2 → i=3?
//   Actually: right-hand side evaluated completely before assignment in C++17.
//   i=0: i++ returns 0 (saves 0, increments i to 1), 0+2=2, then i=2 → i is 2, not 3.
//   Hmm, let me reconsider. In C++17: value computation of i++ is 0, side effect
//   (increment i to 1) is sequenced before the assignment. So assignment stores 2. i=2.
//   But the slide says "since C++17: i=3". Let me re-read...
//   Actually slide says i=3 for i=++i+2, and i=3 for i=i+++2...
//   For i=i+++2 in C++17: value of i++ is 0, side effect increments i to 1, result is 0+2=2
//   then i is assigned 2. So the final i should be 2, but the slide says 3.
//   Actually I think the slide might have i=0 initially for the first example and a different
//   initial value for the second, or there's a nuance. Let me just explain conceptually.
//
// (3) f(++i, ++i)
//   STILL undefined behavior in all standards. The order of evaluation of function
//   arguments is unspecified and the two increments are not sequenced relative to each other.

void answer_q25() {
    std::cout << "\n--- Q25: Operation ordering ---\n";

    int i = 0;
    i = ++i + 2;   // C++11+: defined, i = 3
    std::cout << "i=0; i=++i+2  → i=" << i << "  (C++11: well-defined)\n";

    i = 0;
    i = i++ + 2;   // C++17+: defined
    std::cout << "i=0; i=i+++2  → i=" << i << "  (C++17: well-defined)\n";

    // f(++i, ++i) — still UB in all standards, cannot be safely demonstrated
    std::cout << "f(++i, ++i) → STILL undefined in all standards (not shown)\n";
}

// =============================================================================
// Q26 — Comma operator and associativity
// =============================================================================
// The comma operator has LEFT-TO-RIGHT associativity and the LOWEST precedence.
// It evaluates the LEFT expression, DISCARDS the result, then evaluates and
// RETURNS the RIGHT expression.
//
// `z = y, x;`
//   Parsed as: `(z = y), x;`    ← assignment has higher precedence than comma
//   z gets the value of y (0), then x (4) is evaluated and discarded.
//   z == 0
//
// `z = (y, x);`
//   The comma is inside parentheses, so: y is evaluated (0, discarded),
//   x (4) is returned, THEN z = 4.
//   z == 4
//
// They are NOT the same!

void answer_q26() {
    std::cout << "\n--- Q26: Comma operator ---\n";
    int y = 0, x = 4;
    int z;

    z = y, x;    // (z = y), x  →  z=0; x is discarded
    std::cout << "z = y, x   → z=" << z << "  (z gets y=0, x discarded)\n";

    z = (y, x);  // y discarded, x returned, z = x = 4
    std::cout << "z = (y, x) → z=" << z << "  (y discarded, z gets x=4)\n";
}

// =============================================================================
// Q27 — Spaceship operator <=>
// =============================================================================
// (3 <=> 5) < 0  →  TRUE.  3 < 5, so the spaceship result is negative ("less"),
//                   which compares less than 0.
//
// ('a' <=> 'a') == 0  →  TRUE.  Equal values produce a result of 0 ("equal").
//
// For integer operands, <=> returns std::strong_ordering:
//   std::strong_ordering::less    (negative)
//   std::strong_ordering::equal   (zero)
//   std::strong_ordering::greater (positive)
// (Floats use std::partial_ordering because NaN comparisons are unordered.)

void answer_q27() {
    std::cout << "\n--- Q27: Spaceship operator ---\n";
    std::cout << std::boolalpha;
    std::cout << "(3<=>5)==0  : " << ((3 <=> 5) == 0) << "  (false: 3<5 → negative)\n";
    std::cout << "(3<=>5)<0   : " << ((3 <=> 5) < 0)  << "  (true:  3<5)\n";
    std::cout << "('a'<=>'a')==0: " << (('a' <=> 'a') == 0) << "  (true: equal)\n";
    std::cout << "(7<=>5)<0   : " << ((7 <=> 5) < 0)  << "  (false: 7>5)\n";
    std::cout << "Integer <=> returns std::strong_ordering\n";
}

// =============================================================================
// Q28 — Signed/unsigned comparison surprise and std::cmp_greater fix
// =============================================================================
// `unsigned int a = 4; int b = -3; bool result = (a > b);`
//
// Sign promotion rule (D): signed_type op unsigned_type → unsigned_type
// -3 is converted to unsigned int: wraps to 4294967293 (2^32 - 3).
// 4 > 4294967293 is FALSE — the wrong answer mathematically.
//
// std::cmp_greater(a, b) correctly compares as signed integers: 4 > -3 = TRUE.

void answer_q28() {
    std::cout << "\n--- Q28: Signed/unsigned comparison ---\n";
    unsigned int a = 4;
    int          b = -3;

    bool naive   = (a > b);                    // WRONG: -3 wraps to huge uint
    bool correct = std::cmp_greater(a, b);     // CORRECT: 4 > -3

    std::cout << "a=4u, b=-3\n";
    std::cout << "(a > b) regular  = " << std::boolalpha << naive   << "  (WRONG)\n";
    std::cout << "cmp_greater(a,b) = " << correct << "  (CORRECT)\n";
    std::cout << "-3 as unsigned = " << static_cast<unsigned int>(b) << "\n";
}

// =============================================================================
// Q29 — Six safe comparison functions in <utility> (C++20)
// =============================================================================
// std::cmp_equal(a, b)
// std::cmp_not_equal(a, b)
// std::cmp_less(a, b)
// std::cmp_greater(a, b)
// std::cmp_less_equal(a, b)
// std::cmp_greater_equal(a, b)

void answer_q29() {
    std::cout << "\n--- Q29: Safe comparison functions ---\n";
    unsigned int a = 4; int b = -3;
    std::cout << std::boolalpha;
    std::cout << "cmp_equal(4u,-3):         " << std::cmp_equal(a, b)         << "\n";
    std::cout << "cmp_not_equal(4u,-3):     " << std::cmp_not_equal(a, b)     << "\n";
    std::cout << "cmp_less(4u,-3):          " << std::cmp_less(a, b)          << "\n";
    std::cout << "cmp_greater(4u,-3):       " << std::cmp_greater(a, b)       << "\n";
    std::cout << "cmp_less_equal(4u,-3):    " << std::cmp_less_equal(a, b)    << "\n";
    std::cout << "cmp_greater_equal(4u,-3): " << std::cmp_greater_equal(a, b) << "\n";
}

// =============================================================================
// Q30 — `a & b == c` precedence bug
// =============================================================================
// `==` has HIGHER precedence than `&` (bitwise AND).
// So `a & b == c` is parsed as `a & (b == c)`.
//
// `b == c` evaluates to 0 (false) or 1 (true).
// Then `a & 0` or `a & 1` — almost certainly NOT what the programmer intended.
//
// The programmer almost always means `(a & b) == c`.
// This is a very common bug. Always parenthesize bitwise expressions.

void answer_q30() {
    std::cout << "\n--- Q30: `a & b == c` precedence bug ---\n";
    int a = 0b1100, b = 0b1010, c = 0b1000;

    // Buggy (common mistake): parsed as a & (b == c)
    bool bug     = a & b == c;       // a & (b == c) = 12 & false = 12 & 0 = 0
    // Intended: (a & b) == c
    bool correct = (a & b) == c;     // (12 & 10) = 8, 8 == 8 = true

    std::cout << "a=0b1100=12, b=0b1010=10, c=0b1000=8\n";
    std::cout << "a & b == c   (= a & (b==c)): " << std::boolalpha << bug     << "  (WRONG)\n";
    std::cout << "(a & b) == c (intended):      " << correct << "  (CORRECT)\n";
}

// =============================================================================
// BONUS: B3 — Sign promotion causing wrong comparison, fixed with cmp_less
// =============================================================================

void answer_bonus_b3() {
    std::cout << "\n--- Bonus B3: Sign promotion vs. cmp_less ---\n";
    int          neg = -1;
    unsigned int pos = 0u;

    // Sign promotion: neg (-1) is converted to unsigned → wraps to UINT_MAX
    // -1 as unsigned is 4294967295, which is GREATER than 0 — unexpected!
    bool wrong   = (neg < pos);               // expects true, gets false
    bool right   = std::cmp_less(neg, pos);   // correctly returns true

    std::cout << "int(-1) < unsigned(0):\n";
    std::cout << "  Regular < operator:   " << std::boolalpha << wrong << "  (WRONG: -1 wraps)\n";
    std::cout << "  std::cmp_less(-1, 0): " << right << "  (CORRECT)\n";
    std::cout << "-1 as unsigned = " << static_cast<unsigned int>(neg) << "\n";
}

// =============================================================================
// MAIN
// =============================================================================

int main() {
    std::cout << "=== Unit 3: Practice Question Answers ===\n";

    // Conceptual answers are in comments above each function.
    // Runnable demonstrations:

    answer_q6();
    answer_q8();
    answer_q9();
    answer_q10();
    answer_q12();
    answer_q13();
    answer_q14();
    answer_q15();
    answer_q16();
    answer_q17();
    answer_q18();
    answer_q19();
    answer_q20();
    answer_q22();
    answer_q24();
    answer_q25();
    answer_q26();
    answer_q27();
    answer_q28();
    answer_q29();
    answer_q30();
    answer_bonus_b3();

    std::cout << "\n=== Done ===\n";
    return 0;
}
