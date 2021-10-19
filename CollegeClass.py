# * Copyright 2020-2021 Michael Pollak.
# *
# * Use of this source code is governed by an MIT-style
# * licence that can be found in the LICENSE file.

from copy import deepcopy
from math import floor
import datetime

G11Scale = {
        'A': [90, 100],
        'A-': [85, 89],
        'B+': [80, 84],
        'B': [75, 79],
        'B-': [70, 74],
        'C+': [67, 69],
        'C': [64, 66],
        'C-': [60, 63],
        'D+': [57, 59],
        'D': [54, 56],
        'D-': [50, 53],
        'F': [0, 49],
    }

GenScale = {
    'A': [90, 100],
    'B': [80, 89],
    'C': [70, 79],
    'D': [60, 69],
    'F': [0, 59],
    }

HighScale = {
    'A': [93, 100],
    'A-': [90, 92],
    'B+': [87, 89],
    'B': [83, 86],
    'B-': [80, 82],
    'C+': [77, 79],
    'C': [73, 76],
    'C-': [70, 72],
    'D': [60, 69],
    'F': [0, 59],
    }

PFScale = {
    'P': [60, 100],
    'F': [0, 59],
    }

DAYS = [
    "M",
    "T",
    "W",
    "R",
    "F",
    "S",
    "Y",
]

