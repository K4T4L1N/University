import Options as op
import Sarc2 as ex2


i=0

while  i!=4:
  print("[1 - Print the shopping list; 2 - Add a new item; 3 - Exit]")

  i = input()
  if i == '1':
    op.print_list()

  elif i == "2":
    op.add_element()

  elif i == "3":
    op.delete()

  elif i == "4":
    print("!!! Dobby went shopping !!!")
    break

  else:
    print("Please choose one of the of the options:")

print("---------------> SARCINA 2 <---------------")

ex2.sarc2_list()
ex2.sarc2_tuple()
ex2.sarc2_dict()