from CollegeClass import *


def Example():
    # Example of a class:
    apmth_226 = CollegeClass(name="Neural Computation", units=4, letterScale=GenScale)
    apmth_226.institution = "Harvard University"
    apmth_226.details = "APMTH 226 #212912"
    apmth_226.crn = 14469
    apmth_226.days = "MW"
    apmth_226.setStartDateTime(year=2021, month=9, day=1, hour=15, minute=0)
    apmth_226.setEndDateTime(2021, 12, 2, 16, 15)
    apmth_226.location = "Maxwell Dworkin G125 (SEAS)"
    apmth_226.instructor = "Cengiz Pehlevan"
    apmth_226.addBook(book="Some Book Title, by Ann Perrson, Som Pulishers, 2017.")

    apmth_226.addCategory(name='Assignments', weight=0.1, drop=2)
    apmth_226.addCategory('Projects', 0.2, rpl=(1, 'Final Proj'))
    apmth_226.addCategory('Proj Draft', 0.25)
    apmth_226.addCategory('Final Proj', 0.45)
  
    apmth_226.addGrade(name='Assignments', earn=9, poss=10)
    apmth_226.addGrade('Assignments', 7, 10)
    apmth_226.addGrade('Assignments', 9, 10)
    apmth_226.addGrade('Assignments', 10, 10)
    apmth_226.addGrade('Assignments', 9.5, 10)
    apmth_226.addGrade('Assignments', 6, 10)
    apmth_226.addGrade('Assignments', 10, 10)
    apmth_226.addGrade('Assignments', 10, 10)
    apmth_226.addGrade('Projects', 25.5, 30)
    apmth_226.addGrade('Projects', 28, 30)
    apmth_226.addGrade('Projects', 23, 25)
    apmth_226.addGrade('Projects', 12.5, 15)
    apmth_226.addGrade('Proj Draft', 117.5, 125)
    apmth_226.addGrade('Final Proj', 207, 225)


    print(apmth_226)

    print('Your GPA is:', round(apmth_226.getGradePoints() / apmth_226.getUnits(), 3))

Example()
