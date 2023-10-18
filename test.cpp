#include <algorithm>
#include <iostream>
#include <stack>
#include <string>
#include <vector>

#include "regex_solver.h"

signed main() {
    struct TestCase {
        int testnum;
        std::string regex;
        char ch_to_repeat;
        int repeat_num;
        int ans;
        TestCase(int testnum, std::string reg, char ch, int rep, int ans)
            : testnum(testnum),
              regex(std::move(reg)),
              ch_to_repeat(ch),
              repeat_num(rep),
              ans(ans) {}
    };

    std::vector<TestCase> test_cases{
        {1, "aa+**", 'a', 3, 3},
        {2, "aa+**", 'b', 3, int(1e9 + 7)},
        {3, "aa+**", 'a', 1, 1},
        {4, "ba.ab.+", 'a', 1, 2},
        {5, "bbb..", 'b', 4, int(1e9 + 7)},
        {6, "bbb..*", 'b', 4, 6},
        {7,
         "ab+c.aba.*.bac.+.+*ab+c.aba.*.bac.+.+*.ab+"
         "c.aba.*.bac.+.+*.ab+c.aba.*.bac.+.+*.ab+c.aba.*.bac.+.+*.ab+c.aba.*."
         "bac.+.+*.ab+c.aba.*.bac.+.+*.ab+c.aba.*.bac.+.+*.ab+c.aba.*.bac.+.+*"
         ".",
         'b', 2, 4},
        {8,
         "ab+c.aba.*.bac.+.+*ab+c.aba.*.bac.+.+*.ab+"
         "c.aba.*.bac.+.+*.ab+c.aba.*.bac.+.+*.ab+c.aba.*.bac.+.+*.ab+c.aba.*."
         "bac.+.+*.ab+c.aba.*.bac.+.+*.ab+c.aba.*.bac.+.+*.ab+c.aba.*.bac.+.+*"
         ".bb.+",
         'b', 2, 2},
        {9, "aa.ab.++", 'b', 10, 228},
        {10, "asskek", 'a', 112, 911},
        {11, "a.", 'b', 10, 220},
    };

    for (auto& test : test_cases) {
        std::cerr << "testcase #" << test.testnum << ":\n";
        int ans;
        try {
            ans = RegexSolver(test.regex, test.repeat_num, test.ch_to_repeat).SolveIt();
        } catch (std::exception& e) {
            std::cerr << "nice, caught exception :: " << e.what() << "\n";
            continue;
        }
        if (ans != test.ans) {
            std::cerr << "test failed, got " << ans << ", expected " << test.ans << "\n";
            exit(-1);
        }
        std::cerr << "test passed, got " << ans << ", expected " << test.ans << "\n";
    }

    return 0;
}
