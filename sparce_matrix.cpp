#include "sparce_matrix.h"
#include <iostream>

int main()
{
    using D2m = DNmatr<2, int, 0>;
    D2m matrix;
    for (int i = 0; i < 10; ++i)
    {
        matrix(i, i) = i;
        matrix(i, 9 - i) = 9 - i;
    }

    for (int i = 1; i < 9; ++i)
    {
        for (int j = 1; j < 9; ++j)
            std::cout << matrix(i, j) << ' ';
        std::cout << '\n';
    }

    std::cout << "Кол-во занятых: " << matrix.size() << '\n';

    for (auto i = matrix.begin(); i != matrix.end(); ++i)
    {
        std::cout << '(';
        for (auto j = 0; j < 2; ++j)
            std::cout << i->first[j] << (j != 1 ? "," : ") -> ");
        std::cout << i->second << '\n';
    }

    // // 5 dimensional
    // std::cout << '\n'
    //           << "5 dimensional" << '\n';
    // using D5m = DNmatr<5, int, -1>;
    // D5m d;
    // d(0, 0, 0, 0, 0) = 75;
    // std::cout << "d(0, 0, 0, 0, 0): " << d(0, 0, 0, 0, 0) << '\n';
    // int i = d(0, 0, 0, 0, 0);
    // std::cout << "i: " << i << '\n';
    // d(0, 0, 0, 0, 0) = D2m::emp;
    // std::cout << "empty d(0, 0, 0, 0, 0): " << d(0, 0, 0, 0, 0) << '\n';

    return 0;
}
