/**
 * Unit 4: Basic Concepts II
 * Integer Types, Overflow Semantics, and Floating-Point Arithmetic
 *
 * Compile: g++ -std=c++17 -Wall -Wextra -Wpedantic -o concepts concepts.cpp
 * Run:     ./concepts
 *
 * Notes on standards used:
 *   C++11 — fixed-width integers, numeric_limits, cmath functions
 *   C++17 — used throughout; standard target for this unit
 *   C++20 — bitwise ops on negative signed types become well-defined (noted in comments)
 *   C++26 — saturation arithmetic (noted in comments, not compiled)
 */

#include <algorithm>   // std::max
#include <bitset>      // std::bitset (for float bit display)
#include <cfenv>       // FE_DIVBYZERO, std::feclearexcept, etc.
#include <climits>     // INT_MAX, INT_MIN, UINT_MAX
#include <cmath>       // std::isinf, std::isnan, std::sqrt, std::abs, etc.
#include <cstddef>     // size_t, ptrdiff_t
#include <cstdint>     // int8_t … uint64_t, uintptr_t
#include <cstring>     // std::memcpy  (safe type-punning)
#include <iomanip>     // std::setprecision
#include <iostream>
#include <limits>      // std::numeric_limits
#include <numeric>     // std::accumulate
#include <vector>      // std::vector (Kahan summation demo)

// =============================================================================
// SECTION 1: C++ Data Models
// =============================================================================
//
// The C++ standard does NOT mandate sizes for most built-in types; only:
//   sizeof(char) == 1  always
//   short  ≤  int  ≤  long  ≤  long long
//
// In practice three data models are used on modern platforms:
//
//  Model  | OS                  | short | int | long | long long | pointer
//  -------|---------------------|-------|-----|------|-----------|--------
//  ILP32  | Windows/Unix 32-bit |  16   | 32  |  32  |    64     |   32
//  LLP64  | Windows 64-bit      |  16   | 32  |  32  |    64     |   64
//  LP64   | Linux 64-bit        |  16   | 32  |  64  |    64     |   64
//
// Key differences:
//   LLP64 vs LP64:  `long` is 32-bit on Windows, 64-bit on Linux
//   This is why size-sensitive code should use fixed-width types.
//
// char is ALWAYS 1 byte.
// LP32 (Windows 16-bit APIs) is historical and no longer used.

void section_data_models() {
    std::cout << "\n=== Section 1: C++ Data Models ===\n";
    std::cout << "Current platform type sizes:\n";
    std::cout << "  char:        " << sizeof(char)        * 8 << " bits\n";
    std::cout << "  short:       " << sizeof(short)       * 8 << " bits\n";
    std::cout << "  int:         " << sizeof(int)         * 8 << " bits\n";
    std::cout << "  long:        " << sizeof(long)        * 8 << " bits\n";
    std::cout << "  long long:   " << sizeof(long long)   * 8 << " bits\n";
    std::cout << "  pointer:     " << sizeof(void*)       * 8 << " bits\n";

    // Detect the model
    if (sizeof(long) == 4 && sizeof(void*) == 4)
        std::cout << "  → Data model: ILP32\n";
    else if (sizeof(long) == 4 && sizeof(void*) == 8)
        std::cout << "  → Data model: LLP64 (Windows 64-bit)\n";
    else if (sizeof(long) == 8 && sizeof(void*) == 8)
        std::cout << "  → Data model: LP64 (Linux/macOS 64-bit)\n";
}

// =============================================================================
// SECTION 2: Fixed-Width Integer Types (<cstdint>)
// =============================================================================
//
// C++11 provides fixed-width integer types via <cstdint>.
// They have the SAME SIZE on any architecture — vital for portable code.
//
//   int8_t    uint8_t
//   int16_t   uint16_t
//   int32_t   uint32_t
//   int64_t   uint64_t
//
// Good practice: prefer fixed-width types over native types.
// Exception: int and unsigned are widely accepted because C++ data models
// agree that int is 32 bits on all modern (32/64-bit) platforms.
//
// IMPORTANT: int*_t are NOT "real" types — they are TYPEDEFS to fundamental types.
// The standard requires five fundamental integral types (char, short, int, long,
// long long) but only four int*_t widths (8, 16, 32, 64 bits).  This means there
// is no guaranteed one-to-one mapping; e.g., int32_t may be `int` OR `long`.
// This matters for overload resolution and template specialization.
//
// WARNING — I/O Stream and int8_t/uint8_t:
// Because int8_t is a typedef for signed char and uint8_t for unsigned char,
// std::cout and std::cin treat them as CHARACTERS, not integers:
//
//   int8_t var;
//   std::cin >> var;      // reads one CHARACTER ('2'), not the number 2
//   std::cout << var;     // prints the character '2' (not 50)
//   std::cout << var + 0; // promotes to int → prints 50 (ASCII of '2')

void section_fixed_width_integers() {
    std::cout << "\n=== Section 2: Fixed-Width Integer Types ===\n";

    std::cout << "Sizes (bytes):\n";
    std::cout << "  int8_t  / uint8_t:  " << sizeof(int8_t)  << "\n";
    std::cout << "  int16_t / uint16_t: " << sizeof(int16_t) << "\n";
    std::cout << "  int32_t / uint32_t: " << sizeof(int32_t) << "\n";
    std::cout << "  int64_t / uint64_t: " << sizeof(int64_t) << "\n";

    // Demonstrate the I/O stream char-interpretation gotcha
    std::cout << "\n--- int8_t / uint8_t I/O Gotcha ---\n";
    int8_t  var = 50;   // 50 is the ASCII code of '2'
    std::cout << "int8_t var = 50\n";
    std::cout << "  cout << var       → '" << var          << "'  (treated as char!)\n";
    std::cout << "  cout << var + 0   → "  << (var + 0)   << "   (promoted to int)\n";
    std::cout << "  cout << +var      → "  << (+var)       << "   (unary + promotes to int)\n";
    std::cout << "  static_cast<int>  → "  << static_cast<int>(var) << "\n";

    // ==========================================================================
    // SECTION 2b: size_t (<cstddef>)
    // ==========================================================================
    // size_t is an ALIAS type for the largest representable value on the architecture.
    //   - unsigned integer (at least 16 bits)
    //   - Return type of sizeof()
    //   - Used to represent sizes and counts (container sizes, array indices)
    //   - 4 bytes on 32-bit, 8 bytes on 64-bit
    //   - C++23 adds `uz`/`UZ` literals: e.g., 5uz

    std::cout << "\n--- size_t ---\n";
    size_t sz = sizeof(int);
    std::cout << "sizeof(size_t) = " << sizeof(size_t) << " bytes\n";
    std::cout << "sizeof(int) stored in size_t: " << sz << "\n";
    std::cout << "Max size_t: " << std::numeric_limits<size_t>::max() << "\n";

    // ==========================================================================
    // SECTION 2c: ptrdiff_t (<cstddef>)
    // ==========================================================================
    // ptrdiff_t is the SIGNED version of size_t.
    //   - Used to store the result of pointer or iterator differences
    //   - 4 bytes on 32-bit, 8 bytes on 64-bit
    //   - C++23 adds `z`/`Z` literals: e.g., 5z

    std::cout << "\n--- ptrdiff_t ---\n";
    int arr[5] = {10, 20, 30, 40, 50};
    ptrdiff_t diff = &arr[4] - &arr[0];   // difference between two pointers
    std::cout << "sizeof(ptrdiff_t) = " << sizeof(ptrdiff_t) << " bytes\n";
    std::cout << "&arr[4] - &arr[0] = " << diff << " (signed distance)\n";

    // ==========================================================================
    // SECTION 2d: uintptr_t (<cstdint>)
    // ==========================================================================
    // uintptr_t is an integer type that can hold a void* pointer value.
    //   - Unsigned type
    //   - sizeof(uintptr_t) == sizeof(void*)
    //   - OPTIONAL in the standard — compilers may not provide it
    //   - Useful for pointer arithmetic expressed as integers

    std::cout << "\n--- uintptr_t ---\n";
    int value = 42;
    uintptr_t addr = reinterpret_cast<uintptr_t>(&value);
    std::cout << "sizeof(uintptr_t) = " << sizeof(uintptr_t) << " bytes\n";
    std::cout << "sizeof(void*)     = " << sizeof(void*)     << " bytes\n";
    std::cout << "Address of value as integer: 0x" << std::hex << addr << std::dec << "\n";
}

