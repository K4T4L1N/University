import functions as fc

fc.fnc_noparam()

fc.fnc_simple("Catalin")

print(fc.fnc_return(3))

v = fc.fnc_lambda(2)

print(v(11))

print('Enter fibbonaci limit :')
x = input()

if int(x) > 0:
  print("you have inserted a nr above 0")
  fc.fib(int(x))
elif int(x) == 0:
    print("Zero (((")
else:
    print("the number is negative XD")

thislist = ["apple", "banana", "cherry"]
for i in range(len(thislist)):
  print(thislist[i])

thistuple = ("Cola", "Sprite", "Fanta")
for x in thistuple:
  print(x)
thisdict = {
  "brand": "Ford",
  "model": "Mustang",
  "year": 1964
}
for x, y in thisdict.items():
  print(x, y)

