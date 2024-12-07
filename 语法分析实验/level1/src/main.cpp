#include <iostream>
#include <map>
#include <set>
#include <stack>
#include <string>
#include <vector>

std::map<int, std::pair<char, std::string>> prods;
std::map<char, std::map<char, int>> M;
std::set<char> terminals;
std::set<char> nonterminals;
constexpr char startSymbol = 'E';
std::map<char, std::vector<std::string>> grammar;

void buildGrammar()
{
    grammar['E'].emplace_back("TA");
    grammar['A'].emplace_back("+TA");
    grammar['A'].emplace_back("-TA");
    grammar['A'].emplace_back(""); // ε
    grammar['T'].emplace_back("FB");
    grammar['B'].emplace_back("*FB");
    grammar['B'].emplace_back("/FB");
    grammar['B'].emplace_back(""); // ε
    grammar['F'].emplace_back("(E)");
    grammar['F'].emplace_back("n");

    nonterminals = { 'E', 'A', 'T', 'B', 'F' };
    terminals = { 'n', '+', '-', '*', '/', '(', ')', '$' };

    prods[1] = std::make_pair('E', "TA");
    prods[2] = std::make_pair('A', "+TA");
    prods[3] = std::make_pair('A', "-TA");
    prods[4] = std::make_pair('A', "");
    prods[5] = std::make_pair('T', "FB");
    prods[6] = std::make_pair('B', "*FB");
    prods[7] = std::make_pair('B', "/FB");
    prods[8] = std::make_pair('B', "");
    prods[9] = std::make_pair('F', "(E)");
    prods[10] = std::make_pair('F', "n");
}

std::map<char, std::set<char>> FIRSTSets;
std::map<char, std::set<char>> FOLLOWSets;

bool isTerminal(char c)
{
    return terminals.count(c) != 0U;
}

std::set<char> FIRST_str(const std::string& str)
{
    std::set<char> result;
    if (str.empty()) {
        result.insert('@');
        return result;
    }
    for (int i = 0; i < static_cast<int>(str.size()); i++) {
        char X = str[i];
        if (isTerminal(X)) {
            result.insert(X);
            return result;
        }
        bool hasEpsilon = false;
        for (auto c : FIRSTSets[X]) {
            if (c == '@') {
                hasEpsilon = true;
            } else {
                result.insert(c);
            }
        }
        if (!hasEpsilon) {
            return result;
        }
        if (i == static_cast<int>(str.size()) - 1) {
            result.insert('@');
        }
    }
    return result;
}

void buildFIRST()
{
    bool changed = true;
    for (auto nt : nonterminals) {
        FIRSTSets[nt]; // init empty
    }

    while (changed) {
        changed = false;
        for (auto nt : nonterminals) {
            for (auto& rhs : grammar[nt]) {
                std::set<char> f = FIRST_str(rhs);
                int oldSize = static_cast<int>(FIRSTSets[nt].size());
                for (auto c : f) {
                    FIRSTSets[nt].insert(c);
                }
                if (static_cast<int>(FIRSTSets[nt].size()) > oldSize) {
                    changed = true;
                }
            }
        }
    }
}

void buildFOLLOW()
{
    for (auto nt : nonterminals) {
        FOLLOWSets[nt];
    }
    FOLLOWSets['E'].insert('$');

    bool changed = true;
    while (changed) {
        changed = false;
        for (auto nt : nonterminals) {
            for (auto& rhs : grammar[nt]) {
                for (int i = 0; i < static_cast<int>(rhs.size()); i++) {
                    char B = rhs[i];
                    if (nonterminals.count(B) != 0U) {
                        std::string beta = rhs.substr(i + 1);
                        std::set<char> firstBeta = FIRST_str(beta);
                        int oldSize = static_cast<int>(FOLLOWSets[B].size());
                        if (firstBeta.count('@') != 0U) {
                            firstBeta.erase('@');
                            for (auto c : firstBeta) {
                                FOLLOWSets[B].insert(c);
                            }
                            for (auto c : FOLLOWSets[nt]) {
                                FOLLOWSets[B].insert(c);
                            }
                        } else {
                            for (auto c : firstBeta) {
                                FOLLOWSets[B].insert(c);
                            }
                        }
                        if (static_cast<int>(FOLLOWSets[B].size()) > oldSize) {
                            changed = true;
                        }
                    }
                }
            }
        }
    }
}

void buildParsingTable()
{
    for (auto& p : prods) {
        char A = p.second.first;
        std::string alpha = p.second.second;
        std::set<char> firstSet = FIRST_str(alpha);
        bool hasEpsilon = (firstSet.count('@') != 0U);
        for (auto a : firstSet) {
            if (a != '@') {
                M[A][a] = p.first;
            }
        }
        if (hasEpsilon) {
            for (auto b : FOLLOWSets[A]) {
                M[A][b] = p.first;
            }
        }
    }
}

void printStep(const std::stack<char>& st, const std::string& input_line, int ip, const std::string& action)
{
    {
        std::vector<char> v;
        std::stack<char> tmp = st;
        while (!tmp.empty()) {
            v.push_back(tmp.top());
            tmp.pop();
        }
        for (int i = static_cast<int>(v.size()) - 1; i >= 0; i--) {
            std::cout << v[i];
        }
    }
    std::cout << "\t";
    for (int i = ip; i < static_cast<int>(input_line.size()); i++) {
        std::cout << input_line[i];
    }
    std::cout << "\t";
    std::cout << action << "\n";
}

void LLparse(const std::string& input_line)
{
    std::stack<char> st;
    st.push('$');
    st.push(startSymbol);

    int ip = 0;
    bool done = false;
    bool error_flag = false;

    while (!done && !error_flag) {
        char X = st.top();
        char a = input_line[ip];

        if ((terminals.count(X) != 0U || X == '$')) {
            if (X == a) {
                if (X == '$') {
                    printStep(st, input_line, ip, "accept");
                    done = true;
                } else {
                    printStep(st, input_line, ip, "match");
                    st.pop();
                    ip++;
                }
            } else {
                printStep(st, input_line, ip, "error");
                error_flag = true;
            }
        } else {
            if (M[X].find(a) != M[X].end()) {
                int p = M[X][a];
                std::string rhs = prods[p].second;
                printStep(st, input_line, ip, std::to_string(p));
                st.pop();
                for (int i = static_cast<int>(rhs.size()) - 1; i >= 0; i--) {
                    if (rhs[i] != '@' && rhs[i] != 0) {
                        st.push(rhs[i]);
                    }
                }
            } else {
                printStep(st, input_line, ip, "error");
                error_flag = true;
            }
        }
    }
}

int main()
{
    std::string input_line;
    std::getline(std::cin, input_line);
    input_line.push_back('$');

    buildGrammar();
    buildFIRST();
    buildFOLLOW();
    buildParsingTable();

    LLparse(input_line);

    return 0;
}