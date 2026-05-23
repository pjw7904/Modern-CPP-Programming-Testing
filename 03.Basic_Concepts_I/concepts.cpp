/**
 * Unit 3: Basic Concepts I
 * Type System, Fundamental Types, and Operators
 *
 * Compile: g++ -std=c++20 -Wall -Wextra -o concepts concepts.cpp
 * Run:     ./concepts
 */

#include <compare>   // std::strong_ordering (spaceship operator)
#include <cstdint>   // Fixed-width integer types (int8_t, uint32_t, etc.)
#include <iostream>
#include <utility>   // std::cmp_greater and other safe comparison functions

// =============================================================================
// SECTION 1: The C++ Type System
// =============================================================================
//
// C++ is STRONGLY TYPED and STATICALLY TYPED:
//   - Every entity (variable, function, expression) has a type.
//   - That type NEVER changes at runtime.
//   - The compiler must know every type at compile time.
//
// A type specifies:
//   1. The amount of MEMORY allocated for the variable.
//   2. The KINDS OF VALUES that may be stored and how bits are interpreted.
//   3. The OPERATIONS that are permitted on those entities.
//
// Users can introduce new types with `class` or `struct`.
//
// Type Categories:
//
//   FUNDAMENTAL TYPES (primitive — no extra headers required):
//     - Arithmetic types: integer and floating-point
//     - void
//     - nullptr (C++11)
//
//   COMPOUND TYPES (compositions/references of other types):
//     - Pointers
//     - References
//     - Enumerators
//     - Arrays
//     - struct, class, union
//     - Functions
//
// Type Properties:
//   - OBJECT types have: size (sizeof), alignment (alignof), storage duration,
//     lifetime, value, and optionally a name.
//     Includes: Arithmetic, Pointers, nullptr, Enumerators, Arrays, struct/class/union
//
//   - SCALAR types: hold a single value, trivially copyable, standard layout,
//     implicit lifetime (no user constructor/destructor).
//     Includes: Arithmetic, Pointers, nullptr, Enumerators
//
//   - TRIVIAL types: trivial default/copy constructor, copy assignment, destructor.
//     Includes: Scalar types, trivial class types, arrays of such types.
//
//   - INCOMPLETE types: declared but not yet fully defined.
//     Examples: void, `struct A;` (forward declaration)

// =============================================================================
// SECTION 2: Fundamental Types Overview
// =============================================================================

