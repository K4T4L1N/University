def sarc2_list():
    list1=[1, 2, 3, 4, 5, 6, 7, 8, 9]
    print("Lista este: ", list1)
    list1.reverse()
    print("Lista reversata este: ", list1)

def sarc2_tuple():
    thistuple = (1, 3, 7, 8, 7, 5, 4, 6, 8, 5)

    x = thistuple.count(5)

    print("There are ", x, " in the tuple")

def sarc2_dict():
    car = {
        "brand": "Ford",
        "model": "Mustang",
        "year": 1964
    }

    car.update({"color": "White"})

    print(car)
