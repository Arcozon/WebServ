wrk.method = nil
wrk.path   = nil
wrk.headers = {}

request = function()
  return
    "GET / HTTP/1.1\r\n" ..
    "Host : 127.0.0.1\r\n" ..
    "Bad Header: value\r\n" ..
end

response = function(status, headers, body)
  if counter[status] == nil then
    counter[status] = 1
  else
    counter[status] = counter[status] + 1
  end
end

done = function(summary, latency, requests)
  io.write("\nHTTP Status Breakdown:\n")
  for code, count in pairs(counter) do
    io.write(string.format("  %d -> %d responses\n", code, count))
  end
end
