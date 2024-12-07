/*
 * @Author: Zinan Tang
 * @Date: 2024-09-24 12:00:27
 * @Description:
 */
// 目前只支持测评单文件程序，所以需要将代码都写到这个文件中。

/*
 ! 末尾需要一个空格来读入来判断最后一个 token
 ? ////error 的处理逻辑
 TODO ////字符常量 \ddd \xhh 支持
 ? 为什么 'abc' 是字符常量
*/

#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>

using namespace std;

const int ERROR = 100;
const int BUFFER_SIZE = 4096;
const int HALF_SIZE = BUFFER_SIZE / 2;
const unordered_set<string> keywords = { "auto", "switch", "union", "void", "double", "case", "const", "default", "int", "enum", "float", "goto", "struct", "register", "short", "sizeof", "break", "typedef", "unsigned", "volatile", "else", "char", "continue", "do", "static", "extern", "for", "if", "long", "return", "signed", "while" };
const unordered_set<char> escape_chars = { 'o', 'n', 'r', 't', 'v', 'a', 'b', 'f', '\'', '"', '\\', '?', '0', '1', 'x'};
unordered_set<string> indentifiers = {};

ifstream file;
int state;
char C;
string token;
char buffer[BUFFER_SIZE];
int forwardptr;
int lexemebegin;
int iskey;
int line_cnt = 1;
int keywords_cnt;
int identifiers_cnt;
int operators_cnt;
int delimiters_cnt;
int charcons_cnt;
int strings_cnt;
int numbers_cnt;
int errors_cnt;

void add_eof(int base)
{
    int bytes_read = file.gcount();
    if (bytes_read < HALF_SIZE - 1) {
        buffer[base + bytes_read] = '\0';
    }
}

void get_char()
{
    if (buffer[forwardptr] == '\0') {
        if (forwardptr == HALF_SIZE - 1) {
            file.read(buffer + HALF_SIZE, HALF_SIZE - 1);
            add_eof(HALF_SIZE);
            ++forwardptr;
        } else if (forwardptr == BUFFER_SIZE - 1) {
            file.read(buffer, HALF_SIZE - 1);
            add_eof(0);
            forwardptr = 0;
        }

        if (buffer[forwardptr] == '\0') {
            file.close();
            cout << line_cnt << '\n';
            cout << keywords_cnt << ' ' << identifiers_cnt << ' ' << operators_cnt << ' ' << delimiters_cnt << ' ' << charcons_cnt << ' ' << strings_cnt << ' ' << numbers_cnt << '\n';
            cout << errors_cnt;
            exit(0);
        }
    }
    C = buffer[forwardptr];
    ++forwardptr;
}

void cat()
{
    token += C;
}

void get_nbc()
{
    while (C == ' ') {
        get_char();
    }
}

bool letter()
{
    return static_cast<bool>(isalpha(C));
}

bool digit()
{
    return static_cast<bool>(isdigit(C));
}

void retract()
{
    --forwardptr;
}

bool reserve()
{
    return keywords.find(token) != keywords.end();
}

void table_insert()
{
    indentifiers.insert(token);
}

void error()
{
    ++errors_cnt;
    cout << line_cnt << " <ERROR," << token << '>' << '\n';
}

