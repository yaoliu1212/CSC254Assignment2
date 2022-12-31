/* Complete recursive descent parser for the calculator language.
   Builds on figure 2.16 in the text.  Prints a trace of productions
   predicted and tokens matched.  Does no error recovery: prints
   "syntax error" and dies on invalid input.
   Michael L. Scott, 2008-2022.
*/

#include <iostream>
#include <set>
#include <string.h>
#include <tuple>
#include <string>
using std::cerr;
using std::cout;
using std::endl;
using std::string;
using std::tie;

#include "scan.hpp"
bool CalculatorErrorDetect = false;
bool LogGrammerExpansion = false;

// all possible values for token next_token in class parser
const char *names[] = {"read", "write", "id", "gets", "add",
                       "sub", "mul", "div", "lparen", "rparen", "eof",
                       "semicolon", "int", "assign", "real", "if", "then", "end", "while",
                       "do", "inum", "rnum", "trunc", "float",
                       "equal", "notEqual", "larger", "smaller", "largerEqual", "smallerEqual"};
// First
std::set<token> program_First = {t_int, t_real, t_id, t_read, t_write, t_if, t_while};
std::set<token> stmt_list_First = {t_int, t_real, t_id, t_read, t_write, t_if, t_while};
std::set<token> stmt_First = {t_int, t_real, t_id, t_read, t_write, t_if, t_while};
std::set<token> tp_First = {t_int, t_real};
std::set<token> condi_First = {t_lparen, t_id, t_inum, t_rnum, t_trunc, t_float};
std::set<token> expr_First = {t_lparen, t_id, t_inum, t_rnum, t_trunc, t_float};
std::set<token> term_First = {t_lparen, t_id, t_inum, t_rnum, t_trunc, t_float};
std::set<token> factor_First = {t_lparen, t_id, t_inum, t_rnum, t_trunc, t_float};
std::set<token> term_tail_First = {t_add, t_sub};
std::set<token> add_op_First = {t_add, t_sub};
std::set<token> factor_tail_First = {t_mul, t_div};
std::set<token> mul_op_First = {t_mul, t_div};
std::set<token> ro_First = {t_equal, t_notEqual, t_larger, t_smaller, t_smallerEqual, t_largerEqual};

// Follow
std::set<token> sprogram_Follow = {};
std::set<token> stmt_list_Follow = {t_eof, t_end};
std::set<token> stmt_Follow = {t_semicolon};
std::set<token> tp_Follow = {t_id};
std::set<token> condi_Follow = {t_then, t_do};
std::set<token> expr_Follow = {t_rparen, t_semicolon, t_equal, t_notEqual, t_smaller, t_larger, t_smallerEqual, t_largerEqual, t_then, t_do};
std::set<token> term_tail_Follow = {t_rparen, t_semicolon, t_equal, t_notEqual, t_smaller, t_larger, t_smallerEqual, t_largerEqual, t_then, t_do};
std::set<token> term_Follow = {t_rparen, t_semicolon, t_equal, t_notEqual, t_smaller, t_larger, t_smallerEqual, t_largerEqual, t_then, t_do, t_add, t_sub};
std::set<token> factor_tail_Follow = {t_rparen, t_semicolon, t_equal, t_notEqual, t_smaller, t_larger, t_smallerEqual, t_largerEqual, t_then, t_do, t_add, t_sub};
std::set<token> factor_Follow = {t_rparen, t_semicolon, t_equal, t_notEqual, t_smaller, t_larger, t_smallerEqual, t_largerEqual, t_then, t_do, t_add, t_sub, t_mul, t_div};
std::set<token> ro_Follow = {t_lparen, t_id, t_inum, t_rnum, t_trunc, t_float};
std::set<token> add_op_Follow = {t_rparen, t_id, t_inum, t_rnum, t_trunc, t_float};
std::set<token> mul_op_Follow = {t_rparen, t_id, t_inum, t_rnum, t_trunc, t_float};

// parser object
class parser {
    token next_token;
    string token_image;
    scanner s;

