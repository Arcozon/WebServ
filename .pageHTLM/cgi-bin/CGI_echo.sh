#!/bin/bash
# Enable error reporting for debugging
set -e

# Read POST data
if [ -z "$CONTENT_LENGTH" ]; then
	MESSAGE="Nothing"
else
	read -n "$CONTENT_LENGTH" POST_BODY
	MESSAGE=$(echo "$POST_BODY" | sed -n 's/.*message=\([^&]*\).*/\1/p' | sed 's/%20/ /g')
fi



# Output CGI headers
echo "Content-Type: text/html; charset=utf-8"
echo ""

# Output HTML
echo "
<html>
<head><title>Echo CGI (Bash)</title></head>
<body>
<h1>Echo (Bash CGI)</h1>
<p>You sent: $MESSAGE</p>
<p><a href="/">Back</a></p>
</body>
</html>"
