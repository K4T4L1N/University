from Lab8_class import HourlyEmployee,SalaryEmployee

empl1=SalaryEmployee("Catalin","09231541","22.08.2001","cat@gmail.com","ceo",119200.0)
empl2=HourlyEmployee("Dumitrita","08151259","23.10.2000","dumi.frumi@gmail.com","hr",60,150.2)
empl3=HourlyEmployee("Monica","06124815","27.12.2026","mon1k4@gmail.com","accoutant",60,120.5)
empl4=SalaryEmployee("Herald","07134618","27.12.2026","hrld_1@gmail.com","storage_manager",11200.0)

emps=[empl1,empl2,empl3,empl4]

for i in emps:
    print(i)
    print("Salary:" + str(i._calculateSalary()))