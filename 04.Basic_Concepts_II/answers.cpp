/**
 * Unit 4: Basic Concepts II — Practice Question Answers
 *
 * Compile: g++ -std=c++17 -Wall -Wextra -Wpedantic -o answers answers.cpp
 * Run:     ./answers
 *
 * Conceptual answers are in detailed comments above each function.
 * Every question with a code component has a runnable demonstration.
 */

#include <algorithm>   // std::max
#include <bitset>
#include <cfenv>
#include <climits>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <limits>
#include <numeric>
#include <vector>

// =============================================================================
// Q1 — C++ Data Models
// =============================================================================
// Three data models (name | OS | key property):
//   ILP32  | Windows/Unix 32-bit  | int=32, long=32, pointer=32
//   LLP64  | Windows 64-bit       | int=32, long=32, pointer=64
//   LP64   | Linux/macOS 64-bit   | int=32, long=64, pointer=64
//
// Key difference: the size of `long` (32 on Windows, 64 on Linux) and
// the pointer size (32-bit on ILP32, 64-bit on LLP64/LP64).
// `long` differs most significantly across platforms.

void answer_q1() {
    std::cout << "\n--- Q1: C++ Data Models ---\n";
    std::cout << "  sizeof(short)   = " << sizeof(short)   * 8 << " bits\n";
    std::cout << "  sizeof(int)     = " << sizeof(int)     * 8 << " bits\n";
    std::cout << "  sizeof(long)    = " << sizeof(long)    * 8 << " bits  ← differs by platform!\n";
    std::cout << "  sizeof(pointer) = " << sizeof(void*)   * 8 << " bits\n";

    if (sizeof(long) == 4 && sizeof(void*) == 8)
        std::cout << "  → LLP64 (Windows 64-bit): long is 32-bit\n";
    else if (sizeof(long) == 8)
        std::cout << "  → LP64 (Linux/macOS): long is 64-bit\n";
}

// =============================================================================
// Q2 — Size guarantees
// =============================================================================
// `char` is guaranteed to be exactly 1 byte.
// Standard guarantees: sizeof(char) ≤ sizeof(short) ≤ sizeof(int)
//                                   ≤ sizeof(long)  ≤ sizeof(long long)
// (all "≤", not strict inequality — sizes may be equal)

// =============================================================================
// Q3 — `long` portability trap
// =============================================================================
// Code using `long` as a 64-bit type works on Linux/macOS (LP64) but BREAKS
// silently on Windows 64-bit (LLP64) where `long` is only 32 bits.
// Fix: use `int64_t` from <cstdint>.

// =============================================================================
// Q4 — Why fixed-width types if `int`/`long` exist?
// =============================================================================
// Because `int` and `long` have platform-dependent sizes. Fixed-width types
// (int32_t, int64_t, etc.) have the SAME size on any architecture, making
// code portable. Essential for binary protocols, file formats, cross-platform
// algorithms, and interfacing with hardware or external libraries.

// =============================================================================
// Q5 — int*_t as typedefs and overload resolution
// =============================================================================
// int32_t might be `int` on one platform and `long` on another.
// If a function is overloaded for both `int` and `long`, passing an `int32_t`
// may call different overloads on different platforms — undefined behavior
// masked as a portability bug. Example:
//   void f(int);  void f(long);
//   int32_t x = 5;
//   f(x);  // calls f(int) on Linux, f(long) on Windows potentially

// =============================================================================
// Q6 — int8_t / uint8_t I/O stream gotcha
// =============================================================================
// int8_t is a typedef for signed char. cout treats it as a character.
// var = 50 is the ASCII code for '2' — so cout << var prints '2' (the char).
// var + 0 promotes var to int via integer promotion → prints 50.

void answer_q6() {
    std::cout << "\n--- Q6: int8_t I/O gotcha ---\n";
    int8_t var = 50;   // ASCII '2'
    std::cout << "  var        = '" << var      << "'  (char: '2')\n";
    std::cout << "  var + 0    = "  << (var + 0) << "    (int: 50)\n";
    std::cout << "  +var       = "  << (+var)    << "    (int via unary+)\n";
    std::cout << "  (int)var   = "  << static_cast<int>(var) << "    (explicit cast)\n";
}

// =============================================================================
// Q7 — size_t vs ptrdiff_t
// =============================================================================
// size_t:
//   - Header: <cstddef> (also available via <cstdint>, <cstring>, etc.)
//   - UNSIGNED (at least 16 bits; 4 bytes on 32-bit, 8 bytes on 64-bit)
//   - Return type of sizeof(); used for sizes, counts, array indices
//   - C++23: `uz`/`UZ` literals, e.g., 5uz
//
// ptrdiff_t:
//   - Header: <cstddef>
//   - SIGNED (same size as size_t)
//   - Used to hold the result of subtracting two pointers/iterators
//   - C++23: `z`/`Z` literals, e.g., 5z

void answer_q7() {
    std::cout << "\n--- Q7: size_t vs ptrdiff_t ---\n";
    std::cout << "  sizeof(size_t)    = " << sizeof(size_t)    << " bytes (unsigned)\n";
    std::cout << "  sizeof(ptrdiff_t) = " << sizeof(ptrdiff_t) << " bytes (signed)\n";

    int arr[5] = {};
    ptrdiff_t diff = &arr[4] - &arr[0];
    std::cout << "  &arr[4] - &arr[0] = " << diff << " (ptrdiff_t)\n";

    size_t sz = sizeof(arr) / sizeof(arr[0]);
    std::cout << "  sizeof(arr)/sizeof(arr[0]) = " << sz << " (size_t)\n";
}

// =============================================================================
// Q8 — uintptr_t
// =============================================================================
// uintptr_t is an integer type capable of holding a void* pointer.
// Guaranteed properties:
//   (1) sizeof(uintptr_t) == sizeof(void*)
//   (2) A void* can be converted to uintptr_t and back without loss
// Optional caveat: it is NOT required by the standard — compilers may omit it.
// Use case: pointer arithmetic expressed as integers (e.g., alignment checks).

void answer_q8() {
    std::cout << "\n--- Q8: uintptr_t ---\n";
    int value = 42;
    uintptr_t addr = reinterpret_cast<uintptr_t>(&value);
    std::cout << "  sizeof(uintptr_t) = " << sizeof(uintptr_t) << " bytes\n";
    std::cout << "  sizeof(void*)     = " << sizeof(void*)     << " bytes\n";
    std::cout << "  address as int:   0x" << std::hex << addr << std::dec << "\n";
    std::cout << "  alignment check (16-byte): " << std::boolalpha << ((addr % 16) == 0) << "\n";
}

