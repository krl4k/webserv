import requests as req

contLen = 1234
def generator():
    v1 = ("ab" * contLen)
    yield v1.encode('utf8')

url = 'http://localhost:8000/'

# count user
counUser = 20
for i in range(0, counUser):
    print("==============================================================")
    r = req.post(url)
    print("Count    = " + str(i))
    print("Content  = " + r.text)
    print("Response = " + str(r.status_code))