// =============================================================================
// SECTION 3: Arithmetic Operation Semantics
// =============================================================================
//
// When an arithmetic result exceeds the word size, three behaviors are defined:
//
// OVERFLOW:    The result exceeds the largest/smallest representable value.
//              In C++, signed integer overflow is UNDEFINED BEHAVIOR.
//
// WRAPAROUND:  Result is reduced modulo 2^N (N = number of bits in the type).
//              In C++, unsigned integers ALWAYS use wraparound — well-defined.
//
// SATURATION:  Result is clamped to the min or max value.
//              Not the default in C++, but C++26 adds std::add_sat() etc.
//
// Signed vs. Unsigned use the same hardware (two's complement), but have
// DIFFERENT SEMANTICS in the C++ standard.  The compiler may exploit signed UB
// for optimization even if the hardware would produce a well-defined result.

void section_overflow_semantics() {
    std::cout << "\n=== Section 3: Arithmetic Operation Semantics ===\n";

    // --- Unsigned wraparound (well-defined) ---
    unsigned int umax = std::numeric_limits<unsigned int>::max();  // 2^32 - 1
    unsigned int wrapped = umax + 1u;  // wraps to 0 (modulo 2^32)
    std::cout << "WRAPAROUND (unsigned):\n";
    std::cout << "  UINT_MAX + 1 = " << wrapped << "  (wraps to 0)\n";
    std::cout << "  0u - 1u      = " << (0u - 1u) << "  (wraps to UINT_MAX)\n";

    // Both signed and unsigned use the same binary representation (two's complement):
    int          a1 = INT_MAX;
    unsigned int a2 = static_cast<unsigned int>(INT_MAX);
    // a1 + 4 is UB for signed; a2 + 4 is well-defined for unsigned:
    unsigned int b2 = a2 + 4u;
    std::cout << "\nSame hardware, different semantics:\n";
    std::cout << "  int    INT_MAX = " << a1 << "\n";
    std::cout << "  uint   INT_MAX = " << a2 << "\n";
    std::cout << "  uint   INT_MAX + 4 (defined) = " << b2 << "\n";
    // int INT_MAX + 4 → UNDEFINED BEHAVIOR (see Section 4)

    // Saturation (C++26 preview — standard API, shown here conceptually):
    // std::add_sat(T x, T y)  — result clamped to [min, max] instead of wrapping
    // std::sub_sat(T x, T y)
    // std::mul_sat(T x, T y)
    // std::div_sat(T x, T y)
    // std::saturate_cast<R>(T x)
    std::cout << "\nSATURATION (C++26 std::add_sat — shown conceptually):\n";
    // Simulated saturation for unsigned add:
    auto sat_add = [](unsigned x, unsigned y) -> unsigned {
        unsigned result = x + y;
        return (result < x) ? std::numeric_limits<unsigned>::max() : result;
    };
    std::cout << "  sat_add(UINT_MAX, 5) = " << sat_add(UINT_MAX, 5u) << "  (clamped)\n";
    std::cout << "  sat_add(10, 5)       = " << sat_add(10u, 5u)      << "\n";
}

// =============================================================================
// SECTION 4: Signed Integer Characteristics and Undefined Behavior
// =============================================================================
//
// SIGNED INTEGER PROPERTIES:
//   + Represents positive, negative, and zero (Z)
//   + Commutative:  a + b = b + a
//   + Reflexive:    a == a
//   - NOT Associative (overflow/underflow)
//   + Human intuition of numbers
//   + All bitwise operations are well-defined EXCEPT shift  (C++17)
//   (C++20: bitwise ops on negatives are now fully defined, two's complement guaranteed)
//
// SIGNED INTEGER PROBLEMS:
//   ⚠ More negative values than positive: 2^31 negatives vs 2^31-1 positives
//       → INT_MIN * -1 is UNDEFINED BEHAVIOR (result 2^31 doesn't fit)
//       → INT_MAX * -1 is fine (-2^31 + 1, fits)
//   ⚠ Overflow/underflow → UNDEFINED BEHAVIOR
//       Compiler may assume it NEVER happens and optimize accordingly
//   ⚠ Shift UB:
//       x << y  is UB if y >= #bits of x
//       x << y  is UB if x is negative (UNTIL C++20; well-defined in C++20)
//       x << y  is UB if y is negative (in all standards)
//
// REAL-WORLD CONSEQUENCES of signed overflow UB:
//   - Compiler sees `pos++ < 0` as always false (signed never overflows → no check)
//   - Linux kernel overflow check eliminated by optimizer
//   - PostgreSQL division-by-zero guard bypassed
//   - Simple loop `for(int i=0; i<4; ++i) cout << i*1000000000;` becomes infinite
//     loop under -O3 because 1000000000*i > INT_MAX is assumed never true

void section_signed_integer() {
    std::cout << "\n=== Section 4: Signed Integer Behavior ===\n";

    std::cout << "INT_MAX = " << INT_MAX << "  (2^31 - 1 = " << (1LL<<31)-1 << ")\n";
    std::cout << "INT_MIN = " << INT_MIN << "  (-2^31 = " << -(1LL<<31) << ")\n";

    // INT_MAX * -1 is FINE (result = -2147483647, fits in int)
    int safe_neg = INT_MAX * -1;
    std::cout << "INT_MAX * -1 = " << safe_neg << "  (OK, fits)\n";

    // INT_MIN * -1 → RESULT IS 2^31, which does NOT fit in int → UNDEFINED BEHAVIOR
    // DO NOT actually execute — shown here in a comment only:
    // int ub = INT_MIN * -1;  // UB! optimizer may do anything

    // Initializing with out-of-range value is also UB:
    // int z = 3000000000;  // UB: 3e9 > INT_MAX

    // Shift UB examples (shown as comments — DO NOT compile/run):
    // int y = -1 << 12;        // UB until C++20 (negative left shift)
    // int z2 = 1 << -12;       // UB always (negative shift amount)
    // unsigned w = 1u << 32u;  // UB: shift >= bit width of type

    std::cout << "\nSafe shift examples (all well-defined):\n";
    int pos = 1;
    std::cout << "  1 << 4  = " << (pos << 4)  << "  (16)\n";
    std::cout << "  1 << 30 = " << (pos << 30) << "  (2^30 = 1073741824)\n";
    // 1 << 31 would set the sign bit — implementation-defined in C++17
    unsigned u = 1u;
    std::cout << "  1u << 31 (unsigned) = " << (u << 31) << "  (2^31, well-defined)\n";
}