// =============================================================================
// Q9 — Arithmetic operation semantics
// =============================================================================
// OVERFLOW:    Result exceeds representable range.
//              C++ signed integers → UNDEFINED BEHAVIOR
//
// WRAPAROUND:  Result = mathematical_result mod 2^N
//              C++ unsigned integers → WELL-DEFINED (always wraps)
//
// SATURATION:  Result clamped to [min, max]
//              C++26: std::add_sat, sub_sat, mul_sat, div_sat, saturate_cast

void answer_q9() {
    std::cout << "\n--- Q9: Overflow / Wraparound / Saturation ---\n";
    // Wraparound (unsigned, well-defined):
    unsigned umax = std::numeric_limits<unsigned>::max();
    std::cout << "  UINT_MAX + 1u = " << (umax + 1u) << "  (wraps to 0)\n";
    std::cout << "  0u - 1u       = " << (0u - 1u)   << "  (wraps to UINT_MAX)\n";

    // Saturation (simulated):
    auto sat_add_u = [](unsigned a, unsigned b) -> unsigned {
        unsigned r = a + b;
        return (r < a) ? std::numeric_limits<unsigned>::max() : r;
    };
    std::cout << "  sat_add(UINT_MAX,5) = " << sat_add_u(umax, 5u) << "  (clamped at max)\n";
    // Signed overflow: shown as comment only — actual UB
    // int overflow = INT_MAX + 1;  // UB — do not run
}

// =============================================================================
// Q10 — UINT_MAX + 1u
// =============================================================================
// Value: 0 (wraps to zero, modulo 2^32)
// This is NOT undefined behavior — unsigned arithmetic is defined to wrap.
// The C++ standard explicitly guarantees wraparound for unsigned types.

void answer_q10() {
    std::cout << "\n--- Q10: UINT_MAX + 1u ---\n";
    unsigned a = std::numeric_limits<unsigned>::max();
    std::cout << "  UINT_MAX     = " << a << "\n";
    std::cout << "  UINT_MAX + 1 = " << (a + 1u) << "  (wraps, NOT UB)\n";
}

// =============================================================================
// Q11 — C++26 saturation arithmetic (five functions)
// =============================================================================
// std::add_sat(x, y)        — saturating addition
// std::sub_sat(x, y)        — saturating subtraction
// std::mul_sat(x, y)        — saturating multiplication
// std::div_sat(x, y)        — saturating division
// std::saturate_cast<R>(x)  — clamped conversion to type R
//
// add_sat(T x, T y): instead of wrapping (unsigned) or UB (signed),
// the result is clamped to numeric_limits<T>::max() if it would exceed max,
// or numeric_limits<T>::min() if it would go below min.

// =============================================================================
// Q12 — Four UB categories for signed integer arithmetic
// =============================================================================
// 1. OVERFLOW/UNDERFLOW:         int x = INT_MAX + 1;         // UB
// 2. OUT-OF-RANGE INITIALIZATION: int z = 3000000000;         // UB (> INT_MAX)
// 3. BITWISE SHIFT:
//    a. Shift amount ≥ bit width:  1 << 32                    // UB
//    b. Negative operand shift:    -1 << 12                   // UB until C++20
//    c. Negative shift amount:     1 << -1                    // UB always
// 4. OPERATIONS ON SPECIAL VALUES:
//    - INT_MIN / -1 → result 2^31 doesn't fit → UB
//    - INT_MIN % -1 → same issue                             // UB

void answer_q12() {
    std::cout << "\n--- Q12: UB categories (safe demos in comments) ---\n";
    std::cout << "  1. Signed overflow: int x = INT_MAX + 1;  // UB — not run\n";
    std::cout << "  2. Out-of-range:    int z = 3000000000;   // UB — not run\n";
    std::cout << "  3a. Shift too far:  1 << 32;              // UB always\n";
    std::cout << "  3b. Neg operand:    -1 << 12;             // UB until C++20\n";
    std::cout << "  3c. Neg amount:     1 << -1;              // UB always\n";
    std::cout << "  4. INT_MIN * -1:    result 2^31 overflows // UB\n";

    // Safe shift examples:
    int pos = 1;
    std::cout << "  Safe: 1 << 30 = " << (pos << 30) << "\n";
}

// =============================================================================
// Q13 — INT_MAX * -1 vs INT_MIN * -1
// =============================================================================
// INT_MAX = 2^31 - 1 = 2147483647
// INT_MAX * -1 = -2147483647 → fits in int (INT_MIN = -2147483648 < -2147483647)
//
// INT_MIN = -2^31 = -2147483648
// INT_MIN * -1 = +2147483648 = 2^31 → DOES NOT FIT in int (max is 2^31 - 1)
// → Undefined behavior

void answer_q13() {
    std::cout << "\n--- Q13: INT_MAX * -1 vs INT_MIN * -1 ---\n";
    std::cout << "  INT_MAX         = " << INT_MAX << "\n";
    std::cout << "  INT_MIN         = " << INT_MIN << "\n";
    std::cout << "  INT_MAX * -1    = " << (INT_MAX * -1) << "  (OK: fits in int)\n";
    // INT_MIN * -1 → UB — do NOT execute
    std::cout << "  INT_MIN * -1    = UB (result = 2^31, exceeds INT_MAX)\n";
}

// =============================================================================
// Q14 — Shift UB in C++17 vs C++20
// =============================================================================
// (a) int y = -1 << 12;    — UB until C++20 (negative value left-shifted)
//                            Well-defined in C++20 (two's complement guaranteed)
// (b) int z = 1 << -12;    — UB always (negative shift amount)
// (c) unsigned w = 1u<<32u; — UB always (shift amount ≥ bit width)
// (d) int v = 1 << 30;     — WELL-DEFINED (result fits in int, 2^30 = 1073741824)

void answer_q14() {
    std::cout << "\n--- Q14: Shift UB ---\n";
    std::cout << "  (a) -1 << 12:   UB in C++17, well-defined in C++20\n";
    std::cout << "  (b) 1 << -12:   UB always (negative shift amount)\n";
    std::cout << "  (c) 1u << 32u:  UB always (shift >= bit width)\n";
    int v = 1 << 30;
    std::cout << "  (d) 1 << 30:    " << v << "  (well-defined)\n";
}

