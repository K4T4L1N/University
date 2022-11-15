class Employee:
    def __init__(self,nameEmployee,phone,bday,email,position):
        self.__nameEmployee=nameEmployee
        self.__phone=phone
        self.__bday=bday
        self.__email=email
        self.__position=position

    
    def calculateSalary(self):
        pass
    
    def _calculateAge(self):
        pass

    # Setters & Getters
    def get_nameEmployee(self):
        return self.__nameEmployee

    def get_phone(self):
        return self.__phone

    def get_bday(self):
        return self.__bday

    def get_email(self):
        return self.__email

    def get_position(self):
        return self.__position

    def set_nameEmployee(self,nameEmployee):
        self.__nameEmployee=nameEmployee
    
    def set_phone(self,phone):
        self.__phone=phone

    def set_bday(self,bday):
        self.__bday=bday

    def set_email(self,email):
        self.__email=email

    def set_position(self,position):
        self.__position=position

    nameEmployee=property(get_nameEmployee,set_nameEmployee)
    phone=property(get_phone,set_phone)
    bday=property(get_bday,set_bday)
    email=property(get_email,set_email)
    position=property(get_position,set_position)

    @property
    def nameEmployee(self):
        return self.__nameEmployee
    
    @nameEmployee.setter
    def nameEmployee(self,nameEmployee):
        self.__nameEmployee=nameEmployee

    @property 
    def phone(self):
        return self.__phone
    
    @phone.setter
    def phone(self,phone):
        self.__phone=phone

    @property 
    def bday(self):
        return self.__bday
    
    @bday.setter
    def bday(self,bday):
        self.__bday=bday
 
    @property 
    def email(self):
        return self.__email
    
    @email.setter
    def email(self,email):
        self.__email=email
    
    @property 
    def position(self):
        return self.__position
    
    @position.setter
    def position(self,position):
        self.__position=position

    def __str__(self):
        return "Name: {} Phone: {} Birthday: {} Email: {} Position: {}".format(self.nameEmployee,self.phone,self.bday,self.email,self.position)