// =============================================================================
// SECTION 5: Unsigned Integer Characteristics
// =============================================================================
//
// UNSIGNED INTEGER PROPERTIES:
//   + Represents only non-negative values (N)
//   + Commutative:  a + b = b + a
//   + Reflexive:    a == a
//   + ASSOCIATIVE:  (a + b) + c == a + (b + c)  (modulo 2^N)
//   + Discontinuity at 0 and 2^32-1
//   + Wraparound is WELL-DEFINED (modulo 2^N)
//   + All bitwise operations are well-defined EXCEPT shift (same UB as signed for overflow)
//   ⚠ Shift UB: x << y is UB if y >= #bits of x

void section_unsigned_integer() {
    std::cout << "\n=== Section 5: Unsigned Integer Behavior ===\n";

    unsigned int a = 10, b = 20, c = 30;

    // Wraparound is well-defined for unsigned:
    std::cout << "Wraparound (modulo 2^32):\n";
    std::cout << "  0u - 1u              = " << (0u - 1u) << "  (= UINT_MAX)\n";
    std::cout << "  UINT_MAX + 1u        = " << (UINT_MAX + 1u) << "  (wraps to 0)\n";

    // Associativity holds (modulo 2^N):
    unsigned sum1 = (a + b) + c;
    unsigned sum2 = a + (b + c);
    std::cout << "\nAssociativity: (a+b)+c == a+(b+c): " << std::boolalpha << (sum1 == sum2) << "\n";

    // Common pitfall: size() - 1 when size() == 0
    std::vector<int> empty_vec;
    size_t sz = empty_vec.size();          // 0
    size_t dangerous = sz - 1;            // wraps to SIZE_MAX! well-defined but WRONG
    std::cout << "\nPitfall: empty vector size() - 1:\n";
    std::cout << "  size() = " << sz << "\n";
    std::cout << "  size() - 1 = " << dangerous << "  (SIZE_MAX — wraps!)\n";
    std::cout << "  Safe check: use if (!vec.empty()) before size()-1\n";
}

// =============================================================================
// SECTION 6: When to Use Signed vs. Unsigned / Numeric Limits
// =============================================================================
//
// GOOGLE STYLE GUIDE NOTE: "Because of historical accident, the C++ standard
// also uses unsigned integers to represent the size of containers — many
// members of the standards body believe this to be a mistake."
//
// RECOMMENDATION: use int64_t for most counters and sizes.
//   max value: 2^63 - 1 = 9,223,372,036,854,775,807
//   = 9 quintillion, ≈ 292 years in nanoseconds, ≈ 9 million terabytes
//
// USE SIGNED when:
//   - Values may be mixed with negatives (e.g., subtracting byte sizes)
//   - Prefer expressing non-negative values with signed + assertion
//   - Optimization: exploit UB for overflow in loops
//
// USE UNSIGNED when:
//   - Values can NEVER be negative (e.g., bitmask flags)
//   - Modulo/division optimization purposes
//   - Safety-critical: signed overflow is "non-deterministic" (UB)
//
// (Bjarne Stroustrup: "Subscripts and sizes should be signed")
//
// ARITHMETIC TYPE LIMITS — query with std::numeric_limits<T> (<limits>):

void section_numeric_limits() {
    std::cout << "\n=== Section 6: Numeric Limits ===\n";

    // Integer limits
    std::cout << "Integer limits:\n";
    std::cout << "  numeric_limits<int>::max()    = " << std::numeric_limits<int>::max()          << "\n";
    std::cout << "  numeric_limits<int>::min()    = " << std::numeric_limits<int>::min()          << "\n";
    std::cout << "  numeric_limits<uint16_t>::max()= " << std::numeric_limits<uint16_t>::max()   << "\n";
    std::cout << "  numeric_limits<unsigned>::min()= " << std::numeric_limits<unsigned>::min()    << "\n";
    std::cout << "  numeric_limits<int64_t>::max() = " << std::numeric_limits<int64_t>::max()    << "\n";

    // Floating-point limits
    std::cout << "\nFloating-point limits:\n";
    std::cout << "  numeric_limits<float>::max()      = " << std::numeric_limits<float>::max()      << "\n";
    std::cout << "  numeric_limits<float>::lowest()   = " << std::numeric_limits<float>::lowest()   << "  (most negative, C++11)\n";
    std::cout << "  numeric_limits<float>::min()      = " << std::numeric_limits<float>::min()      << "  (smallest POSITIVE normal)\n";
    std::cout << "  numeric_limits<float>::denorm_min()=" << std::numeric_limits<float>::denorm_min() << "  (smallest denormal)\n";
    std::cout << "  numeric_limits<float>::epsilon()  = " << std::numeric_limits<float>::epsilon()  << "\n";
    std::cout << "  numeric_limits<float>::infinity() = " << std::numeric_limits<float>::infinity() << "\n";
    std::cout << "  numeric_limits<float>::quiet_NaN()= " << std::numeric_limits<float>::quiet_NaN() << "\n";

    std::cout << "\nDouble limits:\n";
    std::cout << "  numeric_limits<double>::max()     = " << std::numeric_limits<double>::max()     << "\n";
    std::cout << "  numeric_limits<double>::epsilon() = " << std::numeric_limits<double>::epsilon() << "\n";
}

// =============================================================================
// SECTION 7: Promotion and Truncation
// =============================================================================
//
// PROMOTION (smaller type → larger type): preserves the value, SIGN-EXTENDS.
//   int16_t x = -1;
//   int     y = x;   // sign extends: all upper bits become 1 → -1
//
// TRUNCATION (larger type → smaller type): MODULO operation.
//   The value is reduced modulo 2^N where N is the number of bits in the target.
//   int      x = 65537;   // 2^16 + 1
//   int16_t  y = x;       // x % 2^16 = 1  → prints 1
//
//   int      z = 32769;   // 2^15 + 1 (does not fit in int16_t)
//   int16_t  w = z;       // (int16_t)(z % 2^16) = 32769 → -32767 (sign bit flips)