// =============================================================================
// Q15 — Linux kernel signed overflow UB / optimizer elimination
// =============================================================================
// The code: if (offset + len < 0) return -EINVAL;
// The compiler knows that signed integer overflow is UB. Therefore it is
// PERMITTED to assume signed addition never overflows. Given that guarantee,
// if both offset and len have passed earlier non-negative checks
// (offset >= 0 after the first guard), the compiler concludes the sum is
// always non-negative → the check (sum < 0) is always false → eliminates it.
// BUG: with very large values, offset + len DOES overflow at hardware level
// and produces a negative number, but the compiler-generated code never checks.

// =============================================================================
// Q16 — Signed overflow on hardware
// =============================================================================
// YES, on two's complement hardware (virtually all modern CPUs), signed
// overflow produces a well-defined bit pattern. However, C++ declares it UB
// so the compiler can apply optimizations that assume it never happens:
//   - Eliminate redundant overflow checks
//   - Assume loop induction variables don't overflow (enable vectorization)
//   - Constant-fold expressions involving signed arithmetic
// Using __builtin_sadd_overflow (GCC/Clang) or -fwrapv flag opts into
// defined wraparound at the cost of those optimizations.

// =============================================================================
// Q17 — Unsigned associativity
// =============================================================================
// Unsigned addition is associative modulo 2^N: (a+b)+c ≡ a+(b+c) (mod 2^N)
// For signed integers, intermediate overflow is UB, so the associative
// rearrangement is only valid for the compiler when no overflow can occur.
// Because signed overflow is UB, the compiler cannot always assume the
// rearrangement is safe, and the math differs when intermediates overflow.

void answer_q17() {
    std::cout << "\n--- Q17: Unsigned associativity ---\n";
    unsigned a = UINT_MAX, b = 1, c = 1;
    unsigned r1 = (a + b) + c;   // (UINT_MAX + 1) + 1 = 0 + 1 = 1
    unsigned r2 = a + (b + c);   // UINT_MAX + 2 = 1 (same, wraps same way)
    std::cout << "  (UINT_MAX+1)+1 = " << r1 << "\n";
    std::cout << "  UINT_MAX+(1+1) = " << r2 << "\n";
    std::cout << "  Equal: " << std::boolalpha << (r1 == r2) << "\n";
}

// =============================================================================
// Q18 — unsigned x = 0; x -= 1;
// =============================================================================
// Output: 4294967295 (UINT_MAX). Defined behavior — unsigned wraparound.

void answer_q18() {
    std::cout << "\n--- Q18: unsigned 0 -= 1 ---\n";
    unsigned x = 0;
    x -= 1;
    std::cout << "  Result: " << x << "  (UINT_MAX = 2^32 - 1, well-defined)\n";
}

// =============================================================================
// Q19 — size_t loop underflow bug
// =============================================================================
// BUG: When v.size() == 0, v.size() - 1 underflows from 0 to SIZE_MAX
// (unsigned wraparound), making the loop condition `0 < SIZE_MAX` true.
// The loop body then accesses v[0] on an empty vector — undefined behavior.
//
// FIXES:
//   1. if (!v.empty()) { for (size_t i = 0; i+1 < v.size(); i++) ... }
//   2. for (size_t i = 0; i + 1 < v.size(); i++) ...  (avoids v.size()-1)
//   3. for (int i = 0; i < (int)v.size()-1; i++) ...  (signed loop variable)

void answer_q19() {
    std::cout << "\n--- Q19: size_t loop bug ---\n";
    std::vector<int> empty;
    size_t sz = empty.size();
    std::cout << "  empty.size()     = " << sz << "\n";
    std::cout << "  empty.size() - 1 = " << (sz - 1) << "  (SIZE_MAX — bug!)\n";

    // Safe version:
    std::vector<int> v = {1, 2, 3, 4, 5};
    std::cout << "  Safe loop (i+1 < v.size()): ";
    for (size_t i = 0; i + 1 < v.size(); i++)
        std::cout << "(" << v[i] << "," << v[i+1] << ") ";
    std::cout << "\n";
}

// =============================================================================
// Q20 — Google Style Guide / Bjarne Stroustrup recommendation
// =============================================================================
// Bjarne Stroustrup: "Subscripts and sizes should be signed."
// Google Style Guide recommends int64_t as the default size type.
// Max value: 2^63 - 1 = 9,223,372,036,854,775,807
//   ≈ 9 quintillion (9 billion of billions)
//   ≈ 292 years in nanoseconds
//   ≈ 9 million terabytes

void answer_q20() {
    std::cout << "\n--- Q20: int64_t recommendation ---\n";
    std::cout << "  int64_t max = " << std::numeric_limits<int64_t>::max() << "\n";
    std::cout << "  ≈ 292 years in nanoseconds\n";
    std::cout << "  ≈ 9 million terabytes\n";
}

// =============================================================================
// Q21 — When to use signed vs unsigned
// =============================================================================
// USE SIGNED:
//   1. Values may be negative (e.g., differences, offsets, error codes)
//   2. Overflow in loops can be exploited for optimization (IV analysis)
//
// USE UNSIGNED:
//   1. Bitmask values (bit flags, masks) — bitwise ops semantically unsigned
//   2. Safety-critical: signed overflow UB is "non-deterministic"; unsigned
//      wraparound is well-defined and predictable

// =============================================================================
// Q22 — numeric_limits expressions
// =============================================================================

void answer_q22() {
    std::cout << "\n--- Q22: numeric_limits ---\n";
    std::cout << "  Largest int:           " << std::numeric_limits<int>::max()       << "\n";
    std::cout << "  Smallest (neg) float:  " << std::numeric_limits<float>::lowest()  << "\n";
    std::cout << "  Smallest pos normal f: " << std::numeric_limits<float>::min()     << "\n";
    std::cout << "  double epsilon:        " << std::numeric_limits<double>::epsilon() << "\n";
}

// =============================================================================
// Q23 — Promotion and truncation values
// =============================================================================
// (a) int16_t x = -1;  int y = x;
//     Sign-extends: all upper bits become 1 → y = -1
//
// (b) int z = 32769;   int16_t w = z;
//     Truncation: 32769 % 2^16 = 32769. As int16_t (signed):
//     32769 = 0x8001; bit 15 is set → interpreted as negative: 32769 - 65536 = -32767