void section_fundamental_types() {
    std::cout << "\n=== Fundamental Types ===\n";

    // -------------------------------------------------------------------------
    // 2a. Arithmetic Types — Integral
    //
    //  Native Type            | Bytes | Range
    //  -----------------------|-------|-------------------------------
    //  bool                   |   1   | true, false
    //  char                   |   1   | implementation-defined (signed or unsigned)
    //  signed char            |   1   | -128 to 127               → int8_t
    //  unsigned char          |   1   | 0 to 255                  → uint8_t
    //  short                  |   2   | -2^15 to 2^15-1           → int16_t
    //  unsigned short         |   2   | 0 to 2^16-1               → uint16_t
    //  int                    |   4   | -2^31 to 2^31-1           → int32_t
    //  unsigned int           |   4   | 0 to 2^32-1               → uint32_t
    //  long int               |  4/8  | platform-dependent        → int32_t/int64_t
    //  unsigned long int      |  4/8* | platform-dependent        → uint32_t/uint64_t
    //  long long int          |   8   | -2^63 to 2^63-1           → int64_t
    //  long long unsigned int |   8   | 0 to 2^64-1               → uint64_t
    //
    //  * 4 bytes on Windows 64-bit systems
    //  Note: char is signed or unsigned — implementation defined.
    //        Two's complement is guaranteed from C++11 onward.
    // -------------------------------------------------------------------------

    bool     b  = true;
    char     c  = 'A';
    signed char   sc = -10;
    unsigned char uc = 200;
    short    s  = -300;
    unsigned short us = 600;
    int      i  = -100000;
    unsigned int  ui = 100000u;
    long     l  = -1000000L;
    unsigned long ul = 1000000UL;
    long long     ll  = -9000000000LL;
    unsigned long long ull = 9000000000ULL;

    std::cout << "bool:               " << b  << "  (size=" << sizeof(b)  << ")\n";
    std::cout << "char:               " << c  << "  (size=" << sizeof(c)  << ")\n";
    std::cout << "signed char:        " << static_cast<int>(sc) << "  (size=" << sizeof(sc) << ")\n";
    std::cout << "unsigned char:      " << static_cast<int>(uc) << "  (size=" << sizeof(uc) << ")\n";
    std::cout << "short:              " << s  << "  (size=" << sizeof(s)  << ")\n";
    std::cout << "unsigned short:     " << us << "  (size=" << sizeof(us) << ")\n";
    std::cout << "int:                " << i  << "  (size=" << sizeof(i)  << ")\n";
    std::cout << "unsigned int:       " << ui << "  (size=" << sizeof(ui) << ")\n";
    std::cout << "long:               " << l  << "  (size=" << sizeof(l)  << ")\n";
    std::cout << "unsigned long:      " << ul << "  (size=" << sizeof(ul) << ")\n";
    std::cout << "long long:          " << ll  << "  (size=" << sizeof(ll)  << ")\n";
    std::cout << "unsigned long long: " << ull << "  (size=" << sizeof(ull) << ")\n";

    // Fixed-width types from <cstdint> — preferred when exact sizes matter
    int8_t   i8  = -128;
    uint8_t  u8  = 255;
    int16_t  i16 = -32768;
    uint16_t u16 = 65535;
    int32_t  i32 = -2147483648;
    uint32_t u32 = 4294967295u;
    int64_t  i64 = -9223372036854775807LL;
    uint64_t u64 = 18446744073709551615ULL;

    std::cout << "\n--- Fixed-width types (<cstdint>) ---\n";
    std::cout << "int8_t:   " << static_cast<int>(i8)  << "\n";
    std::cout << "uint8_t:  " << static_cast<int>(u8)  << "\n";
    std::cout << "int16_t:  " << i16 << "\n";
    std::cout << "uint16_t: " << u16 << "\n";
    std::cout << "int32_t:  " << i32 << "\n";
    std::cout << "uint32_t: " << u32 << "\n";
    std::cout << "int64_t:  " << i64 << "\n";
    std::cout << "uint64_t: " << u64 << "\n";

    // -------------------------------------------------------------------------
    // 2b. Arithmetic Types — Floating-Point
    //
    //  Native Type | IEEE-754 | Bytes | Range
    //  ------------|----------|-------|-----------------------------------
    //  float       |    Y     |   4   | ±1.18×10^-38 to ±3.4×10^38
    //  double      |    Y     |   8   | ±2.23×10^-308 to ±1.8×10^308
    //  long double |    N     |  8/12/16 | implementation-defined
    //
    //  C++23 also introduces std::float16_t, std::bfloat16_t, std::float32_t,
    //  std::float64_t via <stdfloat> (not demonstrated here for portability).
    // -------------------------------------------------------------------------

    float       f   = 3.14f;
    double      d   = 3.141592653589793;
    long double ld  = 3.141592653589793238L;

    std::cout << "\n--- Floating-Point Types ---\n";
    std::cout << "float:       " << f  << "  (size=" << sizeof(f)  << ")\n";
    std::cout << "double:      " << d  << "  (size=" << sizeof(d)  << ")\n";
    std::cout << "long double: " << ld << "  (size=" << sizeof(ld) << ")\n";

    // -------------------------------------------------------------------------
    // 2c. Short Names
    //
    //  Signed Type            | Short Name
    //  -----------------------|-----------
    //  signed short int       | short
    //  signed int             | int
    //  signed long int        | long
    //  signed long long int   | long long
    //
    //  Unsigned Type          | Short Name
    //  -----------------------|-----------
    //  unsigned short int     | unsigned short
    //  unsigned int           | unsigned
    //  unsigned long int      | unsigned long
    //  unsigned long long int | unsigned long long
    // -------------------------------------------------------------------------

    // -------------------------------------------------------------------------
    // 2d. Suffix and Prefix for Literals
    //
    //  SUFFIX — controls the TYPE of an integer or floating-point literal:
    //    u / U        → unsigned int         e.g.  3u
    //    l / L        → long int             e.g.  8L
    //    ul / UL      → unsigned long        e.g.  2ul
    //    ll / LL      → long long int        e.g.  4ll
    //    ull / ULL    → unsigned long long   e.g.  7ULL
    //    f / F        → float (decimal only) e.g.  3.0f
    //    (none)       → double               e.g.  3.0
    //
    //  C++23 suffixes for fixed-width floats (<stdfloat>):
    //    f16/F16  → std::float16_t
    //    bf16/BF16 → std::bfloat16_t
    //    f32/F32  → std::float32_t
    //    f64/F64  → std::float64_t
    //    f128/F128 → std::float128_t
    //
    //  PREFIX — controls the BASE of an integer literal:
    //    0b / 0B  → Binary       (C++14)   e.g. 0b010101
    //    0        → Octal                  e.g. 0307
    //    0x / 0X  → Hexadecimal            e.g. 0xFFA010
    //
    //  C++14 DIGIT SEPARATOR: single-quote ' improves readability
    //    e.g. 1'000'000  is the same as  1000000
    // -------------------------------------------------------------------------

    std::cout << "\n--- Literal Suffixes ---\n";
    auto lit_uint    = 3u;           // unsigned int
    auto lit_long    = 8L;           // long int
    auto lit_ulong   = 2ul;          // unsigned long
    auto lit_llong   = 4ll;          // long long int
    auto lit_ullong  = 7ULL;         // unsigned long long int
    auto lit_float   = 3.0f;         // float
    auto lit_double  = 3.0;          // double

    std::cout << "3u   → " << lit_uint   << "  (size=" << sizeof(lit_uint)   << ")\n";
    std::cout << "8L   → " << lit_long   << "  (size=" << sizeof(lit_long)   << ")\n";
    std::cout << "2ul  → " << lit_ulong  << "  (size=" << sizeof(lit_ulong)  << ")\n";
    std::cout << "4ll  → " << lit_llong  << "  (size=" << sizeof(lit_llong)  << ")\n";
    std::cout << "7ULL → " << lit_ullong << "  (size=" << sizeof(lit_ullong) << ")\n";
    std::cout << "3.0f → " << lit_float  << "  (size=" << sizeof(lit_float)  << ")\n";
    std::cout << "3.0  → " << lit_double << "  (size=" << sizeof(lit_double) << ")\n";

    std::cout << "\n--- Literal Prefixes ---\n";
    int bin = 0b010101;     // Binary (C++14): 21 in decimal
    int oct = 0307;          // Octal: 199 in decimal
    int hex = 0xFFA010;     // Hexadecimal: 16752656 in decimal

    std::cout << "0b010101 (binary)      = " << bin << "\n";
    std::cout << "0307     (octal)       = " << oct << "\n";
    std::cout << "0xFFA010 (hex)         = " << hex << "\n";

    // Digit separator (C++14) — purely cosmetic, no effect on value
    int million = 1'000'000;
    std::cout << "1'000'000 (separator)  = " << million << "\n";

    // -------------------------------------------------------------------------
    // 2e. void Type
    //
    // void is an INCOMPLETE type — not defined, has no value.
    //   - Used as a function return type meaning "no value returned"
    //   - Used as a parameter list to indicate "no parameters" (C-style)
    //   - sizeof(void) is a COMPILE ERROR in C++ (unlike C/GCC where it is 1)
    // -------------------------------------------------------------------------

    std::cout << "\n--- void Type ---\n";
    // void v;               // ERROR: cannot declare a variable of type void
    // sizeof(void);         // ERROR: incomplete type
    std::cout << "void: incomplete type, no sizeof, used as function return/param indicator\n";

    // -------------------------------------------------------------------------
    // 2f. nullptr Keyword (C++11)
    //
    // nullptr represents a null pointer (0x0) and REPLACES the NULL macro.
    // Its type is nullptr_t — safer because it is NOT implicitly convertible
    // to integer types (unlike NULL which was 0 or (void*)0).
    // -------------------------------------------------------------------------

    std::cout << "\n--- nullptr Keyword ---\n";
    int* p1 = NULL;     // OK (legacy): NULL is 0, assigning 0 to pointer
    int* p2 = nullptr;  // OK (preferred): nullptr_t → pointer conversion
    int  n1 = NULL;     // OK but misleading: assigns 0 to an int
    // int  n2 = nullptr;  // ERROR: nullptr is NOT convertible to int
    // int* p3 = true ? 0 : nullptr;  // ERROR: incompatible types in ternary

    std::cout << "p1 (NULL)    = " << p1 << "\n";
    std::cout << "p2 (nullptr) = " << p2 << "\n";
    std::cout << "n1 (NULL→int)= " << n1 << "\n";
    std::cout << "Prefer nullptr over NULL — it cannot accidentally bind to int\n";
}