void section_promotion_truncation() {
    std::cout << "\n=== Section 7: Promotion and Truncation ===\n";

    // Promotion: sign extension
    int16_t x_neg = -1;
    int     y_promoted = x_neg;   // sign-extends
    std::cout << "Promotion (sign-extend):\n";
    std::cout << "  int16_t(-1) → int: " << y_promoted << "\n";

    int16_t x_pos = 100;
    int     y_pos = x_pos;
    std::cout << "  int16_t(100) → int: " << y_pos << "\n";

    // Truncation: modulo 2^N
    std::cout << "\nTruncation (modulo 2^N):\n";
    int x1 = 65537;        // 2^16 + 1
    auto y1 = static_cast<int16_t>(x1);   // 65537 % 65536 = 1
    std::cout << "  int(65537) → int16_t: " << y1 << "  (65537 % 2^16 = 1)\n";

    int x2 = 32769;        // 2^15 + 1 — doesn't fit in int16_t
    auto y2 = static_cast<int16_t>(x2);   // wraps: 32769 - 65536 = -32767
    std::cout << "  int(32769) → int16_t: " << y2 << "  (sign bit flipped → -32767)\n";

    int x3 = -1;
    auto y3 = static_cast<uint16_t>(x3);  // -1 → 65535
    std::cout << "  int(-1) → uint16_t: " << y3 << "  (wraps to 65535)\n";
}

// =============================================================================
// SECTION 8: Mixing Signed/Unsigned Errors
// =============================================================================
//
// Common real-world bugs caused by implicit sign/unsigned conversion.
// These produce WRONG LOGICAL RESULTS (not UB — unsigned arithmetic is defined).
//
// Bug 1: array index from unsigned × negative-int multiplication
//   unsigned a = 10; int b = -1;
//   b is converted to unsigned: -1 → UINT_MAX
//   a * UINT_MAX wraps → huge index → SEGFAULT
//
// Bug 2: comparison of signed < 0 value with unsigned
//   int a = -1; unsigned b = 10;
//   (a > b): -1 converted to unsigned → UINT_MAX; UINT_MAX > 10 → TRUE (wrong!)
//
// Bug 3: size_t loop going to SIZE_MAX when size() == 0
//   for (size_t i = 0; i < v.size() - 1; i++)  // v.size()-1 wraps when empty!

void section_mixed_sign_errors() {
    std::cout << "\n=== Section 8: Mixing Signed/Unsigned Errors ===\n";

    // --- Bug 1: array index ---
    unsigned a = 10;
    int      b = -1;
    // b converted to unsigned → UINT_MAX; a * UINT_MAX wraps; add 10ull → huge
    unsigned  product      = a * static_cast<unsigned>(b);  // unsigned wraparound
    uint64_t  bad_index    = 10ull + static_cast<uint64_t>(product);
    std::cout << "Bug 1: unsigned(10) * int(-1):\n";
    std::cout << "  -1 as unsigned = " << static_cast<unsigned>(-1) << "\n";
    std::cout << "  10u * UINT_MAX wraps to = " << product << "\n";
    std::cout << "  10ull + product = " << bad_index << "  (massive array index!)\n";

    // --- Bug 2: signed/unsigned comparison ---
    int      sa = -1;
    unsigned ub = 10;
    // -1 is converted to unsigned before comparison
    bool naive_result = (static_cast<unsigned>(sa) > ub);  // -Wsign-compare catches this
    std::cout << "\nBug 2: int(-1) > unsigned(10):\n";
    std::cout << "  -1 converted to unsigned = " << static_cast<unsigned>(sa) << "\n";
    std::cout << "  (unsigned)(-1) > 10u → " << std::boolalpha << naive_result << "  (TRUE — wrong!)\n";
    std::cout << "  Use std::cmp_greater(-1, 10) for correct comparison\n";

    // --- Bug 3: size_t underflow in loop ---
    std::vector<int> empty;
    size_t sz = empty.size();   // 0
    std::cout << "\nBug 3: size_t loop with empty vector:\n";
    std::cout << "  v.size()     = " << sz << "\n";
    std::cout << "  v.size() - 1 = " << (sz - 1) << "  (SIZE_MAX — wraps to huge value!)\n";
    std::cout << "  Fix: use (size_t i = 0; i + 1 < v.size(); i++) or check empty()\n";
}

// =============================================================================
// SECTION 9: Overflow / Underflow Detection
// =============================================================================
//
// Detecting overflow for UNSIGNED types (after the fact — wraparound is well-defined):
//
// Detecting overflow/underflow for SIGNED types MUST be done BEFORE the operation.
// Performing the operation first and then checking is undefined behavior.

bool is_add_overflow_u(unsigned a, unsigned b) {
    // After wraparound, a+b < a (or < b) if overflow occurred
    return (a + b) < a || (a + b) < b;
}

bool is_mul_overflow_u(unsigned a, unsigned b) {
    unsigned x = a * b;
    return a != 0 && (x / a) != b;
}

// Signed: MUST check BEFORE the operation
bool signed_add_would_overflow(int a, int b) {
    if (b > 0 && a > (INT_MAX - b)) return true;
    if (b < 0 && a < (INT_MIN - b)) return true;
    return false;
}

void section_overflow_detection() {
    std::cout << "\n=== Section 9: Overflow / Underflow Detection ===\n";

    std::cout << "Unsigned overflow detection (after the fact):\n";
    std::cout << "  is_add_overflow(UINT_MAX, 1) = " << std::boolalpha << is_add_overflow_u(UINT_MAX, 1u) << "\n";
    std::cout << "  is_add_overflow(10, 20)      = " << is_add_overflow_u(10u, 20u) << "\n";
    std::cout << "  is_mul_overflow(UINT_MAX, 2) = " << is_mul_overflow_u(UINT_MAX, 2u) << "\n";
    std::cout << "  is_mul_overflow(10, 20)      = " << is_mul_overflow_u(10u, 20u) << "\n";

    std::cout << "\nSigned overflow detection (before the operation):\n";
    std::cout << "  signed_add_would_overflow(INT_MAX, 1)  = " << signed_add_would_overflow(INT_MAX, 1) << "\n";
    std::cout << "  signed_add_would_overflow(INT_MIN, -1) = " << signed_add_would_overflow(INT_MIN, -1) << "\n";
    std::cout << "  signed_add_would_overflow(10, 20)      = " << signed_add_would_overflow(10, 20) << "\n";
}

// =============================================================================
// SECTION 10: IEEE 754 Floating-Point Representation
// =============================================================================
//
// IEEE754 (1985, revised 2008, 2019) defines binary floating-point formats:
//
//  Format  | Standard C++ | Bits | Sign | Exponent | Mantissa
//  --------|--------------|------|------|----------|----------
//  float   | yes (32-bit) |  32  |   1  |    8     |    23
//  double  | yes (64-bit) |  64  |   1  |   11     |    52
//  float128| C++23        | 128  |   1  |   15     |   112
//  float16 | C++23        |  16  |   1  |    5     |    10     (GPU, Arm7)
//  bfloat16| C++23        |  16  |   1  |    8     |     7     (TPU, GPU, Arm8)
//  E4M3    | (non-std)    |   8  |   1  |    4     |     3     (ML/AI)
//  E5M2    | (non-std)    |   8  |   1  |    5     |     2     (ML/AI)
//
// Value formula: n = 1.M × 2^e
//   where e = stored_exponent - bias
//   bias = 127 for float, 1023 for double
//
// EXPONENT BIAS: exponent stored as unsigned for easy comparison;
//   actual exponent = stored_exponent - bias
//   Float: stored range [1,254], actual range [-126, +127]
//          (0 = zero/denormal, 255 = inf/NaN)
//
// Example (from slides): 448.0f
//   Sign: 0 (+)
//   Stored exponent: 10000111b = 135; actual = 135 - 127 = 8 → 2^8 = 256
//   Mantissa: 11000000...0 → 1.11b = 1.75
//   Value: 1.75 × 256 = 448.0 ✓

