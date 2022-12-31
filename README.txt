Name: Yao Liu
NetID: yliu204
Partner: Yibo (Mike) He

Files
1. scan.hpp
2. scan.cpp
3. parse.cpp

How to run:
1. go to the directory of this project file;
2. make -f Makefile ("make" also works);
3. ./parse < "fileName.txt" (or you could just type ./parse, then enter then string you want to test. We also have a log argument, which helps to give all predictions.); As required in this project, the program will print all errors if they encounter error; else when there's no error, it will print AST. If you want to see production predictions and ast both, you could type ./parse log ast.

Project Idea:

The initial source code does not includes all terminals we need, so we just added them into scan.hpp and scan.cpp files.
1. Lexical Errors:
With full terminal tokens, we divided the scan process into 4 different conditions. For condition 1, it catchs the end of file signal and make the t_eof tuple;
in condition 2, the program accepts the characters and consists of a string that start with a letter and followed by letters, digits, and "_". The third condition accepts
the digits and ".". We checked some lexical errors here to handle most exceptions, such as more than one digital points. In the last condition, we made distinguishment between
operators and checked some lexical errors.
If the whole process, if the constructing string matches the condition, we made tuple to store the token and the corresponding string.
Else, if not match, just print the error message or fall into default condition, which is also error. 

2. Extend the calculator language
This process is mainly in the parse.cpp file. We implemented all terminals as tokens and array(names[]). After calculating the FIRST set and FOLLOW set of all non-terminal tokens,
we stored them in the set data structure for further checking. 
The parse class involved three global variables, one string and two variables to represent the current token. It also contains the error-check, error-report, and match functions, as well as the parser() function to get next token from scanner. 
For each non-terminal, there's a function that return the string (this string is for task 4, building AST). Under each function, we added all terminals in the FIRST set to the switch cases. 
If the current token meets with one of the first case, it indicates the following terminals and non-terminals will substitute the current non-terminal token. This process happens from program, until all things are terminal.
It is a recursive way to reach the end from the top. If it is a terminal in the grammar, we just call match function to see if the current token match with the expectation terminal. If it is a non-terminal, 
we just call the function of that non-terminal and move deeper. Finally, we would reach the end and match all terminals (if there's no error).

3. Error recovery
Based on the Wirth's error recovery algorithm (as well as the psueduo code in the note), the general idea of error recovery if report the error message first when there's no match in terminal, 
or current token is not in the FIRST set of the current non-terminal (it means we could not use current token and its following tokens to substitute the current non-token), or the current non-terminal cannot be epsilon 
(in the epsilon case it is not an error because the current token might be the first token of the next non-terminal, and current non-terminal is an epsilon). After report the error message, the parse does not stop and 
it could generate more error at one time. It will keep getting the new token from scanner, until the current token is in the First or Follow set of current non-terminal, or reach the end of the file.
In real case, the algorithm should have different operations for current is in the First (all tokens before are extra tokens and should be deleted) set or Follow set (all tokens before are the mis-type of current non-terminal and should insert expected tokens).
However, in this project, for the simplicity, we did not make exception insertion and just delete the error tokens (not real delete, just keep moving).
We also added FOLLOW set in the switch case becasue we need to handle the condition that tokens stops scanning and is in the FOLLOW set.
We also considered about four epsilon cases, because if the non-terminal can be epsilon, it always passes the error-check function.
Overall, we could get all error message with this implementation.

4. AST
We just built AST when we parse the program from the top to the end. The program starts from the top, and call the next non-terminals to substitute the most-top non-tokens. Finally we reach the bottom and get terminals.
The terminal will return back its representation in AST as string, and upper function which calls this terminal will get the string, and concadanate it with other same level returned string.
Finally, it will return to the top program. If we get the return value of the program(), we just have the whole tree. 