void answer_q23() {
    std::cout << "\n--- Q23: Promotion and Truncation ---\n";
    int16_t x_a = -1;
    int     y_a = x_a;   // sign-extend: y = -1
    std::cout << "  (a) int16_t(-1) → int: " << y_a << "  (sign-extended)\n";

    int     z_b = 32769;
    auto    w_b = static_cast<int16_t>(z_b);  // truncate and reinterpret
    std::cout << "  (b) int(32769) → int16_t: " << w_b << "  (32769 - 65536 = -32767)\n";
}

// =============================================================================
// Q24 — Truncation as modulo
// =============================================================================
// When storing a value in a smaller type, only the low N bits survive (where N
// is the target type's bit width). This is equivalent to: value mod 2^N.
// For signed targets, the retained bit pattern is then reinterpreted in
// two's complement (so high bit set → negative).

// =============================================================================
// Q25 — Dangerous array index
// =============================================================================
// unsigned a = 10; int b = -1;
// b is implicitly converted to unsigned before multiplication.
// -1 as unsigned = UINT_MAX = 4294967295
// a * UINT_MAX (as uint32): wraps → 4294967286
// 10ull + 4294967286 = 4294967296 (a massive index → segfault if dereferenced)
// This is NOT UB (unsigned arithmetic is well-defined); the SEGFAULT from
// accessing that index is UB.

void answer_q25() {
    std::cout << "\n--- Q25: Dangerous array index ---\n";
    unsigned a = 10;
    int      b = -1;
    unsigned product  = a * static_cast<unsigned>(b);  // unsigned wraparound
    uint64_t bad_idx  = 10ull + static_cast<uint64_t>(product);
    std::cout << "  -1 as unsigned      = " << static_cast<unsigned>(b) << "\n";
    std::cout << "  10u * UINT_MAX wraps = " << product  << "\n";
    std::cout << "  10ull + product      = " << bad_idx  << "  (massive — SEGFAULT if dereferenced)\n";
    std::cout << "  Arithmetic itself: well-defined; dereferencing that index: UB\n";
}

// =============================================================================
// Q26 — f(-1, 5, arr) signed/unsigned comparison
// =============================================================================
// int a = -1; unsigned b = 5;
// (a > b): sign promotion rule — a (-1) is converted to unsigned.
// -1 as unsigned = UINT_MAX. UINT_MAX > 5 → TRUE.
// So the function takes the branch: return arr[-1 - 5] = arr[UINT_MAX-4] → SEGFAULT.
// The "safety" check `if (a > b)` is meaningless when a is negative.

void answer_q26() {
    std::cout << "\n--- Q26: f(-1, 5, arr) ---\n";
    int sa = -1; unsigned ub = 5;
    bool comparison = (static_cast<unsigned>(sa) > ub);  // -1 → UINT_MAX > 5
    std::cout << "  (int)-1 as unsigned = " << static_cast<unsigned>(sa) << "\n";
    std::cout << "  UINT_MAX > 5        = " << std::boolalpha << comparison << "  (wrong! branch taken)\n";
    std::cout << "  arr[-1 - 5] would access arr[UINT_MAX-4] → SEGFAULT\n";
}

// =============================================================================
// Q27 — Sign promotion and std::cmp_greater
// =============================================================================
// Rule D from Unit 3: signed_type op unsigned_type → unsigned_type
// This converts negative signed values to huge unsigned ones before comparison.
// Fix: std::cmp_greater, std::cmp_less, etc. from <utility> (C++20).

void answer_q27() {
    std::cout << "\n--- Q27: Safe signed/unsigned comparison ---\n";
    int      neg = -1;
    unsigned pos = 10;
    // Regular: wrong
    bool wrong   = (static_cast<unsigned>(neg) > pos);
    // Safe (C++20):
    // bool correct = std::cmp_greater(neg, pos);  // requires C++20 <utility>
    std::cout << "  (unsigned)(-1) > 10 = " << std::boolalpha << wrong << "  (wrong — UB check: use cmp_greater)\n";
    std::cout << "  Fix: std::cmp_greater(neg, pos) from <utility> (C++20)\n";
}

// =============================================================================
// Q28 — Before vs after overflow detection
// =============================================================================
// UNSIGNED: wraparound is well-defined, so you CAN perform the operation
// and then check if the result wrapped (e.g., result < operand).
//
// SIGNED: overflow is UB. Once the overflow occurs, the compiler may have
// already optimized away any subsequent check as dead code (since it assumes
// overflow never happens). Detection must happen BEFORE the operation using
// the operand values and numeric limits.

// =============================================================================
// Q29 — Overflow detection functions
// =============================================================================

bool is_add_overflow_u(unsigned a, unsigned b) {
    return (a + b) < a || (a + b) < b;
    // If addition wrapped, the result is smaller than either operand.
}

bool is_mul_overflow_u(unsigned a, unsigned b) {
    unsigned x = a * b;
    return a != 0 && (x / a) != b;
    // If multiplication overflowed, dividing back doesn't recover b.
}

void answer_q29() {
    std::cout << "\n--- Q29: Overflow detection ---\n";
    std::cout << "  is_add_overflow(UINT_MAX, 1) = " << std::boolalpha << is_add_overflow_u(UINT_MAX, 1u) << "\n";
    std::cout << "  is_add_overflow(10, 20)      = " << is_add_overflow_u(10u, 20u) << "\n";
    std::cout << "  is_mul_overflow(UINT_MAX, 2) = " << is_mul_overflow_u(UINT_MAX, 2u) << "\n";
    std::cout << "  is_mul_overflow(10, 20)      = " << is_mul_overflow_u(10u, 20u) << "\n";
}

// =============================================================================
// Q30 — signed_add_safe
// =============================================================================

bool signed_add_safe(int a, int b) {
    if (b > 0 && a > (INT_MAX - b)) return true;   // positive overflow
    if (b < 0 && a < (INT_MIN - b)) return true;   // negative underflow
    return false;  // no overflow
}

void answer_q30() {
    std::cout << "\n--- Q30: signed_add_safe ---\n";
    std::cout << "  would_overflow(INT_MAX, 1)   = " << std::boolalpha << signed_add_safe(INT_MAX, 1) << "\n";
    std::cout << "  would_overflow(INT_MIN, -1)  = " << signed_add_safe(INT_MIN, -1) << "\n";
    std::cout << "  would_overflow(10, 20)        = " << signed_add_safe(10, 20) << "\n";
}

