###########################################################
#                                                         #
# Copywrite 2020 by Michael Pollak.                       #
#                                                         #
# This is intended to calculate a weighted grade average. #
#                                                         #
# Scroll down to the bottom to customise scores.          #
#                                                         #
###########################################################

from math import floor


def GradeWeighted(pointsEarned, pointsPossible, weights = {}, drops = {}):
    
    #makes sure everything is a valid format.
    if (len(pointsEarned) != len(pointsPossible)):
        return 'Error: The length of earned and possible points are not the same!'
    if(sum(weights.values()) != 1 and len(weights) != 0):
        return 'Error: Total weights must equal 1!'
    
    #if no weight is given, one will be populated with equally distributed weight.
    if (len(weights) == 0):
        weights = dict.fromkeys((range(len(pointsEarned))),1 / len(pointsEarned))
    
    #if drops is empty, a zeroed one will be populated.
    if (len(drops) == 0):
        drops = dict.fromkeys((range(len(pointsEarned))),0)
    
    #calculates total points and adds weights to each group.
    finalGrade = []
    for earn,poss,drp,wei in zip(pointsEarned, pointsPossible, drops.values(), weights.values()):
        if (len(pointsEarned.get(earn)) != 0):
            grades = [earnpt / posspt for earnpt, posspt in zip(pointsEarned.get(earn), pointsPossible.get(poss))]
        
            #drop lowest grades in each group if necessary.
            for i in range(drp):
                grades.remove(min(grades))
        
            #sum up each group and take the average.
            finalGrade.append((sum(grades) / len(grades)) * wei)
            
    #calculate unused weight.
    remWeight = 0
    if (len(weights) > len(finalGrade)):
        remWeight = sum(list(weights.values())[len(weights) - (x + 1)] for x in range(len(weights) - len(finalGrade)))

    return round(sum(finalGrade) * 100 / (1 - remWeight), 5)


def Driver():

    #Make sure the lists in pointsEarned and pointsPossible are the same length and in order.
    #If you don't have a grade in a group yet, but it has a weight, just leave the group empty and keep the weight.
    pointsEarned = {
        'HW': [10, 13, 19, 16, 12],
        'Quiz': [10, 10, 4, 9, 9],
        'Midterm': [92],
        'Final': []
    }
    pointsPossible = {
        'HW': [11, 17, 25, 16, 14],
        'Quiz': [10, 10, 10, 10, 10],
        'Midterm': [100],
        'Final': []
    }

    #input weights as decimals.
    weights = {
      'HW': 0.2,
      'Quiz': 0.2,
      'Midterm': 0.25,
      'Final': 0.35
      }

    #number of lowest grade drops for each group.
    drops = {
      'HW': 1,
      'Quiz': 1,
      'Midterm': 0,
      'Final': 0
      }

    #change the intervals as needed.
    gradeScale = {
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
        'F': [0, 49]
    }

    grade = GradeWeighted(pointsEarned, pointsPossible, weights, drops)

    print('Your current grade is:', str(grade) + '%')

    for letter in gradeScale:
        if (floor(grade) >= gradeScale.get(letter)[0]
                and floor(grade) <= gradeScale.get(letter)[1]):
            print('Your current letter grade is:', letter)


Driver()
