### parsing BNF Grammar

<MainContext> = "http" "{" {<http-directives> | <serverContext>} "}"

<serverContext> = "server" "[" <WORD: server_name> "]" "{" {<server-directives> | <locationContext>} "}"

<locationContext> = "location" "[" <WORD: route> "]" "{" {<loaction-directives> | <locationContext>} "}"

<http-directives> = {<http-directive> ";"}+

<server-directives> = {<server-directive> ";"}+

<location-directives> = {<location-directive> ";" }+

<http-directive> = { 
    "root" <WORD> | "allow" {("GET" | "POST" | "DELETE")}+ | "client_max_body_size" <SIZE> 
    | "autoindex" ("on" | "off")
    | "index" <WORD>+
    | "error_page" <CODE> <WORD> 
}

<server-directive> = {
    { <http-directive> | "listen" <WORD>  | "return" <INT> [<WORD>] } | <serverContext>
}

<location-directive> = {
    { <http-directive> | "return" <INT> [<WORD>] | "cgi_assign" {<WORD>} } | <locationContext>
}