    void error(string current_token) {
        CalculatorErrorDetect = true;
        cerr << "Syntax ERROR in token mismatch " << current_token << endl;
        tie(next_token, token_image) = s.scan();
    }

    void error_null_prediction(token input_token, std::set<token> firstSet, std::set<token> followSet, string production) {
        CalculatorErrorDetect = true;
        cerr << "Synatx ERROR in token " << names[input_token] << " at production: " << production << endl;
        do {
            tie(next_token, token_image) = s.scan();
        } while (firstSet.count(next_token) == 0 && followSet.count(next_token) == 0 && next_token != t_eof);
    }

    void match(token expected) {
        if (next_token == expected) {
            if (LogGrammerExpansion) {
                if (next_token == t_id || next_token == t_inum || next_token == t_rnum) {
                    cout << ": " << token_image;
                }
                cout << endl;
            }
            tie(next_token, token_image) = s.scan();
        } else {

            error(token_image);
        }
    }

public:
    parser() {
        tie(next_token, token_image) = s.scan();
    }
    string program() {
        string local = "[";
        switch (next_token) {
        case t_int:
        case t_real:
        case t_id:
        case t_read:
        case t_write:
        case t_if:
        case t_while:
            if (LogGrammerExpansion)
                cout << "predict program --> stmt_list eof" << endl;
            local += stmt_list();
            match(t_eof);
            break;
        default:
            if (LogGrammerExpansion)
                cout << "predict program --> eof" << endl;
        }
        local += "]";
        return local;
    }

private:
    string stmt_list() {
        string local = "";
        switch (next_token) {
        case t_int:
        case t_real:
        case t_id:
        case t_read:
        case t_write:
        case t_if:
        case t_while:
            if (LogGrammerExpansion)
                cout << "predict stmt_list --> stmt ; stmt_list" << endl;
            local += stmt();
            match(t_semicolon);
            local += stmt_list();
            break;
        default:
            if (LogGrammerExpansion)
                cout << "predict stmt_list --> epsilon" << endl;
            return "";
        }
        return local;
    }
    string stmt() {
        string local = "";
        string id = "";
        string temp = "";
        switch (next_token) {
        case t_id:
            if (LogGrammerExpansion)
                cout << "predict stmt --> id := expr" << endl;
            id = token_image;
            match(t_id);
            local += "(:= ";
            match(t_gets);
            local += "\"";
            local += id;
            local += "\" ";
            local += expr();
            local += ")";
            break;
        case t_read:
            if (LogGrammerExpansion)
                cout << "predict stmt --> read tp id" << endl;
            match(t_read);
            local = "(" + tp();
            local += ")";
            local += "(read ";
            local += "\"";
            local += token_image;
            local += "\")";
            match(t_id);
            break;
        case t_write:
            if (LogGrammerExpansion)
                cout << "predict stmt --> write expr" << endl;
            match(t_write);
            local = "(write ";
            local += expr();
            local += ")";
            break;
        case t_int:
            if (LogGrammerExpansion)
                cout << "predict stmt --> int id := expr" << endl;
            match(t_int);
            local = "(int \"" + token_image + "\")";
            id = token_image;
            match(t_id);
            match(t_gets);
            local += "(:= \"" + id + "\" ";
            local += expr() + ")";
            break;
        case t_real:
            if (LogGrammerExpansion)
                cout << "predict stmt --> real id := expr" << endl;
            match(t_real);
            local = "(real \"";
            id = token_image;
            match(t_id);
            local += id + "\"";
            local += ")(:= \"";
            local += id;
            local += "\" ";
            match(t_gets);
            local += expr() + ")";
            break;
        case t_if:
            if (LogGrammerExpansion)
                cout << "predict stmt --> if condi then sl end" << endl;
            local = "(if (";
            match(t_if);
            local += condi() + "[";
            match(t_then);
            local += stmt_list() + "])";
            match(t_end);
            break;
        case t_while:
            if (LogGrammerExpansion)
                cout << "predict stmt --> while condi do stmt_list end" << endl;
            local = "(while (";
            match(t_while);
            local += condi();
            local += "[";
            match(t_do);
            local += stmt_list() + "]";
            match(t_end);
            local += ")";
            break;
        default:
            error(token_image);
        }
        return local;
    }

