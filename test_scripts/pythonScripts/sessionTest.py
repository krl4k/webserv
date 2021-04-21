from requests import Request, Session

url = 'http://localhost:8000/lol?key=value'

s = Session()
# req = Request('POST',  url)
req = Request('GET',  url)

prepped = s.prepare_request(req)

prepped.body = "Seriously, send exactly these bytes."
# prepped.headers["Content-Length"] = len(prepped.body)
# prepped.headers["Content-Length"] = str(len(prepped.body) + str(" "))
# prepped.headers["Transfer-Encoding"] = str("chunked")
# prepped.body = "23\r\nThis is the data in the first chunk\r\n3con\r\n0\r\n\r\n"


resp = s.send(prepped)


print("Content  = " + resp.text)
print(resp)
if resp:
    print('Response OK')
else:
    print('Response Failed')

