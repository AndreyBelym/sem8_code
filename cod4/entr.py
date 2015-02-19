from math import log2
l=[0 for i in range(0,256)]
cnt=0
with open("s1.txt", "rb") as f:
    byte = f.read(1)
    while len(byte):
        if(byte[0]<0 or byte[0]>255):
            print(byte[0])
        l[byte[0]]+=1
        cnt+=1
        byte = f.read(1)
summ=0
for i in range(0,256):
    if l[i]:
        print(chr(i),l[i]/cnt)