class CollegeClass:

    def __init__(self, name, units, letterScale = G11Scale):
        self.__name = str(name)
        self.institution = ""
        self.details = ""
        self.crn = 0
        self.days = ""
        self.__start_datetime = ""
        self.__end_datetime = ""
        self.location = ""
        self.instructor = ""
        self.__grade = None
        self.__letter = None
        self.__units = int(units)
        self.__gradePoints = None
        self.books = []
        self.__pointsEarned = {}
        self.__pointsPossible = {}
        self.__letterScale = dict(letterScale)
        self.__weights = {}
        self.__drops = {}
        self.__replace = {}
        self.__withdrawn = False
        self.__replaced = False
        return #no_return

    def __str__(self):
        info = 'Course Name: {}\n'.format(self.__name)
        info += 'Institution: {}\n'.format(self.institution)
        info += 'Details: {}\n'.format(self.details)
        info += 'CRN: {}\n'.format(self.crn)
        info += 'Days: {}\n'.format(self.getDays(self.days))
        info += 'Time: {}\n'.format(self.getStartTime() + ' - ' + self.getEndTime())
        info += 'Date: {}\n'.format(self.getStartDate() + ' - ' + self.getEndDate())
        info += 'Location: {}\n'.format(self.location)
        info += 'Instructor: {}\n'.format(self.instructor)
        info += 'Grade: {}\n'.format(self.__grade)
        info += 'Letter: {}\n'.format(self.__letter)
        info += 'Credit Hours: {}\n'.format(self.__units)
        info += 'Grade Points: {}\n'.format(self.__gradePoints)
        info += self.showBooks()
        info += self.showPoints()
        info += self.showWeights()
        info += self.showDrops()
        return info

    def getDays(self, days):
        daystr = ""
        for d in DAYS:
            if d in days:
                daystr += d
            else:
                daystr += "-"
        return daystr

    def getName(self):
        return self.__name

    def getGrade(self):
        return self.__grade
    
    def getLetter(self):
        return self.__letter

    def getUnits(self):
        return self.__units

    def getGradePoints(self):
        return self.__gradePoints

    def getStartTime(self):
        if self.__start_datetime != "" and self.__start_datetime.strftime('%H:%M') != "00:00":
            return self.__start_datetime.strftime('%I:%M %p')
        return ""
    
    def getEndTime(self):
        if self.__end_datetime != "" and self.__end_datetime.strftime('%H:%M') != "00:00":
            return self.__end_datetime.strftime('%I:%M %p')
        return ""

    def getStartDate(self):
        if self.__start_datetime != "":
            return self.__start_datetime.strftime('%m/%d/%Y')
        return ""

    def getEndDate(self):
        if self.__end_datetime != "":
            return self.__end_datetime.strftime('%m/%d/%Y')
        return ""

    def getDateVal(self):
        if self.__start_datetime != "":
            return self.__start_datetime.timestamp()
        return 0

    def getLowest(self, category):
        if category.upper() not in self.__weights.keys():
            print("Category does not exist!")
            return -1
        grades = list(pe/pp for pe,pp in zip(self.__pointsEarned.get(category.upper()), self.__pointsPossible.get(category.upper())))
        return min(range(len(grades)), key=grades.__getitem__)

    def _setGrade(self, grade):
        self.__grade = float(grade)
        self.updateLetter()
        self.updateGradePoints()
        return #no_return

    def setReplaced(self):
        self.__replaced = True
        self.updateGradePoints()
        return #no_return

    def setStartDateTime(self, year, month, day, hour = 0, minute = 0):
        self.__start_datetime = datetime.datetime(year, month, day, hour, minute);
        return #no_return

    def setEndDateTime(self, year, month, day, hour = 0, minute = 0):
        self.__end_datetime = datetime.datetime(year, month, day, hour, minute);
        return #no_return

    def updateLetter(self):
        if (self.__withdrawn == True):
            self.__letter = 'W'
            return #no_return
        for letter in self.__letterScale:
            if (floor(self.__grade) >= self.__letterScale.get(letter)[0] and floor(self.__grade) <= self.__letterScale.get(letter)[1]):
                self.__letter = letter
                return #no_return

    def updateGradePoints(self):
        if(self.__letter == 'P' or self.__letter == 'F' or self.__withdrawn == True or self.__replaced == True):
            self.__gradePoints = None
        elif (self.__letter == 'A'):
            self.__gradePoints = 4.0 * self.__units
        elif (self.__letter == 'A-'):
            self.__gradePoints = round(3.7 * self.__units, 2)
        elif (self.__letter == 'B+'):
            self.__gradePoints = round(3.3 * self.__units, 2)
        elif (self.__letter == 'B'):
            self.__gradePoints = 3.0 * self.__units
        elif (self.__letter == 'B-'):
            self.__gradePoints = round(2.7 * self.__units, 2)
        elif (self.__letter == 'C+'):
            self.__gradePoints = round(2.3 * self.__units, 2)
        elif (self.__letter == 'C'):
            self.__gradePoints = 2.0 * self.__units
        elif (self.__letter == 'C-'):
            self.__gradePoints = round(1.7 * self.__units, 2)
        elif (self.__letter == 'D+'):
            self.__gradePoints = round(1.3 * self.__units, 2)
        elif (self.__letter == 'D'):
            self.__gradePoints = 1.0 * self.__units
        elif (self.__letter == 'D-'):
            self.__gradePoints = round(0.7 * self.__units, 2)
        elif (self.__letter == 'F'):
            self.__gradePoints = 0.0
        return #no_return

    def showLetterScale(self):
        return 'Grading Scale:\n ' + '\n '.join('{:12}{}'.format(i, j) for i, j in self.__letterScale.items()) + '\n'
    
    def showBooks(self):
        if len(self.books) != 0:
            return 'Book(s):\n ' + '\n '.join(b for b in self.books) + '\n'
        else:
            return 'Book(s):\n'

    def points_Iter(self):
        for earn, poss in zip(self.__pointsEarned.values(), self.__pointsPossible.values()):
            points = []
            for e, p in zip(earn, poss):
                points.append('{}/{}'.format(e,p))
            yield ', '.join(points)

    def showPoints(self):
        if len(self.__pointsEarned) != 0:
            return 'Points:\n ' + '\n '.join('{:12}{}'.format(n, p) for n, p in zip(self.__pointsEarned.keys(), self.points_Iter())) + '\n'
        else:
            return 'Points:\n'

    def showWeights(self):
        if len(self.__weights) != 0:
            return 'Weights:\n ' + '\n '.join('{:12}{}'.format(i, j) for i, j in self.__weights.items()) + '\n'
        else:
            return 'Weights:\n'

    def showDrops(self):
        if len(self.__drops) != 0 and any(d != 0 for d in self.__drops.values()):
            return 'Drops:\n ' + '\n '.join('{:12}{}'.format(i, j) for i, j in self.__drops.items() if j != 0) + '\n'
        else:
            return 'Drops:\n'

    def withdraw(self):
        self.__withdrawn = True
        self.updateGrade()
        return #no_return

    def addBook(self, book):
        self.books.append(book)

    def addExtraCredit(self, amount, category = 'Extra'):
        """
        NOT IMPLEMENTED YET
        """
        if (self.__withdrawn == True):
            print("You can't add extra credit after withdrawing.")
            return #no_return

    def makeExtraCredit(self, category, index):
        if self.__withdrawn == False:
            self.__pointsPossible[category.upper()][index] = 1
            self.updateGrade()
        return #no_return

    def addCategory(self, name, weight = 0, drop = 0, rpl = (0, '')):
        if (self.__withdrawn == True):
            print("You can't add a category after withdrawing.")
        name = name.upper()
        self.__pointsEarned.update({name: []})
        self.__pointsPossible.update({name: []})
        self.__weights.update({name: weight})
        self.__drops.update({name: drop})
        self.__replace.update({name: (rpl[0], rpl[1].upper())})
        return #no_return
    
    def addGrade(self, name, earn, poss):
        if (self.__withdrawn == True):
            print("You can't add a grade after withdrawing.")
            return #no_return
        name = name.upper()
        if name not in self.__pointsEarned:
            self.addCategory(name)
        self.__pointsEarned.setdefault(name).append(earn)
        self.__pointsPossible.setdefault(name).append(poss)
        self.updateGrade()
        return #no_return

    def updateGrade(self):
        if (self.__withdrawn == True):
            self.__grade = None
            self.updateLetter()
            self.updateGradePoints()
            return #no_return
        if (self.__replaced == True):
            return #no_return

        #make editable copies of point values.
        pointsEarned = deepcopy(self.__pointsEarned)
        pointsPossible = deepcopy(self.__pointsPossible)

        #makes sure everything is a valid format.
        if(sum(self.__weights.values()) != 1 and len(self.__weights) != 0):
            for wei in self.__weights.keys():
                 #evenly distribute the weight if the current weight is bad.
                self.__weights[wei] = 1.0 / len(self.__weights)
    
        #calculates total points using weights for each group.
        finalGrade = []
        unusedWeight = []
        for earn,poss,drp,wei,rpl in zip(pointsEarned.values(),
                                         pointsPossible.values(),
                                         self.__drops.values(),
                                         self.__weights.values(),
                                         self.__replace.values()):
            if (len(earn) == 0):
                unusedWeight.append(wei)
            else:
                #turns point values into percentages.
                if (drp > 0 or rpl[0] > 0):
                    grades = list(pe/pp for pe,pp in zip(earn, poss))

                #drop lowest grades if necessary.
                for i in range(drp):
                    if (len(grades) != 0):
                        min_index = min(range(len(grades)), key=grades.__getitem__)
                        grades.pop(min_index)
                        earn.pop(min_index)
                        poss.pop(min_index)

                #replace grades if necessary.
                if (rpl[0] > 0 and rpl[1] in self.__weights and len(pointsEarned[rpl[1]]) != 0):
                    for i in range(rpl[0]):
                        min_index = min(range(len(grades)), key=grades.__getitem__)
                        earnRpl = pointsEarned[rpl[1]][0]
                        possRpl = pointsPossible[rpl[1]][0]
                        earn[min_index] = earnRpl
                        poss[min_index] = possRpl
        
                #sum up the group's grades and apply its weight.
                if (len(earn) != 0):
                    finalGrade.append((sum(earn) / sum(poss)) * wei)

        #update stats.
        if (len(finalGrade) != 0):
            #uses a runnning weighted average by removing any unused weight.
            self.__grade = round(sum(finalGrade) * 100 / (1 - sum(unusedWeight)), 5)
            self.updateLetter()
            self.updateGradePoints()
        return #no_return

