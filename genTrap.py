for i in range(256):
    if i == 8 or (i >= 10 and i <= 14) or i == 17:
        print "TRAPHANDLER(vector%d, %d)" %(i, i)
    else:
        print "TRAPHANDLER_NOEC(vector%d, %d)" %(i, i)
print "#vector table"
print ".data"
print ".globl vectors"
print "vectors:" 
for i in range(256):
    print ".long vector%d" %i
