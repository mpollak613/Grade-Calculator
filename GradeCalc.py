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

def GetGrade(pointsEarned, pointsPossible, weights = {}, drops = {}):
    
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

def GetLetter(grade, scale):
    for letter in scale:
        if (floor(grade) >= scale.get(letter)[0] and floor(grade) <= scale.get(letter)[1]):
            return letter