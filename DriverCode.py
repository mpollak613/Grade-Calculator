import GradeCalc


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
        'Final': [100]
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

    grade = GradeCalc.GetGrade(pointsEarned, pointsPossible, weights, drops)
    print('Your current grade is:', str(grade) + '%')

    letter = GradeCalc.GetLetter(grade, gradeScale)
    print('Your current letter is:', letter)

Driver()
