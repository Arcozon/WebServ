#!/usr/bin/env python3

import cgi
import cgitb

# Enable error reporting
cgitb.enable()

# Read form data from POST
form = cgi.FieldStorage()
message = form.getfirst("message", "")

# Output CGI headers
print("Content-Type: text/html; charset=utf-8")
print()  # blank line separates headers from body

# HTML output
print("<html>")
print("<head><title>Echo CGI (Python)</title></head>")
print("<body>")
print("<h1>Echo (Python CGI)</h1>")
print(f"<p>You sent: {message}</p>")
print('<p><a href="/">Back</a></p>')
print("</body>")
print("</html>")
