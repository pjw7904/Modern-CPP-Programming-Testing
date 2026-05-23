#include <iostream>
using namespace std;

int main()
{
    cout << "Printing char values:\n";
    char a = 65; // 'A'
    cout << a << "\n"; // print 'A' because char by default looks at the character representation, not the integral one. So 65 = character 'A'
    cout << +a << "\n"; // print '65' because you're using + (as in, the + unary operator to make a number positive) so it converts to an int
    cout << (a + 0) << "\n"; // print '65' for the same reason as abovem but as a binary operator.

    cout << "\nPrefix vs Suffix incrementing:\n";
    int b = 5;
    int c = ++b; // 'b' becomes 6, then 6 is assigned to 'c'
    // Result: b = 6, c = 6
    cout << "b = " << b << " | " << "c = " << c << "\n";

    int x = 5;
    int y = x++; // 5 is assigned to 'y', then 'x' becomes 6
    // Result: x = 6, y = 5
    cout << "x = " << x << " | " << "y = " << y << "\n";

    return 0;
}