void section_ieee754_representation() {
    std::cout << "\n=== Section 10: IEEE 754 Floating-Point Representation ===\n";

    // Examine the bit layout of 448.0f using memcpy (safe type-punning)
    float    fval = 448.0f;
    uint32_t fbits;
    std::memcpy(&fbits, &fval, sizeof(fbits));

    uint32_t sign     = (fbits >> 31) & 0x1;
    uint32_t exponent = (fbits >> 23) & 0xFF;
    uint32_t mantissa =  fbits        & 0x7FFFFF;
    int      actual_e = static_cast<int>(exponent) - 127;

    std::cout << "float value: " << fval << "\n";
    std::cout << "Bits (32):   " << std::bitset<32>(fbits) << "\n";
    std::cout << "  Sign:      " << sign << " (" << (sign ? "-" : "+") << ")\n";
    std::cout << "  Exponent:  " << std::bitset<8>(exponent) << " = " << exponent
              << " (stored), actual = " << actual_e << " → 2^" << actual_e
              << " = " << (1 << actual_e) << "\n";
    std::cout << "  Mantissa:  " << std::bitset<23>(mantissa) << "\n";
    std::cout << "  = 1.11b (binary) = 1.75 × " << (1 << actual_e) << " = " << fval << "\n";

    // A few more representative float literals
    std::cout << "\nFloat literal syntax:\n";
    float f1 = 1.3f;        // 1.3
    float f2 = 1.1e2f;      // 1.1 × 10^2 = 110
    float f3 = 3.7E4f;      // 3.7 × 10^4 = 37000
    float f4 = .3f;         // 0.3
    double d1 = 1.3;        // 1.3 double
    double d2 = 5E3;        // 5 × 10^3 = 5000
    std::cout << "  float 1.3f   = " << f1  << "\n";
    std::cout << "  float 1.1e2f = " << f2  << "\n";
    std::cout << "  float 3.7E4f = " << f3  << "\n";
    std::cout << "  float .3f    = " << f4  << "\n";
    std::cout << "  double 1.3   = " << d1  << "\n";
    std::cout << "  double 5E3   = " << d2  << "\n";

    // ==========================================================================
    // SECTION 10b: Normal and Denormal Numbers
    // ==========================================================================
    // NORMAL number: at least one bit set in the exponent (exponent != 0).
    //   Represents most everyday floating-point values.
    //
    // DENORMAL (subnormal) number: all 0s in the exponent, non-zero mantissa.
    //   Fills the "underflow gap" around zero.
    //   Smallest normal float: ±1.17549 × 10^-38
    //   Smallest denormal:     ±1.4 × 10^-45
    //   Denormals allow gradual underflow rather than abrupt flush-to-zero.

    std::cout << "\n--- Normal / Denormal Numbers ---\n";
    float normal_min  = std::numeric_limits<float>::min();       // smallest normal
    float denorm_min  = std::numeric_limits<float>::denorm_min(); // smallest denormal
    std::cout << "Smallest normal float:   " << normal_min  << "\n";
    std::cout << "Smallest denormal float: " << denorm_min  << "\n";
    std::cout << "Is normal_min normal?    " << std::boolalpha << std::isnormal(normal_min) << "\n";
    std::cout << "Is denorm_min normal?    " << std::isnormal(denorm_min) << "\n";
}

// =============================================================================
// SECTION 11: Floating-Point Special Values — Infinity and NaN
// =============================================================================
//
// IEEE754 reserves exponent=all-1s for special values:
//   Mantissa all-0s → ±infinity
//   Mantissa non-0  → NaN (Not a Number)
//
// INFINITY:
//   Generated by: ±∞ × ±∞,  ±∞ × finite,  finite / ±0,  finite > max_value
//   Comparison:   inf == finite → false;  ±inf == ±inf → true
//
// NaN:
//   Generated by: any op with NaN operand, ±∞ × ∓∞, 0/0, ∞/∞,
//                 √negative, log(x) for x < 0
//   Comparison:   NaN == x → FALSE for every x (including NaN itself!)
//                 This is the ONLY way to test for NaN: x != x
//                 (or std::isnan(x))

void section_special_values() {
    std::cout << "\n=== Section 11: Special Values — Infinity and NaN ===\n";

    // ---- Infinity ----
    float pos_inf = std::numeric_limits<float>::infinity();
    float neg_inf = -pos_inf;

    std::cout << "--- Infinity ---\n";
    std::cout << "5.0f / 0.0f  = " << (5.0f / 0.0f)    << "\n";  // +inf
    std::cout << "-5.0f / 0.0f = " << (-5.0f / 0.0f)   << "\n";  // -inf
    std::cout << "inf + 1000   = " << (pos_inf + 1000.0f) << "\n";  // inf
    std::cout << "inf * 0      = " << (pos_inf * 0.0f)    << "\n";  // NaN
    std::cout << "inf - inf    = " << (pos_inf - pos_inf) << "\n";  // NaN
    std::cout << "inf + inf    = " << (pos_inf + pos_inf) << "\n";  // inf
    std::cout << "-0.0 == 0.0  = " << std::boolalpha << (-0.0f == 0.0f) << "\n"; // true (same value)
    std::cout << "inf == inf   = " << (pos_inf == pos_inf) << "\n";            // true
    std::cout << "inf == 1e38f = " << (pos_inf == 1e38f)   << "\n";            // false

    // Float-only: float can produce inf where double doesn't (range difference)
    // Compute inf via overflow at runtime (avoids compile-time overflow warning)
    float big_float = std::numeric_limits<float>::max() * 10.0f;  // overflows → inf
    std::cout << "FLT_MAX*10 == FLT_MAX*10 + 9999999.0f = "
              << (big_float == (big_float + 9999999.0f)) << "\n";  // true (both inf)
    std::cout << "FLT_MAX*10 == inf: "
              << (big_float == std::numeric_limits<float>::infinity()) << "\n";

    // ---- NaN ----
    float nan_val = std::numeric_limits<float>::quiet_NaN();
    std::cout << "\n--- NaN ---\n";
    std::cout << "0.0f / 0.0f  = " << (0.0f / 0.0f)   << "\n";   // nan
    std::cout << "NaN == NaN   = " << (nan_val == nan_val) << "\n"; // false (!)
    std::cout << "NaN != NaN   = " << (nan_val != nan_val) << "\n"; // true → test for NaN
    std::cout << "NaN + 5      = " << (nan_val + 5.0f)     << "\n"; // nan
    std::cout << "NaN < 5      = " << (nan_val < 5.0f)     << "\n"; // false
    std::cout << "std::isnan(NaN) = " << std::isnan(nan_val) << "\n"; // true (preferred test)

    // Special values table:
    std::cout << "\n--- Special Values Behavior ---\n";
    std::cout << "Zero:   a/0  (a≠0) = inf      [IEEE754; UB in C++ for int]\n";
    std::cout << "        0/0         = NaN      [IEEE754; UB in C++ for int]\n";
    std::cout << "        0 * inf     = NaN\n";
    std::cout << "        +0 == -0    = true  (same value, different bits)\n";
    std::cout << "Inf:    inf × inf   = inf\n";
    std::cout << "        inf - inf   = NaN\n";
    std::cout << "        inf ÷ inf   = NaN\n";
    std::cout << "        ±inf × ∓inf = NaN\n";
    std::cout << "NaN:    NaN op any  = NaN\n";
    std::cout << "        NaN ≠ any   = always true (including NaN ≠ NaN)\n";

    // Utility functions
    std::cout << "\n--- Floating-point Utility Functions (<cmath>) ---\n";
    std::cout << "std::isinf(inf)      = " << std::isinf(pos_inf)  << "\n";
    std::cout << "std::isinf(1.0f)     = " << std::isinf(1.0f)     << "\n";
    std::cout << "std::isfinite(1.0f)  = " << std::isfinite(1.0f)  << "\n";
    std::cout << "std::isfinite(inf)   = " << std::isfinite(pos_inf) << "\n";
    std::cout << "std::isnormal(1.0f)  = " << std::isnormal(1.0f)  << "\n";
    std::cout << "std::ldexp(1.0f, 8)  = " << std::ldexp(1.0f, 8)  << "  (1.0 × 2^8 = 256)\n";
    std::cout << "std::ilogb(448.0f)   = " << std::ilogb(448.0f)   << "  (exponent = 8)\n";
    (void)neg_inf; // suppress unused warning
}