// =============================================================================
// SECTION 3: Conversion Rules
// =============================================================================
//
// Implicit type conversion rules are applied IN ORDER, BEFORE any operation:
//   Let ⊗ = any operation (*, +, /, -, %, etc.)
//
// (A) FLOATING POINT PROMOTION
//       floating_type ⊗ integer_type → floating_type
//
// (B) IMPLICIT INTEGER PROMOTION
//       Any signed/unsigned integral type SMALLER than int ⊗ same → int
//       (char, short, uint8_t, etc. are promoted to int before arithmetic)
//
// (C) SIZE PROMOTION
//       small_type ⊗ large_type → large_type
//
// (D) SIGN PROMOTION
//       signed_type ⊗ unsigned_type → unsigned_type
//
// COMMON PITFALL: integer division truncates toward zero!

void section_conversion_rules() {
    std::cout << "\n=== Conversion Rules ===\n";

    float    f = 1.0f;
    unsigned u = 2;
    int      i = 3;
    short    s = 4;
    uint8_t  c = 5;  // unsigned char

    // (A) Floating point promotion: float × unsigned → float
    auto r1 = f * u;    // float * unsigned → float (2.0f)
    std::cout << "f * u  (float×unsigned→float):    " << r1 << "\n";

    // (B) Implicit integer promotion: short × uint8_t → int
    auto r2 = s * c;    // short × unsigned char → int (20)
    std::cout << "s * c  (short×uint8_t→int):       " << r2 << "\n";

    // (C) Size promotion: unsigned × int → unsigned
    auto r3 = u * i;    // unsigned × int → unsigned (6u)
    std::cout << "u * i  (unsigned×int→unsigned):   " << r3 << "\n";

    // (B) Unary + promotes small integral types to int
    auto r4 = +c;       // uint8_t → int (5)
    std::cout << "+c     (uint8_t→int via unary+):  " << r4 << "\n";

    // INTEGERS ARE NOT FLOATING POINTS — integer division truncates!
    int b = 7;
    float a = b / 2;    // b/2 = 3 (integer division first!), then 3.0f
    std::cout << "\nINTEGER DIVISION TRAP:\n";
    std::cout << "float a = 7 / 2   → " << a << "  (expected 3.5, got 3!)\n";
    float a2 = b / 2.0; // b promoted to double, then 3.5
    std::cout << "float a = 7 / 2.0 → " << a2 << "  (correct: 3.5)\n";

    // Implicit promotion: char (value 48 = '0') printed as character,
    // but after unary + or addition it is promoted to int.
    char ch = 48;         // '0'
    std::cout << "\nIMPLICIT PROMOTION of char:\n";
    std::cout << "char a = 48 → cout prints: " << ch << "\n";         // '0'
    std::cout << "+a promotes to int:        " << +ch << "\n";        // 48
    std::cout << "(a + 0) promotes to int:   " << (ch + 0) << "\n";  // 48

    // uint8_t addition: promoted to int, so no overflow
    uint8_t a1 = 255, b1 = 255;
    std::cout << "\nuint8_t 255 + 255 (promoted to int) = " << (a1 + b1) << "\n"; // 510
}

