/* Definitions the scanner shares with the parser.
   Michael L. Scott, 2008-2022.
*/

#include <tuple>
#include <string>
using std::string;
using std::tuple;

enum token { t_read,
             t_write,
             t_id,
             t_gets,
             t_add,
             t_sub,
             t_mul,
             t_div,
             t_lparen,
             t_rparen,
             t_eof,
             t_semicolon,
             t_int,
             t_assign,
             t_real,
             t_if,
             t_then,
             t_end,
             t_while,
             t_do,
             t_inum,
             t_rnum,
             t_trunc,
             t_float,
             t_equal,
             t_notEqual,
             t_larger,
             t_smaller,
             t_largerEqual,
             t_smallerEqual };

const int MAX_TOKEN_LEN = 256;
extern char token_image[MAX_TOKEN_LEN];

class scanner {
    int c = ' ';

public:
    tuple<token, string> scan();
};

extern bool CalculatorErrorDetect;