// =============================================================================
// Q31 — IEEE 754 format table
// =============================================================================
// | Format   | Bits | Sign | Exponent | Mantissa | C++ Type (C++23)   |
// |----------|------|------|----------|----------|--------------------|
// | Single   |  32  |   1  |    8     |    23    | float              |
// | Double   |  64  |   1  |   11     |    52    | double             |
// | float16  |  16  |   1  |    5     |    10    | std::float16_t     |
// | bfloat16 |  16  |   1  |    8     |     7    | std::bfloat16_t    |

// =============================================================================
// Q32 — Exponent bias in single-precision IEEE 754
// =============================================================================
// Bias = 127 for float. The stored exponent e_stored is in [0, 255].
// Actual exponent = e_stored - 127, so actual range = [-127, +128].
// Reserved values:
//   e_stored = 0  (all 0s) → zero (mantissa=0) or denormal (mantissa≠0)
//   e_stored = 255 (all 1s) → ±infinity (mantissa=0) or NaN (mantissa≠0)
// Normal numbers use e_stored ∈ [1, 254], actual ∈ [-126, +127].

// =============================================================================
// Q33 — Decode 0 | 10000111 | 11000000000000000000000
// =============================================================================
// Sign:     0 → positive (+)
// Exponent: 10000111b = 135; actual = 135 - 127 = 8 → multiply by 2^8 = 256
// Mantissa: 11000000...0 → implicit leading 1: 1.11b
//           1.11b = 1 + 1/2 + 1/4 = 1.75
// Value: +1.75 × 256 = 448.0

void answer_q33() {
    std::cout << "\n--- Q33: Decode float bits for 448.0f ---\n";
    float    f = 448.0f;
    uint32_t bits;
    std::memcpy(&bits, &f, sizeof(bits));

    uint32_t sign     = (bits >> 31) & 0x1;
    uint32_t exponent = (bits >> 23) & 0xFF;
    uint32_t mantissa =  bits        & 0x7FFFFF;
    int      actual_e = static_cast<int>(exponent) - 127;

    std::cout << "  Bits:       " << std::bitset<32>(bits) << "\n";
    std::cout << "  Sign:       " << sign << " → +\n";
    std::cout << "  Exponent:   " << std::bitset<8>(exponent) << " = " << exponent
              << " - 127 = " << actual_e << " → 2^" << actual_e << " = " << (1 << actual_e) << "\n";
    std::cout << "  Mantissa:   " << std::bitset<23>(mantissa) << " → 1.11b = 1.75\n";
    std::cout << "  Value:      1.75 × " << (1 << actual_e) << " = " << f << "\n";
}

// =============================================================================
// Q34 — Denormal (subnormal) numbers
// =============================================================================
// A DENORMAL number has exponent field = all 0s and non-zero mantissa.
// They exist to fill the "underflow gap" — the space between 0.0 and the
// smallest normal number. Without denormals, underflowing computations
// jump abruptly from the smallest normal value directly to zero (a gap of
// ~1.2×10^-38 for float). Denormals allow "gradual underflow."
// The implicit leading bit is 0 (not 1), giving: 0.mantissa × 2^(-126)

void answer_q34() {
    std::cout << "\n--- Q34: Denormal numbers ---\n";
    float normal  = std::numeric_limits<float>::min();
    float denorm  = std::numeric_limits<float>::denorm_min();
    std::cout << "  Smallest normal float:    " << normal  << "\n";
    std::cout << "  Smallest denormal float:  " << denorm  << "\n";
    std::cout << "  isnormal(normal):  " << std::boolalpha << std::isnormal(normal)  << "\n";
    std::cout << "  isnormal(denorm):  " << std::isnormal(denorm)  << "  (it is denormal)\n";
}

// =============================================================================
// Q35 — Operations generating inf and NaN
// =============================================================================
// INFINITY: ±∞ × ±∞,  ±∞ × finite_nonzero,  finite / ±0,  finite > max_value
// NaN: any op with NaN,  ±∞ × ∓∞,  0/0,  ∞/∞,  √negative,  log(negative),
//      sin⁻¹(x) for |x|>1,  ∞-∞

// =============================================================================
// Q36 — Special value results
// =============================================================================

void answer_q36() {
    std::cout << "\n--- Q36: Special value operations ---\n";
    float inf = std::numeric_limits<float>::infinity();
    std::cout << "  (a) 5.0f / 0.0f   = " << (5.0f / 0.0f) << "  (+inf)\n";
    std::cout << "  (b) -5.0f / 0.0f  = " << (-5.0f / 0.0f) << "  (-inf)\n";
    std::cout << "  (c) 0.0f / 0.0f   = " << (0.0f / 0.0f)  << "  (NaN: 0/0)\n";
    std::cout << "  (d) inf * 0.0f    = " << (inf * 0.0f)    << "  (NaN: ∞×0)\n";
}

// =============================================================================
// Q37 — Testing for NaN
// =============================================================================
// NaN == NaN is always false (IEEE754 property).
// Test 1: x != x  (unique to NaN — the only value not equal to itself)
// Test 2: std::isnan(x)  (preferred — explicit and readable)

void answer_q37() {
    std::cout << "\n--- Q37: Testing for NaN ---\n";
    float nan = std::numeric_limits<float>::quiet_NaN();
    std::cout << "  nan == nan:       " << std::boolalpha << (nan == nan) << "  (always false for NaN)\n";
    std::cout << "  nan != nan:       " << (nan != nan) << "  (true — unique to NaN)\n";
    std::cout << "  std::isnan(nan):  " << std::isnan(nan) << "  (preferred)\n";
}

// =============================================================================
// Q38 — +0.0f == -0.0f
// =============================================================================
// YES: +0.0f == -0.0f is TRUE (same mathematical value).
// NO: they do NOT have the same bit pattern:
//   +0.0f: 0 00000000 00000000000000000000000
//   -0.0f: 1 00000000 00000000000000000000000  (sign bit differs)

void answer_q38() {
    std::cout << "\n--- Q38: +0.0f vs -0.0f ---\n";
    float pos_zero = 0.0f, neg_zero = -0.0f;
    uint32_t pbits, nbits;
    std::memcpy(&pbits, &pos_zero, 4);
    std::memcpy(&nbits, &neg_zero, 4);
    std::cout << "  +0.0f == -0.0f: " << std::boolalpha << (pos_zero == neg_zero) << "  (same value)\n";
    std::cout << "  +0.0f bits: " << std::bitset<32>(pbits) << "\n";
    std::cout << "  -0.0f bits: " << std::bitset<32>(nbits) << "  (sign bit differs)\n";
}

