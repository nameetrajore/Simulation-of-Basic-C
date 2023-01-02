# Grammar for 'basic c':
\<program> ->\<statements> | \<declare> \<statements> ";"
\<statements> ->\<statement> ";" \<statements> | epsilon
## Grammar for Statements:
\<statement> ->\<read> | \<write> | \<assignment> |<for-loop>
## Grammar for declaration:
\<declare> ->  "int"\<declaration>
\<declaration> ->\<variable> "," \<declaration> | \<variable>
## Grammar for read:
\<read> -> "read"\<variable>
## Grammar for write:
\<write> -> "write"\<variable> | "write" \<integer>
## Grammar for assignment:
\<assignment> ->\<variable> "=" \<expression>
## Grammar for Expressions:
\<expression> ->\<equality>
\<equality> ->\<comparison> | \<equality> "==" \<comparison>
\<comparison> ->\<term> | \<comparison> ">" \<term>
\<term> ->\<factor> | \<term> "-" \<factor> | \<term> "+" \<factor>
\<factor> ->\<primary> | \<factor> "/" \<primary> | \<factor> "*" \<primary>
\<primary> ->\<number> | \<variable> | "(" \<expression> ")"
## Grammar for variables:
\<variable> ->\<alphabet><variable> | \<alphabet>
## Grammar for for-loops:
\<for-loop> -> "for" "("\<assignment> ; \<expression> ; \<assignment> ")" "{" \<statements> "}" ";"
## Grammar for integers:
\<integer> -> 0 |\<digits>
\<positive digit> -> 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9
\<digits> ->\<positive digit> \<digit>
\<digit> ->\<digit> \<separate> | epsilon
\<separate> -> 0 |\<positive digit>
## Grammar for alphabets:
\<alphabet> -> a | b | c | d | e | f | g | h | i | j | k | l | m | n | o | p | q | r | s | t | u | v | w | x | y | z
