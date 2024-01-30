## Parsing BNF Grammar

HttpContext ::= "http" "{" {<httpDirectives>* | <ServerContext>} "}"

ServerContext ::= "server" <Key> "{" { <serverDirectives>* | <LocationContext> } "}"

LocationContext ::= "location" <Key> "{" { <locationDirectives>* | <LocationContext> } "}"

httpDirectives ::= <http-directive> 

serverDirectives ::= <server-directive>

locationDirectives ::= <location-directive>

http-directive ::= {
    <index> 
    | <autoindex> 
    | <error_page> 
    | <error_log>
    | <access_log>
    | <root>
    | <client_max_body_size>    
}

server-directive ::= {
    <http-directives>
    | <allow>
    | <listen>
}

location-directive ::= {
    <http-directives>
    | <redirect>
    | <allow>
    | <cgi_assign>
}

index ::= "index" { <Word> | <Word> } ";"

autoindex ::= "autoindex" {"off" | "on"} ";"

error_page ::= "error_page" {<number> | <number>} <Word> ";"

error_log ::= "error_log" <Word> ";"

access_log ::= "access_log" <Word> ";"

root ::= "root" <Word> ";"

client_max_body_size ::= "client_max_body_size" <number> ";"

listen ::= "listen" <number> ";"

redirect ::= "redirect" <number> <Word> ";"

allow ::= "allow_methods" { "GET" | "POST" | "DELETE" } ";"

cgi_assign ::= "cgi_assign" {<Word> | <Word>} ";"

Word ::= <character>*

character ::= "a" | "b" | "c" | ... | "z" | "A" | "B" | "C" | ... | "Z" | "0" | "1" | ... | "9"