// =============================================================================
// Q39 — Machine epsilon
// =============================================================================
// ε is the smallest number that, when added to 1.0, gives a result ≠ 1.0.
// float  ε = 2^-23 ≈ 1.19209 × 10^-7
// double ε = 2^-52 ≈ 2.22045 × 10^-16

void answer_q39() {
    std::cout << "\n--- Q39: Machine Epsilon ---\n";
    std::cout << std::setprecision(20);
    std::cout << "  float  epsilon = " << std::numeric_limits<float>::epsilon()  << "\n";
    std::cout << "  double epsilon = " << std::numeric_limits<double>::epsilon() << "\n";
    std::cout << "  1.0f + eps_f != 1.0f: "
              << std::boolalpha << (1.0f + std::numeric_limits<float>::epsilon() != 1.0f) << "\n";
    std::cout << std::setprecision(6);
}

// =============================================================================
// Q40 — ULP (Units at the Last Place)
// =============================================================================
// ULP(p, e) = β^(e-(p-1)) → for IEEE754 binary: 2^(e-(p-1))
// At magnitude 1.0 (e=0, p=24 for float): ULP = 2^(0-23) = 2^-23 = ε
// As magnitude grows, ULP grows proportionally (the gap between adjacent
// representable numbers gets larger).

void answer_q40() {
    std::cout << "\n--- Q40: ULP ---\n";
    float eps = std::numeric_limits<float>::epsilon();
    std::cout << "  ULP near 1.0f:  " << (1.0f * eps) << "\n";
    std::cout << "  ULP near 1e4f:  " << (1e4f * eps)  << "\n";
    std::cout << "  ULP near 1e8f:  " << (1e8f * eps)  << "\n";
    std::cout << "  (gap grows with magnitude)\n";
}

// =============================================================================
// Q41 — Absolute and relative error bounds
// =============================================================================
// fl(x) = x(1 + δ) where |δ| ≤ ε/2
// Absolute error: |fl(x) - x| ≤ ½ × ULP_x = ½ × ε × |x| × β^e(x)
// Relative error: |fl(x) - x| / |x| ≤ ½ × ε

// =============================================================================
// Q42 — (int)16777217.0f = 16777216
// =============================================================================
// 16777217 = 2^24 + 1. Float mantissa has 23 bits → can exactly represent
// integers up to 2^24 = 16777216. Beyond that, adjacent floats are 2 apart.
// 16777217 rounds to the nearest representable float: 16777216.

void answer_q42() {
    std::cout << "\n--- Q42: (int)16777217.0f precision loss ---\n";
    float f17 = 16777217.0f;   // rounds to 16777216
    float f16 = 16777216.0f;   // exact
    std::cout << "  (float)16777217 = " << f17 << "  (rounds to 16777216!)\n";
    std::cout << "  (float)16777216 = " << f16 << "  (exact)\n";
    std::cout << "  Equal: " << std::boolalpha << (f17 == f16) << "\n";
    std::cout << "  (int)16777217.0f = " << static_cast<int>(16777217.0f) << "\n";
    std::cout << "  (int)16777217.0  = " << static_cast<int>(16777217.0)  << "  (double: correct)\n";
}

// =============================================================================
// Q43 — Floating-point arithmetic properties P1–P7
// =============================================================================
// P1: a op b ≠ a ⊙ b  — generally TRUE (rounding introduces error)
// P2: NOT reflexive   — a≠a only when a=NaN; otherwise reflexive
// P3: NOT commutative — only when NaN involved; a ⊙ b = b ⊙ a without NaN
// P4: NOT associative — even without NaN/inf; rounding order matters
// P5: NOT distributive — even without NaN/inf
// P6: Identity not ensured — (a-b)+b ≠ a due to rounding
// P7: Overflow → ±inf (saturation), NOT wraparound

void answer_q43() {
    std::cout << "\n--- Q43: FP arithmetic properties ---\n";
    float nan = std::numeric_limits<float>::quiet_NaN();
    float a = 0.1f, b = 0.2f, c = 0.3f;

    std::cout << std::boolalpha;
    std::cout << "  P2 (not reflexive with NaN): NaN==NaN → " << (nan==nan) << "\n";
    std::cout << "  P4 (not associative): (0.1+0.2)+0.3 == 0.1+(0.2+0.3) → "
              << (((a+b)+c) == (a+(b+c))) << "\n";
    std::cout << "  P6 (identity fails): (a-b)+b == a → "
              << ((a - b + b) == a) << "\n";
    float large = std::numeric_limits<float>::max();
    std::cout << "  P7 (overflow → inf): FLT_MAX*2 = " << (large*2.0f) << "\n";
}

// =============================================================================
// Q44 — IEEE 754 guarantee and determinism
// =============================================================================
// Each individual IEEE754 operation produces the CORRECTLY ROUNDED result:
// the mathematical exact result, rounded to the nearest representable float.
// Results are fully deterministic if and only if the ORDER OF OPERATIONS
// is always the same (same source code, same compiler, same settings).
// Parallel/vectorized code may reorder operations → non-deterministic results.

// =============================================================================
// Q45 — Floating-point UB
// =============================================================================
// (a) float a = 1.0f / 0.0f;    — DEFINED by IEEE754 → +inf; but note C++ may
//     treat it as UB unless FENV_ACCESS is set. In practice: usually +inf.
// (b) int b = (int)(1e38f);      — UB: 1e38 does not fit in int range.
// (c) float c = (float)(1e400);  — UB: 1e400 > FLT_MAX.
// (d) double d = 0.0/0.0;        — DEFINED by IEEE754 → NaN; same caveat as (a).

void answer_q45() {
    std::cout << "\n--- Q45: FP undefined behavior ---\n";
    std::cout << "  (a) 1.0f/0.0f: " << (1.0f/0.0f) << "  (inf, IEEE754-defined)\n";
    std::cout << "  (b) (int)(1e38f): UB — value doesn't fit in int\n";
    std::cout << "  (c) (float)(1e400): UB — value exceeds FLT_MAX\n";
    std::cout << "  (d) 0.0/0.0: " << (0.0/0.0) << "  (NaN, IEEE754-defined)\n";
}

// =============================================================================
// Q46 — <cfenv> exception macros and functions
// =============================================================================
// Macros: FE_DIVBYZERO, FE_INEXACT, FE_INVALID, FE_OVERFLOW, FE_UNDERFLOW
//         FE_ALL_EXCEPT (combines all)
// Functions:
//   std::feclearexcept(FE_ALL_EXCEPT)  — clear exception flags
//   std::fetestexcept(<macro>)          — returns non-zero if exception is set
// Pragma: #pragma STDC FENV_ACCESS ON — tells the compiler the program
//   inspects floating-point state (prevents FP-environment-breaking optimizations)
//   GCC: yes; Clang: no (not supported on most targets)

