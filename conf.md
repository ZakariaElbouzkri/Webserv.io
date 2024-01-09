
## Configuration file Context:
  - "http": It encloses settings that apply globally to the entire HTTP server.
  - "server": Defines settings specific to a virtual server
  - "location": Allows for configuring settings specific to a URL path or pattern within a "server" or "location" itself block.

## Configuration Directives:
  - "error_page" : Sets custom error pages for specific HTTP error codes. | `error_page 404 Not_Found.html;`
  - "access_log", "error_log" : Configures logging settings.  | `access_log /home/servIO/access.log; ` | `error_log /home/servIO/error.log;`
  - "client_max_body_size" : sets the maximum allowed size of the client request body for incoming requests |  `client_max_body_size 10m;`
  - "allow" : allowed methods like [GET, POST, DELETE, UPDATE] | `allow GET POST DELETE;` 
  - "listen" : Defines the IP addresses and ports on which the server will accept incoming requests | `listen 127.0.0.1:80808;`
  - "server_name" : Specifies the domain names or IP addresses that the server block should respond to. | `server_name example.com`
  - "index": Sets the default file to serve when a directory is requested. | `index index.html index.htm;`
  - "root": Specifies the root directory for serving files.  | `root /home/servIO/;`
  - "autoindex": enables directory listing [on, off] `autoindex on;`
  - "return" : used to send a specific HTTP response code back to the client or redirecting to diffirent URL |
      `return 404 "Custum NotFound message";` | `return 301 https://example.com/new-page;`