// =============================================================================
// SECTION 12: Machine Epsilon and Units at the Last Place (ULP)
// =============================================================================
//
// MACHINE EPSILON (ε):
//   The smallest number that, when added to 1.0, gives a result ≠ 1.0.
//   It is the gap between 1.0 and the next representable float.
//   float  ε = 2^-23 ≈ 1.19209 × 10^-7
//   double ε = 2^-52 ≈ 2.22045 × 10^-16
//
// ULP (Units at the Last Place):
//   The gap between consecutive floating-point numbers at a given magnitude.
//   ULP(p, e) = β^(e-(p-1)) → for IEEE754: 2^(e-(p-1))
//   Machine epsilon = ULP(p, 0) = gap between 1.0 and next representable number.
//   At larger magnitudes, the gap grows: ULP_x = ε × β^e(x)
//
// FLOATING-POINT REPRESENTATION OF A REAL NUMBER:
//   fl(x) = x × (1 + δ)   where |δ| is a small error
//   Absolute error: |fl(x) - x| ≤ ½ × ULP_x
//   Relative error: |fl(x) - x| / |x| ≤ ½ × ε

void section_epsilon_ulp() {
    std::cout << "\n=== Section 12: Machine Epsilon and ULP ===\n";

    constexpr float  eps_f = std::numeric_limits<float>::epsilon();
    constexpr double eps_d = std::numeric_limits<double>::epsilon();

    std::cout << std::setprecision(20);
    std::cout << "float  epsilon  = " << eps_f << "\n";
    std::cout << "double epsilon  = " << eps_d << "\n";

    // Demonstrate epsilon: 1.0 + ε ≠ 1.0, but 1.0 + ε/2 == 1.0
    std::cout << "\n1.0f + epsilon  != 1.0f: " << std::boolalpha << (1.0f + eps_f != 1.0f) << "\n";
    std::cout << "1.0f + eps/2    == 1.0f: " << (1.0f + eps_f / 2.0f == 1.0f) << "\n";

    // ULP demonstration: gap between adjacent floats grows with magnitude
    float small_val = 1.0f;
    float large_val = 1e7f;
    // The next representable float after x:
    float next_small = small_val + eps_f * small_val;
    float next_large = large_val + eps_f * large_val;
    std::cout << "\nULP (gap between adjacent floats):\n";
    std::cout << "  ULP near 1.0e0:  " << (next_small - small_val) << "\n";
    std::cout << "  ULP near 1.0e7:  " << (next_large - large_val) << "\n";
    std::cout << "  (gap grows with magnitude)\n";

    // Float has fewer significant digits than double:
    std::cout << "\nFloat precision < double precision:\n";
    std::cout << "  (int)16777217.0f  = " << static_cast<int>(16777217.0f) << "  (WRONG — precision lost!)\n";
    std::cout << "  (int)16777217.0   = " << static_cast<int>(16777217.0)  << "  (double: correct)\n";
    std::cout << "  (1.1f != 1.1)     = " << (1.1f != 1.1) << "  (float != double!)\n";
    std::cout << std::setprecision(6);  // reset
}

// =============================================================================
// SECTION 13: Floating-Point Arithmetic Properties
// =============================================================================
//
// In IEEE754, floating-point operations (⊕,⊖,⊗,⊘) differ from exact ops (+,-,*,/):
//
// P1: In general, a op b ≠ a ⊙ b  (fl-op introduces rounding error)
// P2: NOT Reflexive: a ≠ a  (true when a = NaN; NaN ≠ NaN always)
// P3: NOT Commutative: a ⊙ b ≠ b ⊙ a  (true when either is NaN)
//     Commutative without NaN
// P4: NOT Associative: (a ⊕ b) ⊕ c ≠ a ⊕ (b ⊕ c)
//     Even excluding NaN and inf in intermediate computations
// P5: NOT Distributive: (a ⊕ b) ⊗ c ≠ (a ⊗ c) ⊕ (b ⊗ c)
// P6: Identity operations NOT ensured:
//     (a ⊖ b) ⊕ b ≠ a   and   (a ⊘ b) ⊗ b ≠ a
// P7: Overflow/Underflow → saturation values: ±inf (not wraparound)
//
// IEEE754 GUARANTEES: each individual operation is correctly rounded
// (result = exact value, then rounded to nearest representable).
// Deterministic: same result for same inputs on all IEEE754-compliant hardware.

void section_fp_arithmetic_properties() {
    std::cout << "\n=== Section 13: Floating-Point Arithmetic Properties ===\n";

    std::cout << std::setprecision(20);

    // P2: Not reflexive (NaN)
    float nan = std::numeric_limits<float>::quiet_NaN();
    std::cout << "P2 (not reflexive): NaN == NaN → " << std::boolalpha << (nan == nan) << "\n";

    // P4: Not associative
    float a = 0.1f, b = 0.2f, c = 0.3f;
    bool assoc = ((a + b) + c) == (a + (b + c));
    std::cout << "P4 (not associative): (0.1+0.2)+0.3 == 0.1+(0.2+0.3) → " << assoc << "\n";
    std::cout << "  (0.1+0.2)+0.3 = " << ((a + b) + c) << "\n";
    std::cout << "  0.1+(0.2+0.3) = " << (a + (b + c)) << "\n";

    // P4: Another example
    double big = 1e15, one = 1.0, neg_big = -1e15;
    double r1 = (big + one) + neg_big;   // (1e15+1) - 1e15 = 1 (correct)
    double r2 = big + (one + neg_big);   // 1e15 + (1 - 1e15) = 1e15 - 1e15 + 1 ... loss
    std::cout << "  (1e15+1)-1e15 = " << r1 << "\n";
    std::cout << "  1e15+(1-1e15) = " << r2 << "  (catastrophic cancellation)\n";

    // P1/P6: Finite precision
    std::cout << "\nP1 (finite precision):\n";
    std::cout << "  3.33333333f    = " << 3.33333333f    << "\n";
    std::cout << "  3.33333333     = " << 3.33333333     << "\n";
    std::cout << "  0.1+0.1+0.1... = " << (0.1+0.1+0.1+0.1+0.1+0.1) << " (not 0.6)\n";

    // P7: Overflow → inf (saturation), not wraparound
    float large = std::numeric_limits<float>::max();
    std::cout << "\nP7 (overflow → inf, not wraparound):\n";
    std::cout << "  FLT_MAX * 2.0f = " << (large * 2.0f) << "\n";

    std::cout << std::setprecision(6);  // reset
}

