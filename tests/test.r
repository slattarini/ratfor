# $Id$
define ONE 1
integer x,y
x=ONE; y=2
if(x == y)
    write(6,600)
else if(x > y)
    write(6,601)
else
    write(6,602)
x=1
while(x < 10){
    if(y != 2) break
    if(y != 2) next
    write(6,603)x
    x=x+1
}
repeat
    x=x-1
until(x == 0)
# 8%12 is "octal 12" a.k.a. 10
for(x=0; x < 8%12; x=x+1)
    write(6,604)x
600 format('Wrong, x != y')
601 format('Also wrong, x < y')
602 format('Ok!')
603 format('x = ',i2)
604 format('x = ',i2)
end
