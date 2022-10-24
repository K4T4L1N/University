import datetime as dt

basket = ["mandarine", "banane", "portocale", "cocos", "fistic"]
print(basket)
print(basket[0], basket[2])
basket[4] = "blackcurrant"
print(basket)

gift_box = ("parfume", "snacks", 23)
print(type(gift_box))
print(gift_box[0], gift_box[2])

pets = {"cat", "snake", "dog", "snake", "hamster", "parrot", "lizard", "snake", "snake"}
print(pets)

my_car = {
    "brand": "Mercedes-Benz",
    "model": "W211",
    "year": 2007
}
my_car_details = {
    1: "E200NGT",
    2: "Gasoline/NaturalGas",
    3: "AVANTGARDE"
}

print(my_car["model"], my_car_details[1])
x = str(my_car["year"])
print(my_car["model"], x, my_car_details[1])
print(type(x))

# Sarcina 2
a = ['paine', 'lapte', 'salam']
b = ['3', '7', '8']
res = "\n".join("{} {}".format(x, y) for x, y in zip(a, b))
print(res)

input_age = input("Introduceti varsta:")
now = dt.datetime.now()
current_year = now.year
birthyear=current_year-int(input_age)
print("V-ati nascut in anul:" + str(birthyear))

# in operator working

list1 = [1, 2, 3, 4, 5]
string1 = "My name is AskPython"
tuple1 = (11, 22, 33, 44)

print(5 in list1)  # True
print("is" in string1)  # True
print("name" not in string1)  # False
print(88 in tuple1)  # False