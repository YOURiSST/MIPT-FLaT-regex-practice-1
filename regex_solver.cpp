#include "regex_solver.h"


RegexSolver::Condition::Condition(int repeats)
    : min_lens_with_filled_prefix(repeats + 1, INF),
      min_lens_with_filled_suffix(repeats + 1, INF),
      min_len_ever(INF),
      min_len_with_solve(INF) {}

RegexSolver::Condition::Condition(int repeats, bool true_chel)
    : min_lens_with_filled_prefix(repeats + 1, INF),
      min_lens_with_filled_suffix(repeats + 1, INF),
      min_len_ever(INF),
      min_len_with_solve(INF) {  // constructor for our ch_to_repeat
    min_len_ever = 1;
    if (!true_chel) {
        min_lens_with_filled_suffix[0] = 1;
        min_lens_with_filled_prefix[0] = 1;
        return;
    }
    if (repeats == 1) {
        min_len_with_solve = 1;
    }
    min_lens_with_filled_suffix[1] = 1;
    min_lens_with_filled_prefix[1] = 1;
}

RegexSolver::RegexSolver(std::string reversed_polish_regex, int repeats, char ch_to_repeat)
    : reversed_polish_regex(std::move(reversed_polish_regex)),
      repeats(repeats),
      ch_to_repeat(ch_to_repeat) {}

[[nodiscard]] RegexSolver::Condition RegexSolver::GenerateEmptyWord(int repeats) {
    Condition emptyword(repeats);
    emptyword.min_len_ever = 0;
    emptyword.min_lens_with_filled_suffix[0] = 0;
    emptyword.min_lens_with_filled_prefix[0] = 0;
    return emptyword;
}

[[nodiscard]] RegexSolver::Condition RegexSolver::Sum(const Condition& ft,
                                                      const Condition& sc) const {
    Condition res(repeats);

    for (int i = 0; i < repeats + 1; ++i) {
        res.min_lens_with_filled_prefix[i] =
            std::min(ft.min_lens_with_filled_prefix[i], sc.min_lens_with_filled_prefix[i]);

        res.min_lens_with_filled_suffix[i] =
            std::min(ft.min_lens_with_filled_suffix[i], sc.min_lens_with_filled_suffix[i]);

        res.min_len_ever = std::min(ft.min_len_ever, sc.min_len_ever);
        res.min_len_with_solve = std::min(ft.min_len_with_solve, sc.min_len_with_solve);
    }
    res.min_len_with_solve =
        std::min({res.min_len_with_solve, res.min_lens_with_filled_prefix.back(),
                  res.min_lens_with_filled_suffix.back()});
    return res;
}

[[nodiscard]] RegexSolver::Condition RegexSolver::Dot(const Condition& ft,
                                                      const Condition& sc) const {
    Condition res(repeats);

    for (int i = 0; i < repeats + 1; ++i) {
        res.min_lens_with_filled_prefix[i] =
            std::min(res.min_lens_with_filled_prefix[i],
                     ft.min_lens_with_filled_prefix[i] + sc.min_len_ever);

        if (ft.min_lens_with_filled_prefix[i] == i) {  // so we can continue sc pref with us
            for (int j = 0; j < repeats + 1; ++j) {
                int cur_ind = std::min(repeats, i + j);

                res.min_lens_with_filled_prefix[cur_ind] =
                    std::min(res.min_lens_with_filled_prefix[cur_ind],
                             i + sc.min_lens_with_filled_prefix[j]);
            }
        }
    }

    for (int i = 0; i < repeats + 1; ++i) {
        res.min_lens_with_filled_suffix[i] =
            std::min(res.min_lens_with_filled_suffix[i],
                     sc.min_lens_with_filled_suffix[i] + ft.min_len_ever);

        if (sc.min_lens_with_filled_suffix[i] == i) {
            for (int j = 0; j < repeats + 1; ++j) {
                int cur_ind = std::min(repeats, i + j);
                res.min_lens_with_filled_suffix[cur_ind] =
                    std::min(res.min_lens_with_filled_suffix[cur_ind],
                             i + ft.min_lens_with_filled_suffix[j]);
            }
        }
    }
    for (int i = 0; i < repeats + 1; ++i) {
        for (int j = (repeats - i); j < repeats + 1; ++j) {
            res.min_len_with_solve =
                std::min(res.min_len_with_solve, ft.min_lens_with_filled_suffix[i] +
                                                     sc.min_lens_with_filled_prefix[j]);
        }
    }


    res.min_len_with_solve =
        std::min({res.min_len_with_solve, ft.min_len_with_solve + sc.min_len_ever,
                  sc.min_len_with_solve + ft.min_len_ever});

    res.min_len_ever = ft.min_len_ever + sc.min_len_ever;

    res.min_len_with_solve =
        std::min({res.min_len_with_solve, res.min_lens_with_filled_prefix.back(),
                  res.min_lens_with_filled_suffix.back()});

    return res;
}