class CollegeClassWLAB(CollegeClass):

    def __init__(self, name, units, letterScale = G11Scale):
        super().__init__(name, units, letterScale)
        self.LabDays = ""
        self.__LabStart_datetime = ""
        self.__LabEnd_datetime = ""
        self.LabLocation = ""

    def __str__(self):
        info = 'Course Name: {}\n'.format(self.getName())
        info += 'Institution: {}\n'.format(self.institution)
        info += 'Details: {}\n'.format(self.details)
        info += 'CRN: {}\n'.format(self.crn)
        info += 'Days: {}\n'.format(self.getDays(self.days))
        info += 'Lab Days: {}\n'.format(self.getDays(self.LabDays))
        info += 'Time: {}\n'.format(self.getStartTime() + ' - ' + self.getEndTime())
        info += 'Lab Time: {}\n'.format(self.getLabStartTime() + ' - ' + self.getLabEndTime())
        info += 'Date: {}\n'.format(self.getStartDate() + ' - ' + self.getEndDate())
        info += 'Lab Date: {}\n'.format(self.getStartDate() + ' - ' + self.getEndDate())
        info += 'Location: {}\n'.format(self.location)
        info += 'Lab Location: {}\n'.format(self.LabLocation)
        info += 'Instructor: {}\n'.format(self.instructor)
        info += 'Grade: {}\n'.format(self.getGrade())
        info += 'Letter: {}\n'.format(self.getLetter())
        info += 'Credit Hours: {}\n'.format(self.getUnits())
        info += 'Grade Points: {}\n'.format(self.getGradePoints())
        info += self.showBooks()
        info += self.showPoints()
        info += self.showWeights()
        info += self.showDrops()
        return info
        
    
    def getLabStartTime(self):
        if self.__LabStart_datetime != "" and self.__LabStart_datetime.strftime('%H:%M') != "00:00":
            return self.__LabStart_datetime.strftime('%I:%M %p')
        return ""
    
    def getLabEndTime(self):
        if self.__LabEnd_datetime != "" and self.__LabEnd_datetime.strftime('%H:%M') != "00:00":
            return self.__LabEnd_datetime.strftime('%I:%M %p')
        return ""

    def getLabStartDate(self):
        if self.__LabStart_datetime != "":
            return self.__LabStart_datetime.strftime('%m/%d/%Y')
        return ""

    def getLabEndDate(self):
        if self.__LabEnd_datetime != "":
            return self.__LabEnd_datetime.strftime('%m/%d/%Y')
        return ""

    def setLabStartDateTime(self, year, month, day, hour = 0, minute = 0):
        self.__LabStart_datetime = datetime.datetime(year, month, day, hour, minute);
        return #no_return

    def setLabEndDateTime(self, year, month, day, hour = 0, minute = 0):
        self.__LabEnd_datetime = datetime.datetime(year, month, day, hour, minute);
        return #no_return
