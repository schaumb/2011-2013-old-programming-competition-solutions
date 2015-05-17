
f = open('logo.txt', 'r')
res = []
for index, line in enumerate(f):
    act = line[0]
    acc = []
    for i in range(len(line)):
        if line[i] != act:
            acc.append((act, i))
            act = line[i]
    res.append(acc)
    #print(index+1, acc)
    
for line in res:
    for i in range(len(line)-1, -1, -1):
        if i !=0:
            char = line[i][0]
            num = line[i][1]
            nextnum = line[i-1][1]
            line[i] = (char, num - nextnum)
"""
for index, line in enumerate(res):
    print(index+1, line)
"""

for i in range(int(len(res)/2)):
    print(i+1, res[i], len(res)-i, res[len(res)-i-1])
    
for i in range (int(len(res)/2)):
    line = ''
    for char, num in res[i]:
        for i in range(0,num):
            line += char
    print(line)

line = ''
for char, num in res[48]:
    for i in range(0,num):
        line += char
    print(line)

R = res[0:int(len(res)/2)]
R = [i for i in range(len(R))]
R.reverse()
for i in (R):
    line = ''
    if len(res[i]) == 1:
        x = res[i][0][1]
        for j in range(71-x):
            line += ' '
        for j in range(x):
            line += '#'
        
    if len(res[i]) == 2:
        c = res[i][0][1] + res[i][1][1]
        c = 71-c
        for j in range(c):
            line += ' '
        for j in range(res[i][1][1]):
            line += '#'
        
    if len(res[i]) == 3:
        c = 71 - (res[i][0][1] + res[i][1][1] + res[i][2][1])
        for j in range(c):
            line += ' '
        for k in range(2,-1,-1):
            num = res[i][k][1]
            for p in range(0,num):
                line += res[i][k][0]
        
    if len(res[i]) == 4:
        c = 71 - (res[i][0][1] + res[i][1][1] + res[i][2][1] + res[i][3][1])
        for j in range(c):
            line += ' '
        for k in range(3,0,-1):
            num = res[i][k][1]
            for p in range(num):
                line += res[i][k][0]
    print(line)

for i in range(int(len(res)/2)+1):
    print(i+1, res[i])
