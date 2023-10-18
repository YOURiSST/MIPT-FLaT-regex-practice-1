#pragma once

#include <iostream>
#include <stack>
#include <vector>

class RegexSolver {
   private:
    static constexpr int INF = 1e9 + 7;

    static const char PLUS = '+';
    static const char DOT = '.';
    static const char STAR = '*';

    static const char ALPH_START = 'a';
    static const char ALPH_END = 'c';
    static const char EMPTY = '1';

    int repeats;
    char ch_to_repeat;

    class Condition {
       public:
        std::vector<int> min_lens_with_filled_prefix;
        std::vector<int> min_lens_with_filled_suffix;
        int min_len_ever;
        int min_len_with_solve;

        explicit Condition(int repeats);

        Condition(int repeats, bool true_chel);
    };

   public:
    std::string reversed_polish_regex;

    explicit RegexSolver(std::string reversed_polish_regex, int repeats, char ch_to_repeat);

    [[nodiscard]] static Condition GenerateEmptyWord(int repeats);

    [[nodiscard]] Condition Sum(const Condition& ft, const Condition& sc) const;

    [[nodiscard]] Condition Dot(const Condition& ft, const Condition& sc) const;

    [[nodiscard]] Condition Star(const Condition& ft) const;

    static bool BadSymbol(char ch);

    static bool IsLetter(char ch);

    int SolveIt();
};