    string tp() {
        string local = "";
        switch (next_token) {
        case t_int:
            if (LogGrammerExpansion)
                cout << "predict tp --> int" << endl;
            match(t_int);
            local = "int \"" + token_image + "\"";
            break;
        case t_real:
            if (LogGrammerExpansion)
                cout << "predict tp --> real" << endl;
            match(t_real);
            local = "real \"" + token_image + "\"";
            break;
        default:
            if (LogGrammerExpansion)
                cout << "predict tp --> epsilon" << endl;
            return "";
            break;
        }
        return local;
    }

    string condi() {
        string local = "";
        string temp = "";
        switch (next_token) {
        case t_lparen:
        case t_id:
        case t_inum:
        case t_rnum:
        case t_trunc:
        case t_float:
            if (LogGrammerExpansion)
                cout << "predict condi --> expr ro expr" << endl;
            temp = expr();
            local = ro();
            local += " " + temp + " ";
            local += expr() + ")";
            break;
        default:
            error_null_prediction(next_token, condi_First, condi_Follow, "condi");
        }
        return local;
    }

    string expr() {
        string tlocal = "";
        string local = "";
        switch (next_token) {
        case t_id:
        case t_inum:
        case t_rnum:
        case t_lparen:
        case t_trunc:
        case t_float:
            if (LogGrammerExpansion)
                cout << "predict expr --> term term_tail" << endl;
            tlocal = term();
            local = term_tail(tlocal);
            break;
        default:
            error_null_prediction(next_token, expr_First, expr_Follow, "expr");
        }
        return local;
    }

    string term_tail(string tpara) {
        string tlocal = "";
        string local = "";
        switch (next_token) {
        case t_add:
        case t_sub:
            if (LogGrammerExpansion)
                cout << "predict term_tail --> add_op term term_tail" << endl;
            local = "(" + add_op() + " " + tpara + " ";
            tlocal = term();
            local += term_tail(tlocal);
            local += ")";
            break;

        default:
            if (LogGrammerExpansion)
                cout << "predict term_tail --> epsilon" << endl;
            return tpara;
        }
        return local;
    }
    string term() {
        string flocal = "";
        string local = "";
        switch (next_token) {
        case t_id:
        case t_inum:
        case t_rnum:
        case t_lparen:
        case t_trunc:
        case t_float:
            if (LogGrammerExpansion)
                cout << "predict term --> factor factor_tail" << endl;
            flocal = factor();
            local += factor_tail(flocal);
            break;
        default:
            error_null_prediction(next_token, term_First, term_Follow, "term");
        }
        return local;
    }

    string factor_tail(string fpara) {
        string flocal = "";
        string local = "";
        switch (next_token) {
        case t_mul:
        case t_div:
            if (LogGrammerExpansion)
                cout << "predict factor_tail --> mul_op factor factor_tail" << endl;
            local = "(" + mul_op() + " " + fpara + " ";
            flocal = factor();
            local += factor_tail(flocal);
            local += ")";
            break;

        default:
            if (LogGrammerExpansion)
                cout << "predict factor_tail --> epsilon" << endl;
            return fpara;
        }
        return local;
    }
    string factor() {
        string local = "";
        switch (next_token) {
        case t_inum:
            if (LogGrammerExpansion)
                cout << "predict factor --> i_num" << endl;
            local = "\"";
            local += token_image;
            local += "\"";
            match(t_inum);
            break;
        case t_rnum:
            if (LogGrammerExpansion)
                cout << "predict factor --> r_num" << endl;
            local = "\"";
            local += token_image;
            local += "\"";
            match(t_rnum);
            break;
        case t_id:
            if (LogGrammerExpansion)
                cout << "predict factor --> id" << endl;
            local = "\"";
            local += token_image;
            local += "\"";
            match(t_id);
            break;
        case t_lparen:
            if (LogGrammerExpansion)
                cout << "predict factor --> lparen expr rparen" << endl;
            match(t_lparen);
            local = "(" + expr() + ")";
            match(t_rparen);
            break;
        case t_trunc:
            if (LogGrammerExpansion)
                cout << "predict factor --> trunc lparen expr rparen" << endl;
            local = "trunc (";
            match(t_trunc);
            match(t_lparen);
            local += expr();
            match(t_rparen);
            local += ")";
            break;
        case t_float:
            if (LogGrammerExpansion)
                cout << "predict factor --> float lparen expr rparen" << endl;
            local = "float (";
            match(t_float);
            match(t_lparen);
            local += expr();
            match(t_rparen);
            local += ")";
            break;
        default:
            error_null_prediction(next_token, factor_First, factor_Follow, "factor");
        }
        return local;
    }

