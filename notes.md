# Unit 01

* C++ was first released in 1985.

* Performance between C and C++ is nearly identical for identical programs. When using the same compiler and identical code logic, both languages typically produce nearly identical machine code with 100% equivalent performance.

* C++ is written to use the zero overhead principle / zero-cost abstraction:
    * Any abstraction should not cost anything compared to writing the equivalent code at a lower level.

* Written for low memory and energy consumption.

* "C++ is for people who want to use hardware very well and manage the complexity of doing that through abstraction"

* C++ is not a superset of C, there are C programs that will not compile in C++. The diverging standards, updates, and design philosophies cause this. A common subset between the two was a goal, but no longer seems to be the case.

* However, you can write C++ code that looks a lot like C and then add the C++-specific stuff when needed. KISS method, you don't need the advanced stuff unless you do.

* You can write C++ code as procedural or object-oriented style, much like Python. It does not enforce OOP like Python, but technically everything in Python is an object so it's even less OOP than Python.

# Unit 02

* Added to ~/.bashrc an alias for the following compiliation command:
    ```bash
    clang++ -std=c++17 -Wall -Wextra -fsanitize=address,undefined -Wpedantic hello.cpp -o hello
    ```

    The bash code is as follows:
    ```bash
    cppbuild() {
    clang++ -std=c++17 -Wall -Wextra -fsanitize=address,undefined -Wpedantic "$1" -o "${2:-${1%.cpp}}"
    }
    ```

    The ```${2:-${1%.cpp}}``` part means: use the second argument if provided, otherwise strip .cpp from the filename and use that as the output name. After adding it, reload your shell with source ~/.bashrc.

# Unit 03

* Statically and strongly typed language. Meaning, everything has an explicit type associated with it and you cannot change it without explicity calling code that does so (minus some minor things like int to double).

* Integral types, unsigned version for clarity:

| type | size | range |
-------|------|-------|
| char | 1 byte | 0 to 2^8-1 |
| short | 2 bytes | 0 to 2^16-1 |
| int | 4 bytes | 0 to 2^32-1 |
| long long int | 8 bytes | 0 to 2^64-1 |

* floating-point types are huge, float (4 bytes) and double (8 bytes). There is no suffix for double, but there is for float (e.g., ```float x = 3.0f```)

* Any operation between two integral types smaller than int (char and short) results in the promotion of the answer to an int. Like how an int and a double result in a double.

* Be careful when using char, because the cout default behavior is to convert the integral char value into its character value. So:

    ```c++
    char a = 65;
    cout << a << "\n"; // prints 'A'
    ```

* The prefix ++i and suffix i++ operators affect the order in which the statement is evaluated. With the prefix ++i, the value of i is incremented, and that is what is used. But with the suffix i++, the original value of i is used, and then it is incremented for anything following that line.

    ```c++
    int a = 5;
    int b = ++a; // 'a' becomes 6, then 6 is assigned to 'b'
    // Result: a = 6, b = 6

    int x = 5;
    int y = x++; // 5 is assigned to 'y', then 'x' becomes 6
    // Result: x = 6, y = 5
    ```

* You can use and instead of ```&&```, or instead of ```||```, but apparently it isn't professional and C obviously cannot handle it as well if you're porting code.