// =============================================================================
// SECTION 4: auto Keyword (C++11)
// =============================================================================
//
// auto tells the compiler to DEDUCE the type from the initializer expression.
// The variable type is determined at compile time — still statically typed!
//
// Benefits:
//   - Avoids verbose/redundant type names
//   - Useful for complex iterator/template types
//   - Maintainability: changing the right-hand side type propagates automatically
//
// Drawback:
//   - Can hide types, reducing readability if overused
//   - `auto x = 0;` is generally discouraged — x is int, but intent is unclear

// auto with trailing return type (C++11/14)
auto double_it(int x) -> int { return x * 2; }         // C++11: explicit trailing type
auto double_it2(int x) -> decltype(x * 2) { return x * 2; } // C++11: deduced via decltype
auto double_it3(int x) { return x * 2; }               // C++14: full return type deduction

// auto parameter = abbreviated function template (C++20)
// Equivalent to: template<typename T> void print_value(T x)
void print_value(auto x) { std::cout << x << "\n"; }

void section_auto_keyword() {
    std::cout << "\n=== auto Keyword ===\n";

    auto a = 1 + 2;       // int + int = int   → a is int
    auto b = 1 + 2.0;     // int + double = double → b is double
    auto c = 3u;          // unsigned int literal   → c is unsigned int
    auto d = 3.0f;        // float literal          → d is float

    std::cout << "auto a = 1+2    → int,    a=" << a << "  (size=" << sizeof(a) << ")\n";
    std::cout << "auto b = 1+2.0  → double, b=" << b << "  (size=" << sizeof(b) << ")\n";
    std::cout << "auto c = 3u     → uint,   c=" << c << "  (size=" << sizeof(c) << ")\n";
    std::cout << "auto d = 3.0f   → float,  d=" << d << "  (size=" << sizeof(d) << ")\n";

    // Trailing return type (C++11/14)
    std::cout << "\ndouble_it(3)  → " << double_it(3)  << "\n";
    std::cout << "double_it2(3) → " << double_it2(3) << "\n";
    std::cout << "double_it3(3) → " << double_it3(3) << "\n";

    // auto parameters (C++20) — abbreviated function templates
    std::cout << "\nprint_value(42):    "; print_value(42);    // x is int
    std::cout << "print_value(3.14):  "; print_value(3.14);   // x is double
    std::cout << "print_value('A'):   "; print_value('A');     // x is char
}