int main(int argc, char* argv[])
{

    string file_name = argv[1]; // 该字符串变量为识别的C语言程序文件名

    /*程序每次运行只需要考虑识别一个C语言程序;
      需要使用读取文件的操作,可以在自己的本地环境里创建样例文件进行测试；
      在自己的IDE里测试时可以将file_name赋其他的值，
      但注意，最终测评时由平台提供的main参数来传入文件名*/

    ofstream append_file(file_name, ios::app);
    append_file << ' ';
    append_file.close();

    file.open(file_name, ios::binary);

    buffer[HALF_SIZE - 1] = '\0';
    buffer[BUFFER_SIZE - 1] = '\0';

    file.read(buffer, HALF_SIZE - 1);
    add_eof(0);

    while (true) {

        switch (state) {
        case 0:
            token = "";
            get_char();
            get_nbc();
            switch (C) {
            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
            case 'f':
            case 'g':
            case 'h':
            case 'i':
            case 'j':
            case 'k':
            case 'l':
            case 'm':
            case 'n':
            case 'o':
            case 'p':
            case 'q':
            case 'r':
            case 's':
            case 't':
            case 'v':
            case 'w':
            case 'x':
            case 'y':
            case 'z':
            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F':
            case 'G':
            case 'H':
            case 'I':
            case 'J':
            case 'K':
            case 'M':
            case 'N':
            case 'O':
            case 'P':
            case 'Q':
            case 'R':
            case 'S':
            case 'T':
            case 'V':
            case 'W':
            case 'X':
            case 'Y':
            case 'Z':
            case '_':
                state = 1;
                break;
            case 'u':
                state = 31;
                break;
            case 'U':
            case 'L':
                state = 33;
                break;
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                state = 2;
                break;
            case '<':
                state = 8;
                break;
            case '>':
                state = 10;
                break;
            case '=':
                state = 12;
                break;
            case '!':
                state = 13;
                break;
            case '&':
                state = 14;
                break;
            case '|':
                state = 15;
                break;
            case '+':
                state = 16;
                break;
            case '-':
                state = 17;
                break;
            case '*':
                state = 18;
                break;
            case '/':
                state = 19;
                break;
            case '%':
                state = 20;
                break;
            case '^':
                state = 21;
                break;
            case '~':
                state = 0;
                ++operators_cnt;
                cout << line_cnt << " <OPERATOR,~>" << '\n';
                break;
            case '.':
                state = 3;
                break;
            case ';':
            case ',':
            case ':':
            case '?':
            case '[':
            case ']':
            case '(':
            case ')':
            case '{':
            case '}':
                state = 0;
                ++delimiters_cnt;
                cout << line_cnt << " <DELIMITER," << C << ">" << '\n';
                break;
            case '\n':
                state = 0;
                ++line_cnt;
                break;
            case '\'':
                state = 22;
                break;
            case '\"':
                state = 24;
                break;
            case '@':
                cat();
                state = ERROR;
                break;
            }
            break;
        case 1:
            cat();
            get_char();
            if (letter() || digit() || C == '_') {
                state = 1;
            } else {
                retract();
                state = 0;
                if (reserve()) {
                    ++keywords_cnt;
                    cout << line_cnt << " <KEYWORD," << token << ">" << '\n';
                } else {
                    table_insert();
                    ++identifiers_cnt;
                    cout << line_cnt << " <IDENTIFIER," << token << ">" << '\n';
                }
            }
            break;
        case 2:
            cat();
            get_char();
            switch (C) {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                state = 2;
                break;
            case '.':
                state = 3;
                break;
            case 'e':
            case 'E':
                state = 5;
                break;
            case 'x':
            case 'X':
                state = 29;
                break;
            case 'u':
            case 'U':
            case 'l':
            case 'L':
                state = 30;
                break;
            default:
                if (letter()) {
                    cat();
                    state = ERROR;
                    break;
                }
                retract();
                state = 0;
                ++numbers_cnt;
                cout << line_cnt << " <NUMBER," << token << ">" << '\n';
                break;
            }
            break;
        case 3:
            cat();
            get_char();
            if (digit()) {
                state = 4;
            } else {
                retract();
                state = 0;
                ++operators_cnt;
                cout << line_cnt << " <OPERATOR,.>" << '\n';
            }
            break;
        case 4:
            cat();
            get_char();
            if (digit()) {
                state = 4;
            } else if (C == 'e' || C == 'E') {
                state = 5;
            } else if (C == 'f' || C == 'F' || C == 'l' || C == 'L') {
                state = 0;
                cat();
                ++numbers_cnt;
                cout << line_cnt << " <NUMBER," << token << ">" << '\n';
            } else {
                retract();
                state = 0;
                ++numbers_cnt;
                cout << line_cnt << " <NUMBER," << token << ">" << '\n';
            }
            break;
        case 5:
            cat();
            get_char();
            if (C == '+' || C == '-') {
                state = 6;
            } else if (digit()) {
                state = 7;
            }  else {
                retract();
                state = ERROR;
            }
            break;
        case 6:
            cat();
            get_char();
            if (digit()) {
                state = 7;
            } else {
                state = ERROR;
            }
            break;
        case 7:
            cat();
            get_char();
            if (digit()) {
                state = 7;
            } else if (C == 'f' || C == 'F' || C == 'l' || C == 'L') {
                state = 0;
                cat();
                ++numbers_cnt;
                cout << line_cnt << " <NUMBER," << token << ">" << '\n';
            } else {
                retract();
                state = 0;
                ++numbers_cnt;
                cout << line_cnt << " <NUMBER," << token << ">" << '\n';
            }
            break;
        case 8:
            cat();
            get_char();
            switch (C) {
            case '=':
                state = 0;
                ++operators_cnt;
                cout << line_cnt << " <OPERATOR,<=>" << '\n';
                break;
            case '<':
                state = 9;
                break;
            default:
                retract();
                state = 0;
                ++operators_cnt;
                cout << line_cnt << " <OPERATOR,<>" << '\n';
            }
            break;
        case 9:
            cat();
            get_char();
            switch (C) {
            case '=':
                state = 0;
                ++operators_cnt;
                cout << line_cnt << " <OPERATOR,<<=>" << '\n';
                break;
            default:
                retract();
                state = 0;
                ++operators_cnt;
                cout << line_cnt << " <OPERATOR,<<>" << '\n';
            }
            break;
        case 10:
            cat();
            get_char();
            switch (C) {
            case '=':
                state = 0;
                ++operators_cnt;
                cout << line_cnt << " <OPERATOR,>=>" << '\n';
                break;
            case '>':
                state = 11;
                break;
            default:
                retract();
                state = 0;
                ++operators_cnt;
                cout << line_cnt << " <OPERATOR,>>" << '\n';
            }
            break;
        case 11:
            cat();
            get_char();
            switch (C) {
            case '=':
                state = 0;
                ++operators_cnt;
                cout << line_cnt << " <OPERATOR,>>=>" << '\n';
                break;
            default:
                retract();
                state = 0;
                ++operators_cnt;
                cout << line_cnt << " <OPERATOR,>>>" << '\n';
            }
            break;
        case 12:
            cat();
            get_char();
            switch (C) {
            case '=':
                state = 0;
                ++operators_cnt;
                cout << line_cnt << " <OPERATOR,==>" << '\n';
                break;
            default:
                retract();
                state = 0;
                ++operators_cnt;
                cout << line_cnt << " <OPERATOR,=>" << '\n';
            }
            break;
        case 13:
            cat();
            get_char();
            switch (C) {
            case '=':
                state = 0;
                ++operators_cnt;
                cout << line_cnt << " <OPERATOR,!=>" << '\n';
                break;
            default:
                retract();
                state = 0;
                ++operators_cnt;
                cout << line_cnt << " <OPERATOR,!>" << '\n';
            }
            break;
        case 14:
            cat();
            get_char();
            switch (C) {
            case '&':
                state = 0;
                ++operators_cnt;
                cout << line_cnt << " <OPERATOR,&&>" << '\n';
                break;
            case '=':
                state = 0;
                ++operators_cnt;
                cout << line_cnt << " <OPERATOR,&=>" << '\n';
                break;
            default:
                retract();
                state = 0;
                ++operators_cnt;
                cout << line_cnt << " <OPERATOR,&>" << '\n';
            }
            break;
        case 15:
            cat();
            get_char();
            switch (C) {
            case '|':
                state = 0;
                ++operators_cnt;
                cout << line_cnt << " <OPERATOR,||>" << '\n';
                break;
            case '=':
                state = 0;
                ++operators_cnt;
                cout << line_cnt << " <OPERATOR,|=>" << '\n';
                break;
            default:
                retract();
                state = 0;
                ++operators_cnt;
                cout << line_cnt << " <OPERATOR,|>" << '\n';
            }
            break;
        case 16:
            cat();
            get_char();
            switch (C) {
            case '+':
                state = 0;
                ++operators_cnt;
                cout << line_cnt << " <OPERATOR,++>" << '\n';
                break;
            case '=':
                state = 0;
                ++operators_cnt;
                cout << line_cnt << " <OPERATOR,+=>" << '\n';
                break;
            default:
                retract();
                state = 0;
                ++operators_cnt;
                cout << line_cnt << " <OPERATOR,+>" << '\n';
            }
            break;
        case 17:
            cat();
            get_char();
            switch (C) {
            case '-':
                state = 0;
                ++operators_cnt;
                cout << line_cnt << " <OPERATOR,-->" << '\n';
                break;
            case '=':
                state = 0;
                ++operators_cnt;
                cout << line_cnt << " <OPERATOR,-=>" << '\n';
                break;
            case '>':
                state = 0;
                ++operators_cnt;
                cout << line_cnt << " <OPERATOR,->>" << '\n';
                break;
            default:
                retract();
                state = 0;
                ++operators_cnt;
                cout << line_cnt << " <OPERATOR,->" << '\n';
            }
            break;
        case 18:
            cat();
            get_char();
            if (C == '=') {
                state = 0;
                ++operators_cnt;
                cout << line_cnt << " <OPERATOR,*=>" << '\n';
            } else {
                retract();
                state = 0;
                ++operators_cnt;
                cout << line_cnt << " <OPERATOR,*>" << '\n';
            }
            break;
        case 19:
            cat();
            get_char();
            switch (C) {
            case '=':
                state = 0;
                ++operators_cnt;
                cout << line_cnt << " <OPERATOR,/=>" << '\n';
                break;
            case '/':
                state = 26;
                break;
            case '*':
                state = 27;
                break;
            default:
                retract();
                state = 0;
                ++operators_cnt;
                cout << line_cnt << " <OPERATOR,/>" << '\n';
                break;
            }
            break;
        case 20:
            cat();
            get_char();
            if (C == '=') {
                state = 0;
                ++operators_cnt;
                cout << line_cnt << " <OPERATOR,%=>" << '\n';
            } else {
                retract();
                state = 0;
                ++operators_cnt;
                cout << line_cnt << " <OPERATOR,%>" << '\n';
            }
            break;
        case 21:
            cat();
            get_char();
            if (C == '=') {
                state = 0;
                ++operators_cnt;
                cout << line_cnt << " <OPERATOR,^=>" << '\n';
            } else {
                retract();
                state = 0;
                ++operators_cnt;
                cout << line_cnt << " <OPERATOR,^>" << '\n';
            }
            break;
        case 22:
            cat();
            get_char();
            switch (C) {
            case '\'':
                state = 0;
                ++charcons_cnt;
                cout << line_cnt << " <CHARCON," << token << "\'>" << '\n';
                break;
            case '\\':
                state = 23;
                break;
            case '\n':
                retract();
                state = ERROR;
                break;
            default:
                state = 22;
                break;
            }
            break;
        case 23:
            cat();
            get_char();
            if (escape_chars.find(C) != escape_chars.end()) {
                state = 22;
            } else {
                state = ERROR;
            }
            break;
        case 24:
            cat();
            get_char();
            switch (C) {
            case '\"':
                state = 0;
                ++strings_cnt;
                cout << line_cnt << " <STRING," << token << "\">" << '\n';
                break;
            case '\\':
                state = 25;
                break;
            case '\n':
                retract();
                state = ERROR;
                break;
            default:
                state = 24;
                break;
            }
            break;
        case 25:
            cat();
            get_char();
            if (escape_chars.find(C) != escape_chars.end()) {
                state = 24;
            } else {
                state = ERROR;
            }
            break;
        case 26:
            get_char();
            if (C == '\n') {
                state = 0;
                ++line_cnt;
            } else {
                state = 26;
            }
            break;
        case 27:
            get_char();
            switch (C) {
            case '*':
                state = 28;
                break;
            case '\n':
                ++line_cnt;
            default:
                state = 27;
                break;
            }
            break;
        case 28:
            get_char();
            switch (C) {
            case '/':
                state = 0;
                break;
            case '\n':
                ++line_cnt;
            default:
                state = 27;
                break;
            }
            break;
        case 29:
            cat();
            get_char();
            if (digit() || (C >= 'a' && C <= 'f') || (C >= 'A' && C <= 'F')) {
                state = 29;
            } else {
                retract();
                state = 0;
                ++numbers_cnt;
                cout << line_cnt << " <NUMBER," << token << ">" << '\n';
            }
            break;
        case 30:
            cat();
            get_char();
            if (C == 'u' || C == 'U' || C == 'l' || C == 'L') {
                state = 30;
            } else {
                retract();
                state = 0;
                ++numbers_cnt;
                cout << line_cnt << " <NUMBER," << token << ">" << '\n';
            }
            break;
        case 31:
            cat();
            get_char();
            if (C == '\"') {
                state = 24;
            } else if (C == '8') {
                state = 32;
            } else if (C == '\'') {
                state = 22;
            } else {
                state = 1;
            }
            break;
        case 32:
            cat();
            get_char();
            if (C == '\"') {
                state = 24;
            } else {
                state = 1;
            }
            break;
        case 33:
            cat();
            get_char();
            if (C == '\"') {
                state = 24;
            } else if (C == '\'') {
                state = 22;
            } else {
                state = 1;
            }
            break;
        case ERROR:
            error();
            state = 0;
            break;
        }
    }

    return 0;
}