/* Simple ad-hoc scanner for the calculator language.
   Dies on lexical errors.
   Michael L. Scott, 2008-2022.
*/

#include "scan.hpp"
#include <cctype>
#include <iostream>
#include <tuple>
#include <string>
using std::cerr;
using std::cin;
using std::endl;
using std::hex;
using std::make_tuple;
using std::string;

tuple<token, string> scanner::scan() {
    string token_image;
    while (isspace(c)) {
        c = cin.get();
    }
    if (c == EOF)
        return make_tuple(t_eof, "$$");
    if (isalpha(c)) {
        do {
            token_image += c;
            c = cin.get();
        } while (isalpha(c) || isdigit(c) || c == '_');
        if (token_image == "read")
            return make_tuple(t_read, "read");
        else if (token_image == "write")
            return make_tuple(t_write, "write");
        else if (token_image == "int")
            return make_tuple(t_int, "int");
        else if (token_image == "real")
            return make_tuple(t_real, "real");
        else if (token_image == "if")
            return make_tuple(t_if, "if");
        else if (token_image == "then")
            return make_tuple(t_then, "then");
        else if (token_image == "while")
            return make_tuple(t_while, "while");
        else if (token_image == "do")
            return make_tuple(t_do, "do");
        else if (token_image == "trunc")
            return make_tuple(t_trunc, "trunc");
        else if (token_image == "end")
            return make_tuple(t_end, "end");
        else if (token_image == "float")
            return make_tuple(t_float, "float");
        else
            return make_tuple(t_id, token_image);
    } else if (c == '.') {
        do {
            token_image += c;
            c = cin.get();
        } while (isdigit(c));

        if (c == 'e') {
            token_image += c;
            c = cin.get();
            if (c == '+' || c == '-') {
                token_image += c;
                c = cin.get();
            }
            if (!isdigit(c)) {
                CalculatorErrorDetect = true;
                cerr << "Lexical ERROR: expected digit after 'e' or 'e+' or 'e-', got '" << char(c) << "' (0x" << hex << c << ")\n";
                return scan();
            }
            do {
                token_image += c;
                c = cin.get();
            } while (isdigit(c));
            return make_tuple(t_rnum, token_image);
        } else if (c == ';' || c == ' ') {
            return make_tuple(t_rnum, token_image);
        } else {
            CalculatorErrorDetect = true;
            cerr << "Lexical ERROR: expected digit after '.', got '" << char(c) << "' (0x" << hex << c << ")\n";
            return scan();
        }
    }

    else if (isdigit(c)) {
        do {
            token_image += c;
            c = cin.get();
        } while (isdigit(c));
        if (c == '.') {
            do {
                token_image += c;
                c = cin.get();
            } while (isdigit(c));

            if (c == 'e') {
                token_image += c;
                c = cin.get();
                if (c == '+' || c == '-') {
                    token_image += c;
                    c = cin.get();
                }
                if (!isdigit(c)) {
                    CalculatorErrorDetect = true;
                    cerr << "Lexical ERROR: expected digit after 'e' or 'e+' or 'e-', got '" << char(c) << "' (0x" << hex << c << ")\n";
                    return scan();
                }
                do {
                    token_image += c;
                    c = cin.get();
                } while (isdigit(c));
                return make_tuple(t_rnum, token_image);
            } else if (c == ';' || c == ' ') {
                return make_tuple(t_rnum, token_image);
            } else {
                CalculatorErrorDetect = true;
                cerr << "Lexical ERROR: expected digit after '.', got '" << char(c) << "' (0x" << hex << c << ")\n";
                return scan();
            }
        } else {
            return make_tuple(t_inum, token_image);
        }

    }

    else
        switch (c) {
        case ':':
            c = cin.get();
            if (c != '=') {
                CalculatorErrorDetect = true;
                cerr << "Lexical ERROR: expected '=' after ':', got '" << c << "' (0x" << hex << c << ")\n";
                return scan();
            }
            c = cin.get();
            return make_tuple(t_gets, ":=");
            break;
        case '=':
            c = cin.get();
            if (c != '=') {
                CalculatorErrorDetect = true;
                cerr << "Lexical ERROR: expected '=' after '=', got '" << c << "' (0x" << hex << c << ")\n";
                return scan();
            }
            c = cin.get();
            return make_tuple(t_equal, "==");
            break;
        case '>':
            c = cin.get();
            switch (c) {
            case '=':
                c = cin.get();
                return make_tuple(t_largerEqual, ">=");
            case ' ':
                c = cin.get();
                return make_tuple(t_larger, ">");
            default:
                c = cin.get();
                CalculatorErrorDetect = true;
                cerr << "Lexical ERROR: expected '=' after '>', got '" << c << "' (0x" << hex << c << ")\n";
                return scan();
            }

        case '<':
            c = cin.get();
            switch (c) {
            case '=':
                c = cin.get();
                return make_tuple(t_smallerEqual, "<=");
            case '>':
                c = cin.get();
                return make_tuple(t_notEqual, "<>");
            case ' ':
                c = cin.get();
                return make_tuple(t_smaller, "<");
            default:
                c = cin.get();
                CalculatorErrorDetect = true;
                cerr << "Lexical ERROR: expected '=' or '>' after '<', got '" << c << "' (0x" << hex << c << ")\n";
                return scan();
            }
            break;

        case '+':
            c = cin.get();
            return make_tuple(t_add, "+");
        case '-':
            c = cin.get();
            return make_tuple(t_sub, "-");
        case '*':
            c = cin.get();
            return make_tuple(t_mul, "*");
        case '/':
            c = cin.get();
            return make_tuple(t_div, "/");
        case '(':
            c = cin.get();
            return make_tuple(t_lparen, "(");
        case ')':
            c = cin.get();
            return make_tuple(t_rparen, ")");
        case ';':
            c = cin.get();
            return make_tuple(t_semicolon, ";");
        default:
            CalculatorErrorDetect = true;
            cerr << "Lexical ERROR: unexpected character '" << char(c) << "' (0x" << hex << c << ")\n";
            c = cin.get();
            return scan();
        }
    exit(1);
}
