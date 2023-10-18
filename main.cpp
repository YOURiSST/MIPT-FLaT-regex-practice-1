#include <iostream>
#include <stack>
#include <vector>

#include "regex_solver.h"

signed main() {
    std::cout << "input format is regex, letter & repeats\n";
    std::string regex;
    char k;
    int x;

    std::cin >> regex >> k >> x;

    RegexSolver regex_solver(std::move(regex), x, k);
    std::cout << "\naaaand answer is: " << regex_solver.SolveIt();
    return 0;
}