    string add_op() {
        string local = "";
        switch (next_token) {
        case t_add:
            if (LogGrammerExpansion)
                cout << "predict add_op --> add" << endl;
            match(t_add);
            local = "+";
            break;
        case t_sub:
            if (LogGrammerExpansion)
                cout << "predict add_op --> sub" << endl;
            match(t_sub);
            local = "-";
            break;
        default:
            error_null_prediction(next_token, add_op_First, add_op_Follow, "add_op");
        }
        return local;
    }

    string mul_op() {
        string local = "";
        switch (next_token) {
        case t_mul:
            if (LogGrammerExpansion)
                cout << "predict mul_op --> mul" << endl;
            match(t_mul);
            local = "*";
            break;
        case t_div:
            if (LogGrammerExpansion)
                cout << "predict mul_op --> div" << endl;
            match(t_div);
            local = "/";
            break;
        default:
            error_null_prediction(next_token, mul_op_First, mul_op_Follow, "mul_op");
        }
        return local;
    }

    string ro() {
        string local = "";
        switch (next_token) {
        case t_equal:
            if (LogGrammerExpansion)
                cout << "predict ro --> equal" << endl;
            match(t_equal);
            local = "==";
            break;
        case t_notEqual:
            if (LogGrammerExpansion)
                cout << "predict ro --> notEqual" << endl;
            match(t_notEqual);
            local = "<>";
            break;
        case t_smaller:
            if (LogGrammerExpansion)
                cout << "predict ro --> smaller" << endl;
            match(t_smaller);
            local = "<";
            break;
        case t_larger:
            if (LogGrammerExpansion)
                cout << "predict ro --> larger" << endl;
            match(t_larger);
            local = ">";
            break;
        case t_smallerEqual:
            if (LogGrammerExpansion)
                cout << "predict ro --> smallerEqual" << endl;
            match(t_smallerEqual);
            local = "<=";
            break;
        case t_largerEqual:
            if (LogGrammerExpansion)
                cout << "predict ro --> largerEqual" << endl;
            match(t_largerEqual);
            local = ">=";
            break;
        default:
            error_null_prediction(next_token, ro_First, ro_Follow, "ro");
        }
        return local;
    }
};

int main(int argc, char **argv) {

    if (argc > 1) {
        if (!strcmp(argv[1], "log")) {
            LogGrammerExpansion = true;
        }
    }
    parser p;
    string resultPrint = p.program();
    cout << "\n>> Parsing Completed\n";

    if (argc > 1) {
        if (!strcmp(argv[1], "ast")) {
            CalculatorErrorDetect = false;
        }
    }

    if (argc > 2) {
        if (!strcmp(argv[2], "ast") && !strcmp(argv[1], "log")) {
            CalculatorErrorDetect = false;
            LogGrammerExpansion = true;
        }
    }

    if (CalculatorErrorDetect) {
        cout << ">> Error Detected, no AST printed\n\n";
    } else {

        cout << "----------------* Print AST *-------------------" << endl;
        cout << resultPrint << endl;
    }

    return 0;
}
