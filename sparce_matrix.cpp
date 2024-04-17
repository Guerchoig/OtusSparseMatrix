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
        for (auto j = 0; j < 2; ++j)
            std::cout << i->first[j] << (j != 1 ? "," : " ");
        std::cout << i->second << '\n';
    }

    // D3M d;
    // d(0, 0, 0) = 75;
    // int i = d(0, 0, 0);
    // (void)i;
    // d(0, 0, 0) = D3M::emp;

    return 0;
}