void answer_q46() {
    std::cout << "\n--- Q46: cfenv exception detection ---\n";
    std::feclearexcept(FE_ALL_EXCEPT);
    volatile float x = 1.0f / 0.0f;  (void)x;
    std::cout << "  After 1.0/0.0: FE_DIVBYZERO = "
              << std::boolalpha << (bool)std::fetestexcept(FE_DIVBYZERO) << "\n";
    std::feclearexcept(FE_ALL_EXCEPT);
    volatile float y = 0.0f / 0.0f;  (void)y;
    std::cout << "  After 0.0/0.0: FE_INVALID   = "
              << (bool)std::fetestexcept(FE_INVALID) << "\n";
}

// =============================================================================
// Q47 — Two cases of catastrophic cancellation
// =============================================================================
// C1: a ± b where a >> b (or b >> a):
//   The smaller number's precision is lost. Example: float 30,000,000 minus 1
//   — the 1 is lost because the gap between adjacent floats at that magnitude
//   is > 1. Real-world: HPE SSD 32768-hour counter overflows.
//
// C2: a - b where a ≈ b and both are approximations:
//   Most significant bits cancel out, only error-bearing low bits remain.
//   Real-world: Patriot missile — small time-tracking error accumulated over
//   100 hours, causing a 0.34-second timing error → missed an incoming Scud.
//   Quadratic formula: (-b + √(b²-4ac))/2a when b is large.

// =============================================================================
// Q48 — Non-terminating float subtraction loop
// =============================================================================
// At magnitude 30,000,000, adjacent floats are more than 2 apart (ULP > 2).
// Subtracting 1.0f produces no change — the result rounds back to 30,000,000.
// The loop never decrements x → infinite loop.

void answer_q48() {
    std::cout << "\n--- Q48: Non-terminating float subtraction ---\n";
    float x = 30000000.0f;
    float after = x - 1.0f;
    std::cout << "  30000000.0f - 1.0f = " << after << "  (same!)\n";
    std::cout << "  ULP at this magnitude > 1 → subtraction has no effect\n";
    std::cout << "  Loop would run forever → impose a hard iteration limit\n";
}

// =============================================================================
// Q49 — Quadratic formula cancellation
// =============================================================================
// BUG: when b is large, √(b²-4ac) ≈ b, so (-b + √(b²-4ac)) ≈ 0.
// Both -b and √(b²-4ac) are large, their difference has huge relative error.
//
// FIX: multiply top and bottom by the conjugate:
//   (-b + √disc) / 2a = 2c / (-b - √disc)
// This avoids computing (-b + √disc) directly.

void answer_q49() {
    std::cout << "\n--- Q49: Quadratic formula fix ---\n";
    float a=1.0f, b=5000.0f, c=0.25f;
    float disc = std::sqrt(b*b - 4.0f*a*c);
    float x_bug   = (-b + disc) / (2.0f*a);        // catastrophic
    float x_fixed = (2.0f*c) / (-b - disc);        // algebraically equivalent, numerically stable
    std::cout << std::setprecision(10);
    std::cout << "  (-b+√disc)/2a   = " << x_bug   << "  (wrong, ≈0 from cancellation)\n";
    std::cout << "  2c/(-b-√disc)   = " << x_fixed << "  (numerically stable)\n";
    std::cout << "  Correct answer:   ≈ -0.00005\n";
    std::cout << std::setprecision(6);
}

// =============================================================================
// Q50 — Problems with absolute-error comparison
// =============================================================================
// 1. ε "looks small" but may be too LARGE for very small numbers
//    (e.g., ε=1e-6 treats 1e-7 and 2e-7 as equal, but their ratio is 2×)
// 2. ε may be too SMALL for very large numbers
//    (e.g., 1e10f vs 1e10f+1 — difference is 1, but both are "equal" to float)
// 3. Not scale-invariant (ε must be manually tuned per use case)

// =============================================================================
// Q51 — Problems with relative-error comparison (÷b)
// =============================================================================
// 1. b = 0: 0.0/0.0 = NaN; (NaN < ε) = false → always returns false
// 2. b = 0, a ≠ 0: abs(a)/0.0 = ±inf; (inf < ε) = false → always false
// 3. a and b very small: division unstable, may produce wrong relative error
// 4. NOT commutative: areEqual(a,b) may differ from areEqual(b,a)
//    because we always divide by b, not by max(|a|,|b|)

// =============================================================================
// Q52 — Robust areFloatNearlyEqual implementation
// =============================================================================

bool are_float_nearly_equal_robust(float a, float b, float rel_err = 1e-5f) {
    if (!std::isfinite(a) || !std::isfinite(b)) return false;  // reject NaN/inf
    float diff     = std::abs(a - b);
    float norm_min = std::numeric_limits<float>::min();
    if (diff <= norm_min) return true;    // both near zero: absolute comparison
    float abs_a = std::abs(a), abs_b = std::abs(b);
    return (diff / std::max(abs_a, abs_b)) <= rel_err;  // relative comparison
}

void answer_q52() {
    std::cout << "\n--- Q52: Robust float comparison ---\n";
    std::cout << std::boolalpha;
    std::cout << "  0.1+0.2 ≈ 0.3?    " << are_float_nearly_equal_robust(0.1f+0.2f, 0.3f) << "\n";
    std::cout << "  1.0 ≈ 1.0+eps/2?  "
              << are_float_nearly_equal_robust(1.0f, 1.0f + std::numeric_limits<float>::epsilon()/2.0f) << "\n";
    std::cout << "  0.0 ≈ 1e-40?      " << are_float_nearly_equal_robust(0.0f, 1e-40f) << "  (near-zero)\n";
    std::cout << "  inf ≈ inf?         " << are_float_nearly_equal_robust(1.0f/0.0f, 1.0f/0.0f) << "  (inf rejected)\n";
    std::cout << "  0.0 ≈ 0.0?         " << are_float_nearly_equal_robust(0.0f, 0.0f) << "\n";
}