// =============================================================================
// SECTION 14: Floating-Point Undefined Behavior
// =============================================================================
//
// Despite IEEE754, C++ defines certain floating-point operations as UB:
//   1. Division by zero with integers: 0/0 (integer) → UB
//      Division by zero with floats:   1.0/0.0      → inf (IEEE754, well-defined in C++)
//      HOWEVER: in practice C++ treats float /0 as UB unless FENV is used
//   2. Conversion to a narrower floating-point type out of range:
//      e.g., 1e400 (double) → float  → UB
//   3. Conversion from floating-point to integer when value doesn't fit:
//      e.g., (int)(1e8f)  → OK; (int)(1e38f) → UB (value doesn't fit in int)
//   4. Operations on signaling NaNs
//   5. Assuming IEEE754 compliance on all platforms (some embedded ARM do not)
//
// DETECTING FLOATING-POINT EXCEPTIONS (<cfenv> — C++11):
//   Macros: FE_DIVBYZERO, FE_INEXACT, FE_INVALID, FE_OVERFLOW, FE_UNDERFLOW
//   Functions: std::feclearexcept(FE_ALL_EXCEPT), std::fetestexcept(<macro>)
//   Requires: #pragma STDC FENV_ACCESS ON  (supported by GCC, not always Clang)

void section_fp_undefined_behavior() {
    std::cout << "\n=== Section 14: Floating-Point Undefined Behavior / cfenv ===\n";

    // Safe: float divide-by-zero → inf (IEEE754)
    auto fp_divzero = 1.0f / 0.0f;
    std::cout << "1.0f / 0.0f = " << fp_divzero << "  (inf — safe in C++ float context)\n";

    // Safe conversion
    auto large_double = 1e300;
    std::cout << "1e300 double fits: " << large_double << "\n";
    // Unsafe (UB — overflow on conversion):
    // float overflow_f = (float)1e300;  // UB: 1e300 > FLT_MAX

    // Demonstrate cfenv exception detection
    // Note: requires compiler support for floating-point environment access
    std::feclearexcept(FE_ALL_EXCEPT);
    volatile float x = 1.0f / 0.0f;   // division by zero (float → inf)
    (void)x;
    std::cout << "\ncfenv: FE_DIVBYZERO after 1.0/0.0: "
              << std::boolalpha << (bool)std::fetestexcept(FE_DIVBYZERO) << "\n";

    std::feclearexcept(FE_ALL_EXCEPT);
    volatile float x2 = 0.0f / 0.0f;  // 0/0 → NaN → FE_INVALID
    (void)x2;
    std::cout << "cfenv: FE_INVALID after 0.0/0.0:   "
              << (bool)std::fetestexcept(FE_INVALID) << "\n";

    std::feclearexcept(FE_ALL_EXCEPT);
    volatile float x4 = 1e38f * 10.0f;  // overflow → FE_OVERFLOW
    (void)x4;
    std::cout << "cfenv: FE_OVERFLOW after 1e38*10:   "
              << (bool)std::fetestexcept(FE_OVERFLOW) << "\n";
}

// =============================================================================
// SECTION 15: Catastrophic Cancellation
// =============================================================================
//
// CATASTROPHIC CANCELLATION (loss of significance): loss of relevant information
// in a floating-point computation that cannot be recovered.
//
// Two cases:
//
// (C1) a ± b where a >> b OR b >> a:
//   The smaller number's significant bits are lost (granularity effect).
//   Example: float x = 10,000,000; subtracting y = 1 does nothing.
//   At magnitude 2^24 = 16,777,216, float integers have gaps > 1.
//
// (C2) a - b where a ≈ b and both are approximations of exact values:
//   The subtraction cancels most significant bits → large relative error.
//   Classic case: quadratic formula with large |b|:
//     x = (-b + √(b²-4ac)) / 2a  — when b is large, (-b + √(b²-4ac)) ≈ 0

void section_catastrophic_cancellation() {
    std::cout << "\n=== Section 15: Catastrophic Cancellation ===\n";

    // --- C1: Granularity ---
    std::cout << "--- Case C1: Granularity ---\n";

    // Float granularity: integers > 2^24 lose unit precision
    float x1 = 16777216.0f;   // = 2^24, exact
    float x2 = 16777217.0f;   // = 2^24 + 1, NOT representable in float → rounds to 2^24
    std::cout << "(float)16777216 = " << x1 << "\n";
    std::cout << "(float)16777217 = " << x2 << "  (rounded down — same as 16777216!)\n";
    std::cout << "float 16777216 == float 16777217: " << std::boolalpha << (x1 == x2) << "\n";

    // Loop that doesn't terminate: x=30,000,000, subtracting 1 does nothing
    float loop_x = 30000000.0f;
    int iterations = 0;
    while (loop_x > 29999990.0f && iterations < 20) {
        loop_x -= 1.0f;   // 1.0 is lost at this magnitude!
        iterations++;
    }
    std::cout << "\nLoop 'while(x>29999990) x-=1' starting at 30,000,000:\n";
    std::cout << "  Terminated after " << iterations << " iterations (max=20 imposed)\n";
    std::cout << "  x after " << iterations << " steps = " << loop_x << "  (stuck!)\n";

    // Floating-point increment accumulation
    float acc = 0.0f;
    for (int i = 0; i < 20000000; i++) acc += 1.0f;
    std::cout << "\nAdding 1.0f twenty million times: result = " << acc
              << "  (expected 20,000,000)\n";

    // --- C2: Subtracting nearly-equal approximations ---
    std::cout << "\n--- Case C2: Quadratic Formula ---\n";
    // x² + 5000x + 0.25 = 0
    // Correct answer: x ≈ -0.00005 (and x ≈ -4999.99995)
    float fa = 1.0f, fb = 5000.0f, fc = 0.25f;
    float disc = std::sqrt(fb*fb - 4.0f*fa*fc);
    float x_cat = (-fb + disc) / (2.0f * fa);   // catastrophic: -5000 + ≈5000
    float x_ok  = (-fb - disc) / (2.0f * fa);   // fine for the other root

    std::cout << std::setprecision(10);
    std::cout << "x² + 5000x + 0.25 = 0\n";
    std::cout << "  discriminant (float): " << disc << "\n";
    std::cout << "  (-b + √disc) / 2a  = " << x_cat << "  (WRONG, catastrophic cancellation)\n";
    std::cout << "  (-b - √disc) / 2a  = " << x_ok  << "  (ok for this root)\n";
    std::cout << "  Correct answer:       ≈ -0.000050\n";
    std::cout << "  Relative error: ~100%\n";

    // Fix: use the algebraically-equivalent form for the small root
    // x₁ = (-b + √(b²-4ac)) / 2a = 2c / (-b - √(b²-4ac))
    float x_fixed = (2.0f * fc) / (-fb - disc);
    std::cout << "  Fixed form 2c/(-b-√disc) = " << x_fixed << "  (closer to correct)\n";
    std::cout << std::setprecision(6);
}

