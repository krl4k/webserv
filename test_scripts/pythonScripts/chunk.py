import requests as req

blocks = 100
def generator():
    for i in range(0, blocks):
        str = ("lorem ipsum" * 1000000)
        yield str.encode('utf8')

url =  'http://localhost:8000/'

# def gen():
#     yield ("one").encode('utf8')
#     yield ("two").encode('utf8')
#     yield ("three").encode('utf8')

for i in range(0, 1):
    r = req.post(url, data=generator())
    print("==============================================================")
    # print(i)
    print(r)
    print(r.text)