// =============================================================================
// Q53 — Error propagation strategies
// =============================================================================
// 1. Prefer multiplication/division over addition/subtraction
// 2. Reorganize computation to keep operands of similar magnitude (e.g., sort)
// 3. Threshold very small values to zero in iterative algorithms
// 4. Scale by powers of 2 (exact in IEEE754, no rounding)
// 5. Switch to log scale (multiply → add, divide → subtract)
// 6. Use compensation algorithms: Kahan summation, Dekker's FastTwoSum, Rump's AccSum

// =============================================================================
// Q54 — Kahan summation algorithm
// =============================================================================
// PROBLEM: Adding N small floats naively accumulates rounding errors O(N·ε).
// After many additions, the running sum is large and small addends are lost
// (catastrophic cancellation case C1: sum >> individual value).
//
// KAHAN ALGORITHM: tracks the lost low-order bits in a "compensation" variable.
//   sum = 0, c = 0
//   for each x:
//     y = x - c          // compensated value (c corrects for previous rounding)
//     t = sum + y        // rounding occurs here
//     c = (t - sum) - y  // capture what was rounded away
//     sum = t
// Error is O(ε) regardless of N.

float kahan_sum_demo(const std::vector<float>& vals) {
    float sum = 0.0f, c = 0.0f;
    for (float v : vals) {
        float y = v - c;
        float t = sum + y;
        c = (t - sum) - y;
        sum = t;
    }
    return sum;
}

void answer_q54() {
    std::cout << "\n--- Q54: Kahan summation ---\n";
    const int   N   = 20000000;
    const float val = 1.0f;
    std::vector<float> ones(N, val);

    float naive  = std::accumulate(ones.begin(), ones.end(), 0.0f);
    float kahan  = kahan_sum_demo(ones);
    float expect = static_cast<float>(N);

    std::cout << std::setprecision(2);
    std::cout << "  Adding 1.0f × " << N << ":\n";
    std::cout << "  Expected: " << expect << "\n";
    std::cout << "  Naive:    " << naive  << "  (error = " << std::abs(naive-expect) << ")\n";
    std::cout << "  Kahan:    " << kahan  << "  (error = " << std::abs(kahan-expect) << ")\n";
    std::cout << std::setprecision(6);
}

// =============================================================================
// BONUS B1 — SSD 32,768 hour firmware bug
// =============================================================================
// 32,768 = 2^15. The uptime counter is most likely a uint16_t (16-bit unsigned).
// At 2^15 hours, the MSB is set. If the comparison is done as signed int16_t,
// 2^15 (0x8000) is INT16_MIN = -32768 → the signed comparison wraps to negative.
// Or: an unsigned 16-bit counter wraps back to 0 after 2^16 = 65,536 hours;
// a signed 16-bit counter treats 2^15 as overflow → UB or unexpected state.

void answer_bonus_b1() {
    std::cout << "\n--- Bonus B1: SSD Firmware Bug ---\n";
    std::cout << "  32768 = 2^15\n";
    uint16_t counter = 32768;   // 0x8000
    int16_t  signed_view;
    std::memcpy(&signed_view, &counter, sizeof(signed_view));
    std::cout << "  uint16_t 32768 as int16_t = " << signed_view << "  (INT16_MIN!)\n";
    std::cout << "  A signed comparison wraps: counter 'appears' negative → bug\n";
}

// =============================================================================
// BONUS B2 — Boeing 787 51-day reboot
// =============================================================================
// 51 days × 24 × 60 × 60 × 1000 = 4,406,400,000 ms
// This exceeds UINT_MAX (4,294,967,295) — does NOT fit in uint32_t.
// If the counter is uint32_t, it wraps. If int32_t, INT_MAX = 2,147,483,647
// is exceeded after ~24.8 days — signed overflow = UB.

void answer_bonus_b2() {
    std::cout << "\n--- Bonus B2: Boeing 787 ---\n";
    int64_t ms = 51LL * 24 * 60 * 60 * 1000;
    std::cout << "  51 days in ms: " << ms << "\n";
    std::cout << "  UINT_MAX:      " << (uint64_t)UINT_MAX << "\n";
    std::cout << "  INT_MAX:       " << INT_MAX << "\n";
    std::cout << "  51-day ms > UINT_MAX: " << std::boolalpha << (ms > (int64_t)UINT_MAX) << "\n";
    std::cout << "  Overflows uint32_t! Counter wraps → misleading data shown to pilots.\n";
}

// =============================================================================
// BONUS B3 — Ceiling division without float
// =============================================================================

int ceiling_div(int a, int b) {
    // For positive a and b: (a + b - 1) / b
    return (a + b - 1) / b;
}

void answer_bonus_b3() {
    std::cout << "\n--- Bonus B3: Ceiling division ---\n";
    std::cout << "  ceil(10/3)  = " << ceiling_div(10, 3) << "  (expected 4)\n";
    std::cout << "  ceil(9/3)   = " << ceiling_div(9, 3)  << "  (expected 3)\n";
    std::cout << "  ceil(7/2)   = " << ceiling_div(7, 2)  << "  (expected 4)\n";
    std::cout << "  ceil(101/2) = " << ceiling_div(101,2) << "  (expected 51)\n";
}

// =============================================================================
// BONUS B4 — Ariane 5 failure
// =============================================================================
// Category: FLOATING-POINT TO INTEGER CONVERSION of a value out of range.
// A 64-bit double representing horizontal velocity was converted to a 16-bit
// signed integer. The velocity value exceeded INT16_MAX = 32767 → conversion
// UB (or hardware exception) → unhandled exception → backup computer took over
// → same software, same exception → rocket self-destructed 37 seconds after launch.

// =============================================================================
// MAIN
// =============================================================================

int main() {
    std::cout << "=== Unit 4: Practice Question Answers ===\n";

    answer_q1();
    answer_q6();
    answer_q7();
    answer_q8();
    answer_q9();
    answer_q10();
    answer_q12();
    answer_q13();
    answer_q14();
    answer_q17();
    answer_q18();
    answer_q19();
    answer_q20();
    answer_q22();
    answer_q23();
    answer_q25();
    answer_q26();
    answer_q27();
    answer_q29();
    answer_q30();
    answer_q33();
    answer_q34();
    answer_q36();
    answer_q37();
    answer_q38();
    answer_q39();
    answer_q40();
    answer_q42();
    answer_q43();
    answer_q45();
    answer_q46();
    answer_q48();
    answer_q49();
    answer_q52();
    answer_q54();
    answer_bonus_b1();
    answer_bonus_b2();
    answer_bonus_b3();

    std::cout << "\n=== Done ===\n";
    return 0;
}
