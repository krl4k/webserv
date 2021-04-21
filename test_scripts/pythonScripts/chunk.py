# import requests as req
#
# blocks = 1
#
# chunkSize = 10
# sstr = "ab"
#
# chunkHexSize = str(hex(chunkSize * int(len(sstr))))
#
#
# def chunkGenerator():
#     for i in range(0, blocks):
#         # print(i)
#         v1 = str(hex(chunkSize)) + "\r\n"
#         # print("hex = " + v1)
#         v2 = (sstr * chunkSize) + "\r\n"
#         yield v1.encode('utf8')
#         yield v2.encode('utf8')
#
#     v3 = "0\r\n\r\n"
#     # print("end")
#     yield v3.encode('utf8')
#
# url = 'http://localhost:8000/'
#
# # count user
# counUser = 1
# for i in range(0, counUser):
#     print("chunkSize = " + str(chunkSize * 2))
#     r = req.post(url, data=chunkGenerator())
#     print("==============================================================")
#     print(r.text)
#     print(i)

import requests as req
# blocks = 1
# def generator():
#     for i in range(0, blocks):
#         print(i)
#         v1="10\r\n"
#         print("v1 = " + v1)
#         v2= ("ab" * 8) + "\r\n"
#         print("v2 = " + v2)
#         v3 = v1 + v2
#         print("v3 = " + v3)
#         yield v3.encode('utf8')
#
#     v3="0\r\n\r\n"
#     print("end")
#     yield v3.encode('utf8')



url =  'http://localhost:8000/'


def gen():
    yield "one".encode('utf8')
    # time.sleep(1.0)
    yield "two".encode('utf8')
    # time.sleep(1.0)
    yield "three".encode('utf8')


for i in range(0, 25):
    r = req.post(url, data=gen())
    print("==============================================================")
    # print(i)
    print(r)
    print(r.text)
