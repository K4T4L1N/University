import ToolKit as tk

i=0

while  i!=4:
  print("[1 - Input data; 2 - View data; 3 - Exit]")

  i = input()
  if i == '1':
    tk.input_data()

  elif i == "2":
    tk.output_data()
    tk.forSanta()


  elif i == "3":
    print("!!! Warning, the program was stopped !!!")
    break

  else:
    print("Please choose one of the of the options:")