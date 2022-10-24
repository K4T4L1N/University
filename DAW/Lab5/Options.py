def delete():
    print("What element to delete?")
    element=input()
    shopping_list.remove(element)
    print(element, " removed")
    print(shopping_list)

def add_element():
    n=0
    while n != 4:
        print("[  1 - add one element; 2 - add an element on a certain position; 3 - add multiple items; 4 - Stop adding items!  ]")

        n = input()
        if n == '1':
            print("What item do you want to insert?")
            new_item = input()
            shopping_list.append(new_item)
            print(new_item, "inserted succesfully")

        elif n == "2":
            print("On what position to insert the item?")
            index=int(input())
            print("What item do you want to insert?")
            new_item = input()
            shopping_list.insert(index, new_item)


        elif n == "3":
            nr=int(input("how many elements do you want to insert?"))
            for i in range(0, nr):
                ele = input()
                shopping_list.append(ele)

        elif n == "4":
            break

        else:
            print("Please choose one of the of the options:")

def print_list():
    print("The shopping list:\n", shopping_list)
shopping_list=["Milk", "Ham", "Eggs"]