// =============================================================================
// SECTION 16: Floating-Point Comparison
// =============================================================================
//
// PROBLEM: Floating-point equality is almost never exact.
//   0.11f + 0.11f < 0.22f  →  true!   (rounding makes them unequal)
//   0.1f + 0.1f > 0.2f     →  true!
//
// NAIVE (absolute error): |a - b| < ε
//   ✗ ε too large for very small numbers
//   ✗ ε too small for very large numbers
//
// BETTER (relative error): |a - b| / |b| < ε
//   ✗ b = 0 → division by zero (NaN < ε → always false)
//   ✗ a = 0 → 0/0
//   ✗ Not commutative (always divides by b)
//
// ROBUST SOLUTION: |a - b| / max(|a|, |b|) < ε
//   + Near-zero check: use absolute comparison when diff < normal_min
//   + Guard against inf/NaN inputs

bool are_float_nearly_equal(float a, float b, float relative_error = 1e-5f) {
    // Guard: reject inf or NaN
    if (!std::isfinite(a) || !std::isfinite(b))
        return false;

    float diff    = std::abs(a - b);
    float normal_min = std::numeric_limits<float>::min();

    // Near-zero: use absolute comparison
    if (diff <= normal_min)
        return true;

    // Relative comparison
    float abs_a = std::abs(a);
    float abs_b = std::abs(b);
    return (diff / std::max(abs_a, abs_b)) <= relative_error;
}

void section_fp_comparison() {
    std::cout << "\n=== Section 16: Floating-Point Comparison ===\n";

    std::cout << "The problem:\n";
    std::cout << "  0.11f+0.11f < 0.22f: " << std::boolalpha << (0.11f+0.11f < 0.22f) << "\n";
    std::cout << "  0.1f+0.1f > 0.2f:   " << (0.1f+0.1f > 0.2f) << "\n";

    std::cout << "\nNaive absolute comparison (buggy):\n";
    float eps = 1e-6f;
    std::cout << "  |0.11f+0.11f - 0.22f| < 1e-6 = "
              << (std::abs(0.11f+0.11f - 0.22f) < eps) << "\n";

    std::cout << "\nRobust relative comparison (are_float_nearly_equal):\n";
    std::cout << "  0.1+0.2 ≈ 0.3?       " << are_float_nearly_equal(0.1f+0.2f, 0.3f) << "\n";
    std::cout << "  1.0f ≈ 1.0f+eps/2?   " << are_float_nearly_equal(1.0f, 1.0f + std::numeric_limits<float>::epsilon()/2.0f) << "\n";
    std::cout << "  1e10f ≈ 1e10f+1?     " << are_float_nearly_equal(1e10f, 1e10f + 1.0f) << "\n";
    std::cout << "  0.0f ≈ 1e-40f?       " << are_float_nearly_equal(0.0f, 1e-40f) << "  (near-zero handled)\n";
    std::cout << "  inf ≈ inf?            " << are_float_nearly_equal(std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity()) << "  (inf excluded)\n";
}

// =============================================================================
// SECTION 17: Minimize Error Propagation
// =============================================================================
//
// Best practices to reduce floating-point error accumulation:
//   ✓ Prefer multiplication/division over addition/subtraction
//   ✓ Keep near-equal numbers together (sort inputs)
//   ✓ Threshold very small results to zero (iterative algorithms)
//   ✓ Scale by powers of 2 (exact in floating-point)
//   ✓ Use log scale: ×→+, ÷→−
//   ✓ Use compensation algorithms (Kahan summation, Dekker's FastTwoSum)
//
// KAHAN SUMMATION: reduces accumulated rounding error in large sums
//   Standard: error grows as O(n · ε)
//   Kahan:    error bounded by O(ε) regardless of n

float kahan_sum(const std::vector<float>& vals) {
    float sum = 0.0f, compensation = 0.0f;
    for (float v : vals) {
        float y = v - compensation;         // compensated value
        float t = sum + y;                  // new sum (rounding occurs here)
        compensation = (t - sum) - y;       // capture the rounding error
        sum = t;
    }
    return sum;
}

void section_error_propagation() {
    std::cout << "\n=== Section 17: Error Propagation / Kahan Summation ===\n";

    // Demonstrate: adding 1.0f twenty million times with naive vs Kahan
    const int N = 20000000;
    const float val = 1.0f;

    // Naive accumulation (std::accumulate)
    std::vector<float> ones(N, val);
    float naive = std::accumulate(ones.begin(), ones.end(), 0.0f);

    // Kahan summation
    float kahan = kahan_sum(ones);

    std::cout << "Summing 1.0f " << N << " times:\n";
    std::cout << std::setprecision(2);
    std::cout << "  Expected:         " << static_cast<float>(N) << "\n";
    std::cout << "  Naive accumulate: " << naive  << "\n";
    std::cout << "  Kahan sum:        " << kahan  << "\n";
    std::cout << "  Naive error:      " << std::abs(naive - static_cast<float>(N)) << "\n";
    std::cout << "  Kahan error:      " << std::abs(kahan - static_cast<float>(N)) << "\n";
    std::cout << std::setprecision(6);

    std::cout << "\nScale by power-of-2 is exact:\n";
    float x = 3.14159f;
    std::cout << "  3.14159f × 4.0f  = " << (x * 4.0f)  << "  (exact shift, no rounding)\n";
    std::cout << "  3.14159f × 3.0f  = " << (x * 3.0f)  << "  (not power of 2 → rounding)\n";
}

// =============================================================================
// MAIN
// =============================================================================

int main() {
    std::cout << "=== Unit 4: Basic Concepts II ===\n";
    std::cout << "    Integer Types, Overflow, and Floating-Point Arithmetic\n";

    section_data_models();
    section_fixed_width_integers();
    section_overflow_semantics();
    section_signed_integer();
    section_unsigned_integer();
    section_numeric_limits();
    section_promotion_truncation();
    section_mixed_sign_errors();
    section_overflow_detection();
    section_ieee754_representation();
    section_special_values();
    section_epsilon_ulp();
    section_fp_arithmetic_properties();
    section_fp_undefined_behavior();
    section_catastrophic_cancellation();
    section_fp_comparison();
    section_error_propagation();

    std::cout << "\n=== Done ===\n";
    return 0;
}
