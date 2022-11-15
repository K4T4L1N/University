from Lab7_class import Employee

class HourlyEmployee(Employee):
    def __init__(self, nameEmployee, phone, bday, email, position,nmbrOfHour,hourlyPay):
        super().__init__(nameEmployee, phone, bday, email, position)
        self.__nmbrOfHour=nmbrOfHour
        self.__hourlyPay=hourlyPay

    @property
    def nmbrOfHour(self):
        return self.__nmbrOfHour

    @nmbrOfHour.setter
    def salary(self,nmbrOfHour):
        self.__nmbrOfHour=nmbrOfHour

    @property
    def hourlyPay(self):
        return self.__hourlyPay

    @hourlyPay.setter
    def salary(self,hourlyPay):
        self.__hourlyPay=hourlyPay

    def _calculateSalary(self):
      return self.hourlyPay*self.nmbrOfHour 



class SalaryEmployee(Employee):
    def __init__(self, nameEmployee, phone, bday, email, position,salary):
        super().__init__(nameEmployee, phone, bday, email, position)
        self.__salary=salary

    @property
    def salary(self):
        return self.__salary

    @salary.setter
    def salary(self,salary):
        self.__salary=salary

    def _calculateSalary(self):
      return self.salary 


