#!/usr/bin/env python3
import cgi
import cgitb
cgitb.enable()


print("Content-Type: text/html; charset=utf-8")
print()
print("<html><head><title>Hello CGI</title></head><body>")
print("<h1>Hello from CGI!</h1>")
print("<p>This script runs on the server and returns a simple HTML page.</p>")
print("</body></html>")

