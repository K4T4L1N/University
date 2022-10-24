import re

def input_data():
    n = 0
    while n!=1:
        name=input("Name: ")
        if re.match('^[-a-zA-Z]*$', name):
            surname=input("Surname: ")
            if re.match('^[-a-zA-Z]*$', surname):
                children = input("Nr of Children: ")
                if re.match('^[0-1][0-9]*$', children):
                    f = open("data.txt", "a")
                    f.write(name + "\t" + surname + "\t" + children + "\n")
                    f.close()
                    n=1
                else:
                    print("Nr. of children should be between 00 and 19")
                    continue
            else:
                print("Surname should contain just letters (Lower&Upper case)")
                continue
        else:
            print("Name should contain just letters (Lower&Upper case)")
            continue

def output_data():
    f = open("data.txt", "r")
    print(f.read())
    f.close()

def forSanta():
    children=[]

    with open("data.txt", "r") as f:
        lines = f.readlines()

        # Loop through all lines, ignoring header.
        # Add last element to list (i.e. the process name)
        for l in lines[0:]:
            children.append(int(l.split()[-1]))

    print("Santa will need ", sum(children), " gifts\n")

try:
    f = open("data.txt")
    f = open("data.txt", "x")
except IOError:
    print("ForAdmin: File name not accessible")
finally:
    f.close()