[[nodiscard]] RegexSolver::Condition RegexSolver::Star(const Condition& ft) const {
    Condition res = GenerateEmptyWord(repeats);
    Condition to_kek = ft;

    for (int i = 0; i < repeats + 1; ++i) {
        res = Sum(res, to_kek);
        to_kek = Dot(to_kek, to_kek);
    }
    res.min_len_with_solve =
        std::min({res.min_len_with_solve, res.min_lens_with_filled_prefix.back(),
                  res.min_lens_with_filled_suffix.back()});
    return res;
}

bool RegexSolver::BadSymbol(char ch) {
    return (ch < ALPH_START || ch > ALPH_END) && ch != '1' && ch != DOT && ch != PLUS && ch != STAR;
}

bool RegexSolver::IsLetter(char ch) { return ch >= ALPH_START && ch <= ALPH_END; }

int RegexSolver::SolveIt() {
    std::stack<Condition> conditions;
    for (int i = 0; i < reversed_polish_regex.size(); ++i) {
        if (BadSymbol(reversed_polish_regex[i])) {
            throw std::invalid_argument("bad symbol in regex, pos= " + std::to_string(i));
        }
        if (IsLetter(reversed_polish_regex[i])) {
            if (reversed_polish_regex[i] == ch_to_repeat) {
                conditions.push(Condition(repeats, true));
                continue;
            }
            conditions.push(Condition(repeats, false));
            continue;
        }
        if (reversed_polish_regex[i] == EMPTY) {
            conditions.push(GenerateEmptyWord(repeats));
            continue;
        }
        if (reversed_polish_regex[i] == PLUS) {
            if (conditions.size() < 2) {
                throw std::out_of_range(
                    "wrong regex, stack has size < 2, but operation is '+', pos=" +
                    std::to_string(i));
            }
            auto sc = std::move(conditions.top());
            conditions.pop();
            auto ft = std::move(conditions.top());
            conditions.pop();
            auto res = Sum(ft, sc);
            conditions.push(res);
            continue;
        }
        if (reversed_polish_regex[i] == DOT) {
            if (conditions.size() < 2) {
                throw std::out_of_range(
                    "wrong regex, stack has size < 2, but operation is '.', pos=" +
                    std::to_string(i));
            }
            auto sc = std::move(conditions.top());
            conditions.pop();
            auto ft = std::move(conditions.top());
            conditions.pop();
            auto res = Dot(ft, sc);
            conditions.push(res);
            continue;
        }
        if (reversed_polish_regex[i] == STAR) {
            if (conditions.empty()) {
                throw std::out_of_range("wrong regex, stack is empty, but operation is '*', pos=" +
                                        std::to_string(i));
            }
            auto cur = std::move(conditions.top());
            conditions.pop();
            cur = Star(cur);
            conditions.push(cur);
            continue;
        }
        throw std::logic_error("something is broken, but we cannot understand a reason.");
    }
    if (conditions.size() != 1) {
        throw std::logic_error("stack size is not 1 after all instructions, check your regex");
    }

    auto aa = GenerateEmptyWord(repeats);

    return conditions.top().min_len_with_solve;
}
