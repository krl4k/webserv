import requests as req

blocks = 1

cunkSize = 16384;
hexSize = str(hex(cunkSize * 2))

def generator():
    for i in range(0, blocks):
        print (i)
        v1= hexSize + "\r\n"
        print("hex = " + v1)
        v2= ("ab" * 16384) + "\r\n"
        yield v1.encode('utf8')
        yield v2.encode('utf8')

    v3="0\r\n\r\n"
    print("end")
    yield v3.encode('utf8')

url =  'http://localhost:8000/'


# count user
counUser = 2
for i in range(0, counUser):
    r = req.post(url, data=generator())
    print("==============================================================")
    print(r.text)
    print(i)
