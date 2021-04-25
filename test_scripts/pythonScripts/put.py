import requests as req

contLen = 123445
def generator():
    v1 = ("ab" * contLen)
    yield v1.encode('utf8')

url = 'http://localhost:8000/'

# count user
counUser = 1
for i in range(0, counUser):
    print("==============================================================")
    r = req.put(url, data=("ad" * 5000000))
    print("Count    = " + str(i))
    print("Content  = " + r.text)
    print("Response = " + str(r.status_code))