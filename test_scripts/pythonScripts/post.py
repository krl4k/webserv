
import requests as req

contLen = 123445
def generator():
    v1 = ("ab" * 55)
    yield v1.encode('utf8')

url = 'http://localhost:8000/directory/youpi.bla '

# count user
counUser = 1
for i in range(0, counUser):
    print("==============================================================")
    r = req.post(url, data=[('asd', ('av' * 50000000))])
    print("Count    = " + str(i))
    print("Content  = " + r.text)
    print("Response = " + str(r.status_code))
