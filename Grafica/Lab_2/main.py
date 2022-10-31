import matplotlib.pyplot as plt
import numpy as np
import toolkit as tk


"""labels = ['Audi', 'Mercedes', 'Skoda', 'BMW', 'Volvo']
yr1 = [20, 35, 30, 35, 27]
yr2 = [25, 32, 34, 20, 25]"""

# create file + verify if it already exists
tk.FileCreate()

# inserting data in the file
tk.FileInput()

# extracting the data
labels, yr1, yr2, yr3 = tk.FileExtract()

fig, (ax, ax2) = plt.subplots(ncols=2, constrained_layout=True)

x = 0.5  # the width of the bars
width = tk.Width(x)

ax.bar(labels, yr1, width, label='Year 1')
ax.bar(labels, yr2, width,  bottom=yr1,label='Year 2')
ax.bar(labels, yr3, width,  bottom=yr2,label='Year 3')


ax.set_ylabel('Quantity')
ax.set_title('Car Sales')
ax.legend()

#double nested pie
size = 0.2

cmap = plt.colormaps["tab20c"]
outer_colors = cmap(np.arange(5)*4)
inner_colors = cmap(np.arange(5)*4)

ax2.pie(yr1, radius=1, colors=outer_colors, labels=labels, wedgeprops=dict(width=size, edgecolor='w'))
ax2.pie(yr2, radius=1-size, colors=inner_colors, wedgeprops=dict(width=size, edgecolor='w'))
ax2.pie(yr2, radius=1-size-0.2, colors=inner_colors, wedgeprops=dict(width=size, edgecolor='w'))

ax2.set(aspect="equal", title='Pie diagram: Car Sales')
ax2.legend(loc=(-0.1, -0.05))

plt.show()