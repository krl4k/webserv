from requests import Request, Session

url = 'http://localhost:8000/'

s = Session()
# req = Request('POST',  url)
req = Request('POST',  url)

prepped = s.prepare_request(req)
prepped.body = "10\r\nSeriously, send1\r\n0\r\n\r\nexactly these bytes."
prepped.headers["Content-Length"] = len(prepped.body)

# prepped.headers["TRANSFER-ENCODING"] = "chunked"
# prepped.headers["Content-Length"] = str(len(prepped.body) + str(" "))

resp = s.send(prepped)

print("Content  = " + resp.text)
print(resp)
if resp:
    print('Response OK')
else:
    print('Response Failed')