// =============================================================================
// SECTION 5: C++ Operators
// =============================================================================
//
// OPERATOR PRECEDENCE TABLE (high → low):
//  1.  a++ a--                  Postfix increment/decrement     Left-to-right
//  2.  ++a -a +a --a ! ~ not    Prefix inc/dec, unary, logical  Right-to-left
//  3.  a*b a/b a%b              Multiplication, division, mod    Left-to-right
//  4.  a+b a-b                  Addition, subtraction            Left-to-right
//  5.  << >>                    Bitwise shift                    Left-to-right
//  6.  < <= > >=                Relational operators             Left-to-right
//  7.  == !=                    Equality operators               Left-to-right
//  8.  &                        Bitwise AND                      Left-to-right
//  9.  ^                        Bitwise XOR                      Left-to-right
//  10. |                        Bitwise OR                       Left-to-right
//  11. && (and)                 Logical AND                      Left-to-right
//  12. || (or)                  Logical OR                       Left-to-right
//  13. = += -= *= /= %= ^= &= |= <<= >>=   Assignment/compound  Right-to-left
//  (lowest) ,                   Comma operator                   Left-to-right
//
// Key rules:
//  - Unary > binary standard math > comparison/bitwise/logic > assignment > comma
//  - Bitwise operators have HIGHER precedence than logical operators
//  - Bitwise operators have HIGHER precedence than comparison operators
//    (potential bug: `a & b == c` is parsed as `a & (b == c)`)

