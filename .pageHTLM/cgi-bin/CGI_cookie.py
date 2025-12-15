#!/usr/bin/env python3
import cgitb
cgitb.enable()

import cgi
import http.cookies
import os

# Demo credentials
VALID_USER = "admin"
VALID_PASS = "admin"

# Load cookies
cookie = http.cookies.SimpleCookie()
if "HTTP_COOKIE" in os.environ:
    cookie.load(os.environ["HTTP_COOKIE"])

form = cgi.FieldStorage()

# -----------------------------------
# Helper functions
# -----------------------------------
def send_headers(extra_cookie=None):
    if extra_cookie:
        print(extra_cookie.output())
    print("Content-Type: text/html")
    print()

STYLE = """
<style>
body {
    font-family: Arial;
    background: #f1f3f7;
    display: flex;
    height: 100vh;
    justify-content: center;
    align-items: center;
    margin: 0;
}
.card {
    background: white;
    width: 350px;
    padding: 25px;
    border-radius: 10px;
    box-shadow: 0 3px 12px rgba(0,0,0,0.2);
}
h2 {
    margin-top: 0;
    text-align: center;
}
input {
    width: 100%;
    padding: 10px;
    margin: 8px 0;
    border-radius: 6px;
    border: 1px solid #ccc;
}
button {
    width: 100%;
    padding: 12px;
    background: #0078ff;
    color: white;
    border: none;
    border-radius: 6px;
    cursor: pointer;
}
button:hover { background: #005fcc; }
.message { color: red; text-align: center; }
a.logout {
    display: block;
    margin-top: 20px;
    padding: 10px;
    background: #ff4b4b;
    color: white;
    text-align: center;
    border-radius: 5px;
    text-decoration: none;
}
a.logout:hover { background: #d93a3a; }
</style>
"""

def page_login(msg=""):
    return f"""
<html><head>{STYLE}</head><body>
<div class="card">
    <h2>Login</h2>
	<h4>Credentials: <br>admin <br> admin</h3>
    <div class='message'>{msg}</div>
    <form method="POST">
        <input type="text" name="user" placeholder="Username">
        <input type="password" name="pw" placeholder="Password">
        <button type="submit">Login</button>
    </form>
</div></body></html>
"""

def page_admin(username):
    return f"""
<html><head>{STYLE}</head><body>
<div class="card">
    <h2>Welcome, {username}!</h2>
    <p>You are logged in.</p>
    <form method="POST">
        <input type="hidden" name="action" value="logout">
        <button type="submit" class="logout">Log Out</button>
    </form>
</div></body></html>
"""


# -----------------------------------
# Logic
# -----------------------------------

#  Logout request
if form.getfirst("action") == "logout":
    # Delete cookies
    cookie["session"] = ""
    cookie["session"]["path"] = "/"
    cookie["session"]["expires"] = "Thu, 01 Jan 1970 00:00:00 GMT"

    cookie["username"] = ""
    cookie["username"]["path"] = "/cgi-bin"
    cookie["username"]["expires"] = "Thu, 01 Jan 1970 00:00:00 GMT"

    # Send cookie headers and redirect
    print(cookie.output())
    print("Status: 302 Found")
    print("Location: /")
    print()  # End headers
    raise SystemExit


# Already logged in
if "session" in cookie:
    username = cookie["session"].value
    send_headers()
    print(page_admin(username))
    raise SystemExit

#  Login attempt
if "user" in form and "pw" in form:
    user = form.getfirst("user")
    pw = form.getfirst("pw")

    if user == VALID_USER and pw == VALID_PASS:
        cookie["session"] = user
        cookie["session"]["path"] = "/"
        cookie["session"]["max-age"] = 3600

        send_headers(cookie)
        print(page_admin(user))
    else:
        send_headers()
        print(page_login("Invalid username or password"))
    raise SystemExit

send_headers()
print(page_login())
