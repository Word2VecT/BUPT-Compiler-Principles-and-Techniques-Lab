#include <iostream>
#include <map>
#include <set>
#include <stack>
#include <string>
#include <utility>
#include <vector>

class Item {
public:
    std::string left;
    std::string right;
    int dot;
    char lookahead;

    bool operator<(const Item& o) const
    {
        if (left != o.left) {
            return left < o.left;
}
        if (right != o.right) {
            return right < o.right;
}
        if (dot != o.dot) {
            return dot < o.dot;
}
        return lookahead < o.lookahead;
    }
    bool operator==(const Item& o) const
    {
        return left == o.left && right == o.right && dot == o.dot && lookahead == o.lookahead;
    }
};

std::map<int, std::pair<std::string, std::string>> prods;
std::set<char> terminals, nonterminals;
constexpr const char* startSymbol = "E'";

std::map<char, std::set<char>> FIRSTSets;

std::map<std::set<Item>, int> Cidx;
std::vector<std::set<Item>> C;
std::map<int, std::map<char, std::string>> action;
std::map<int, std::map<char, int>> goTable;

std::set<char> FIRST_str(const std::string& str)
{
    std::set<char> result;
    if (str.empty()) {
        result.insert('@');
        return result;
    }
    for (int i = 0; i < static_cast<int>(str.size()); i++) {
        char X = str[i];
        if (terminals.count(X) != 0U) {
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
        if (i == static_cast<int>(str.size()) - 1 && hasEpsilon) {
            result.insert('@');
        }
    }
    return result;
}

void buildGrammar()
{
    terminals = { 'n', '+', '-', '*', '/', '(', ')', '$' };
    nonterminals = { 'E', 'T', 'F' };

    prods[0] = std::make_pair("E'", "E");
    prods[1] = std::make_pair("E", "E+T");
    prods[2] = std::make_pair("E", "E-T");
    prods[3] = std::make_pair("E", "T");
    prods[4] = std::make_pair("T", "T*F");
    prods[5] = std::make_pair("T", "T/F");
    prods[6] = std::make_pair("T", "F");
    prods[7] = std::make_pair("F", "(E)");
    prods[8] = std::make_pair("F", "n");
}

std::map<char, std::vector<std::string>> grammarLR;

bool isTerminal(char c)
{
    return terminals.count(c) > 0;
}

void buildGrammarForFIRST()
{
    grammarLR.clear();
    for (auto& p : prods) {
        char A = p.second.first[0];
        grammarLR[A].push_back(p.second.second);
    }
}

void buildFIRST()
{
    for (auto nt : nonterminals) {
        FIRSTSets[nt];
    }

    bool changed = true;
    while (changed) {
        changed = false;
        for (auto nt : nonterminals) {
            for (auto& rhs : grammarLR[nt]) {
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

std::set<Item> closure(const std::set<Item>& I)
{
    std::set<Item> J = I;
    bool changed = true;
    while (changed) {
        changed = false;
        std::set<Item> Jnew = J;
        for (const auto& it : J) {
            if (it.dot < static_cast<int>(it.right.size())) {
                char B = it.right[it.dot];
                if (nonterminals.count(B) != 0U) {
                    std::string beta = it.right.substr(it.dot + 1);
                    beta.push_back(it.lookahead);
                    std::set<char> firstSet = FIRST_str(beta);
                    if (firstSet.empty()) {
                        firstSet.insert(it.lookahead);
}
                    for (auto& p : prods) {
                        if (p.second.first == std::string(1, B)) {
                            for (auto b : firstSet) {
                                Item newItem = { p.second.first, p.second.second, 0, b };
                                if (J.count(newItem) == 0U) {
                                    Jnew.insert(newItem);
                                }
                            }
                        }
                    }
                }
            }
        }
        if (Jnew.size() != J.size()) {
            J = Jnew;
            changed = true;
        }
    }
    return J;
}

std::set<Item> goFunc(const std::set<Item>& I, char X)
{
    std::set<Item> J;
    for (const auto& it : I) {
        if (it.dot < static_cast<int>(it.right.size()) && it.right[it.dot] == X) {
            J.insert({ it.left, it.right, it.dot + 1, it.lookahead });
        }
    }
    return closure(J);
}

int getIndex(const std::set<Item>& I)
{
    if (Cidx.find(I) != Cidx.end()) {
        return Cidx[I];
}
    int idx = static_cast<int>(C.size());
    C.push_back(I);
    Cidx[I] = idx;
    return idx;
}

void buildAutomaton()
{
    std::set<Item> I0;
    I0.insert({ startSymbol, "E", 0, '$' });
    I0 = closure(I0);
    getIndex(I0);
    bool added = true;
    while (added) {
        added = false;
        int n = static_cast<int>(C.size());
        for (int i = 0; i < n; i++) {
            std::set<char> Xs;
            for (const auto& it : C[i]) {
                if (it.dot < static_cast<int>(it.right.size())) {
                    Xs.insert(it.right[it.dot]);
                }
            }
            for (auto X : Xs) {
                std::set<Item> g = goFunc(C[i], X);
                if (!g.empty()) {
                    int j = 0;
                    if (Cidx.find(g) == Cidx.end()) {
                        j = getIndex(g);
                        added = true;
                    } else {
                        j = Cidx[g];
}
                    if (terminals.count(X) != 0U) {
                        action[i][X] = "s" + std::to_string(j);
                    }
                    if (nonterminals.count(X) != 0U) {
                        goTable[i][X] = j;
                    }
                }
            }
        }
    }
}

void buildActionGoto()
{
    for (int i = 0; i < static_cast<int>(C.size()); i++) {
        for (const auto& it : C[i]) {
            if (it.dot == static_cast<int>(it.right.size())) {
                if (it.left == startSymbol && it.lookahead == '$') {
                    action[i]['$'] = "acc";
                } else {
                    for (auto& p : prods) {
                        if (p.second.first == it.left && p.second.second == it.right) {
                            action[i][it.lookahead] = "r" + std::to_string(p.first);
                        }
                    }
                }
            }
        }
    }
    for (int i = 0; i < static_cast<int>(C.size()); i++) {
        for (const auto& t : terminals) {
            if (action[i].find(t) == action[i].end()) {
                action[i][t] = "err";
            }
        }
        action[i]['$'] = (action[i].find('$') == action[i].end()) ? "err" : action[i]['$'];
    }
}

void LRparse(const std::string& input)
{
    std::stack<int> stateStack;
    std::stack<char> symbolStack;
    stateStack.push(0);
    int ip = 0;
    bool done = false;
    while (!done) {
        int S = stateStack.top();
        char a = input[ip];
        std::string act = (action[S].find(a) != action[S].end()) ? action[S][a] : "err";
        if (act == "acc") {
            std::cout << "accept\n";
            done = true;
        } else if (act[0] == 's') {
            std::cout << "shift\n";
            int nxt = std::stoi(act.substr(1));
            symbolStack.push(a);
            stateStack.push(nxt);
            ip++;
        } else if (act[0] == 'r') {
            int prodNo = std::stoi(act.substr(1));
            std::string A = prods[prodNo].first;
            std::string B = prods[prodNo].second;
            int len = static_cast<int>(B.size());
            for (int i = 0; i < len; i++) {
                symbolStack.pop();
                stateStack.pop();
            }
            int S2 = stateStack.top();
            symbolStack.push(A[0]);
            stateStack.push(goTable[S2][A[0]]);
            std::cout << prodNo << "\n";
        } else {
            std::cout << "error\n";
            done = true;
        }
    }
}

int main()
{
    buildGrammar();
    buildGrammarForFIRST();
    buildFIRST();
    buildAutomaton();
    buildActionGoto();

    std::string input_line;
    std::getline(std::cin, input_line);
    input_line.push_back('$');

    LRparse(input_line);

    return 0;
}