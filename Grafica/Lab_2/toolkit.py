def FileCreate ():
    try:
        f = open("data.txt")
        f = open("data.txt", "x")
    except IOError:
        print("ForAdmin: File name not avaible")
    finally:
        f.close()

def FileInput ():
    labels = ['Audi', 'Mercedes', 'Skoda', 'BMW', 'Volvo']
    yr1_dt = [31, 20, 29, 12, 32]
    yr2_dt = [25, 23, 44, 27, 21]
    yr3_dt = [15, 13, 44, 27, 21]


    if len(labels) == len(yr1_dt):
        if len(yr1_dt) == len(yr2_dt):

            f = open("data.txt", "w")

            for n in range(0, len(labels)-1):
                f.write(labels[n] + "\t" + str(yr1_dt[n]) + "\t" + str(yr2_dt[n]) + "\t" + str(yr3_dt[n]) + "\n")
            f.close()

def FileExtract():
    yr1_dt = []
    yr2_dt = []
    yr3_dt = []
    labels = []

    f = open("data.txt", "r")
    lines = f.readlines()

    for l in lines[0:]:
        yr1_dt.append(int(l.split()[1]))
        yr2_dt.append(int(l.split()[-1]))
        yr3_dt.append(int(l.split()[-2]))
        labels.append(l.split()[0])

    f.close()
    return labels, yr1_dt, yr2_dt, yr3_dt

def Width(x):
    wdth = 1 - x
    return wdth