void section_operators() {
    std::cout << "\n=== C++ Operators ===\n";

    // -------------------------------------------------------------------------
    // 5a. Operator Precedence Examples
    // -------------------------------------------------------------------------

    int a = 2, b = 3, c = 4, d = 5;

    std::cout << "--- Precedence Examples ---\n";
    // a + b * 4  →  a + (b * 4)  = 2 + 12 = 14
    std::cout << "a + b * 4           = " << (a + b * 4)           << "  [a+(b*4)]\n";

    // a * b / c % d  →  ((a * b) / c) % d  = (6/4)%5 = 1%5 = 1
    std::cout << "a * b / c % d       = " << (a * b / c % d)       << "  [((a*b)/c)%d]\n";

    // a + b < 3 >> 4  →  (a + b) < (3 >> 4)  = 5 < 0 = false (0)
    std::cout << "a + b < 3 >> 4      = " << (a + b < 3 >> 4)      << "  [(a+b)<(3>>4)]\n";

    // a && b && c || d  →  ((a && b) && c) || d  = true || true = true
    std::cout << "a && b && c || d    = " << (a && b && c || d)    << "  [((a&&b)&&c)||d]\n";

    // a and b and c or d  — keyword aliases for && and ||
    std::cout << "a and b and c or d  = " << (a and b and c or d)  << "  [same as above]\n";

    // Bitwise vs logical precedence example:
    // a | b & c || d && e  →  (a | (b & c)) || (d && e)
    int e = 1;
    std::cout << "a|b&c||d&&e         = " << (a | b & c || d && e) << "  [(a|(b&c))||(d&&e)]\n";

    // -------------------------------------------------------------------------
    // 5b. Prefix and Postfix Increment/Decrement
    //
    // PREFIX  ++i / --i:
    //   (1) Update the value
    //   (2) Return the NEW (updated) value
    //
    // POSTFIX i++ / i--:
    //   (1) Save the OLD value (temporary copy)
    //   (2) Update the value
    //   (3) Return the OLD (original) value
    // -------------------------------------------------------------------------

    std::cout << "\n--- Prefix vs Postfix Increment ---\n";
    int x = 5;
    std::cout << "x = 5\n";
    std::cout << "++x (prefix)  → returns " << ++x << ", x is now " << x << "\n"; // returns 6
    x = 5;
    std::cout << "x++ (postfix) → returns " << x++ << ", x is now " << x << "\n"; // returns 5

    x = 5;
    std::cout << "--x (prefix)  → returns " << --x << ", x is now " << x << "\n"; // returns 4
    x = 5;
    std::cout << "x-- (postfix) → returns " << x-- << ", x is now " << x << "\n"; // returns 5

    // -------------------------------------------------------------------------
    // 5c. Operation Ordering — Defined Behavior (C++11/17)
    //
    // Several expressions that were UNDEFINED BEHAVIOR before C++11 or C++17
    // now have well-defined results:
    //
    //  i = ++i + 2;  // UB until C++11; since C++11: i = 3 (if i was 0)
    //  i = i++ + 2;  // UB until C++17; since C++17: i = 3 (if i was 0)
    //  f(i=2, i=1);  // UB until C++17; since C++17: i = 2
    //  a[i] = ++i;   // UB until C++17; since C++17: a[1] = 1 (if i was 0)
    //
    //  Still UNDEFINED BEHAVIOR in all standards:
    //  f(++i, ++i);       // UB
    //  i = ++i + i++;     // UB
    // -------------------------------------------------------------------------

    std::cout << "\n--- Operation Ordering ---\n";
    int i2 = 0;
    i2 = ++i2 + 2;   // C++11: well-defined, i2 = 3
    std::cout << "i=0; i = ++i + 2;  → i = " << i2 << "  (C++11: defined)\n";

    int i3 = 0;
    i3 = i3++ + 2;   // C++17: well-defined, i3 = 3 (old value 0 + 2, then stored to i3)
    std::cout << "i=0; i = i++ + 2;  → i = " << i3 << "  (C++17: defined)\n";

    // -------------------------------------------------------------------------
    // 5d. Assignment, Compound, and Comma Operators
    //
    // ASSIGNMENT / COMPOUND ASSIGNMENT:
    //   - Right-to-left associativity
    //   - Returns the assigned value (lvalue)
    //   - Compound: +=, -=, *=, /=, %=, &=, |=, ^=, <<=, >>=
    //
    // COMMA OPERATOR:
    //   - Left-to-right associativity
    //   - Evaluates left expression, DISCARDS result, returns right expression
    //   - Lowest precedence of all operators
    // -------------------------------------------------------------------------

    std::cout << "\n--- Assignment & Compound Operators ---\n";
    int y = 2;
    int xx = y = 3;   // right-to-left: y=3 first, then xx=3
    std::cout << "int xx = y = 3 → xx=" << xx << ", y=" << y << "\n";

    if (xx = 4)       // assigns 4 to xx, then evaluates as bool (true)
        std::cout << "if (xx = 4) is true; xx=" << xx << "\n";

    int val = 10;
    val += 5;   std::cout << "val += 5  → " << val << "\n";  // 15
    val -= 3;   std::cout << "val -= 3  → " << val << "\n";  // 12
    val *= 2;   std::cout << "val *= 2  → " << val << "\n";  // 24
    val /= 4;   std::cout << "val /= 4  → " << val << "\n";  // 6
    val %= 4;   std::cout << "val %= 4  → " << val << "\n";  // 2

    std::cout << "\n--- Comma Operator ---\n";
    int aa = 5, bb = 7;
    int cx = (3, 4);       // evaluates 3 (discards), then returns 4
    std::cout << "cx = (3, 4)    → cx=" << cx << "\n";  // 4

    int zy = 0, z;
    z = zy, aa;            // comma: z=zy (z=0), then aa (result discarded by stmt)
    std::cout << "z=y,x → z=" << z << "  (z=y(0), then x(5) is discarded)\n";
    (void)bb; (void)aa;    // suppress unused-variable warnings

    // -------------------------------------------------------------------------
    // 5e. Spaceship Operator <=> (C++20)
    //
    // The THREE-WAY COMPARISON OPERATOR (a.k.a. "spaceship operator") compares
    // two values and returns an ordering object that can be compared to 0:
    //   result < 0   →  left < right
    //   result == 0  →  left == right
    //   result > 0   →  left > right
    //
    // Return type for integers/pointers: std::strong_ordering
    // Return type for floats: std::partial_ordering (NaN cases)
    // -------------------------------------------------------------------------

    std::cout << "\n--- Spaceship Operator <=> (C++20) ---\n";

    auto r_lt = (3 <=> 5);   // 3 < 5  → negative (less)
    auto r_eq = ('a' <=> 'a'); // equal → 0
    auto r_gt = (7 <=> 5);   // 7 > 5  → positive (greater)

    std::cout << "(3 <=> 5)  == 0 : " << std::boolalpha << ((3 <=> 5) == 0) << "\n"; // false
    std::cout << "('a'<=>'a')== 0 : " << (('a' <=> 'a') == 0) << "\n";              // true
    std::cout << "(3 <=> 5)  < 0  : " << ((3 <=> 5) < 0) << "\n";                   // true
    std::cout << "(7 <=> 5)  < 0  : " << ((7 <=> 5) < 0) << "\n";                   // false

    (void)r_lt; (void)r_eq; (void)r_gt;

    // -------------------------------------------------------------------------
    // 5f. Safe Comparison Operators (C++20)
    //
    // <utility> provides functions to safely compare integers of DIFFERENT types,
    // especially mixed signed/unsigned comparisons which are error-prone:
    //
    //   std::cmp_equal(a, b)
    //   std::cmp_not_equal(a, b)
    //   std::cmp_less(a, b)
    //   std::cmp_greater(a, b)
    //   std::cmp_less_equal(a, b)
    //   std::cmp_greater_equal(a, b)
    //
    // Without these, comparing signed and unsigned can give WRONG results:
    //   unsigned a = 4; int b = -3;
    //   a > b   →  FALSE! (-3 is converted to a huge unsigned number)
    //   std::cmp_greater(a, b) → TRUE  (correct mathematical answer)
    // -------------------------------------------------------------------------

    std::cout << "\n--- Safe Comparison Operators (C++20) ---\n";
    unsigned int ua = 4;
    int          ib = -3;

    bool v1 = (ua > ib);                      // WRONG: -3 wraps to large unsigned!
    bool v2 = std::cmp_greater(ua, ib);       // CORRECT: true (4 > -3 mathematically)

    std::cout << "unsigned 4, int -3:\n";
    std::cout << "  (a > b) via regular >       = " << v1 << "  (WRONG: unsigned wrap)\n";
    std::cout << "  std::cmp_greater(a, b)      = " << v2 << "  (CORRECT)\n";

    std::cout << "  std::cmp_less(ua, ib)       = " << std::cmp_less(ua, ib) << "\n";
    std::cout << "  std::cmp_equal(ua, 4u)      = " << std::cmp_equal(ua, 4u) << "\n";
    std::cout << "  std::cmp_not_equal(ua, ib)  = " << std::cmp_not_equal(ua, ib) << "\n";
}

// =============================================================================
// MAIN
// =============================================================================

int main() {
    std::cout << "=== Unit 3: Basic Concepts I ===\n";
    std::cout << "    Type System, Fundamental Types, and Operators\n";

    section_fundamental_types();
    section_conversion_rules();
    section_auto_keyword();
    section_operators();

    std::cout << "\n=== Done ===\n";
    return 0;
}
