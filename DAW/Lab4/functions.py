def fnc_lambda(n):
  return lambda a : a * n

def fnc_noparam():
  print("Hello from a function with no parameters")

def fnc_simple(fname="ana"):
  print(fname + " Pavlovschi")

def fnc_return(x):
  return 5 * x

def fib(n):
    a, b = 0, 1
    while a < n:
      print(a, end=' ')
      a, b = b, a + b
    print()