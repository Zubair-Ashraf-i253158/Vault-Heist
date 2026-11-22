#pragma once
#include <iostream>
#include <cmath>

using namespace std;

class ScoreSystem
{
public:
    int currentAlarm;
    int maxAlarm;
    int optimalSteps;
    int stepsTaken;
    int penalties;
    int score;
    int stars;

    ScoreSystem()//score system initialize karega
    {
        reset(5);
    }

    void reset(int optimal)//score aur alarm ko reset karega
    {
        optimalSteps = optimal;
        //optimal steps ke hisaab se maximum alarm calculate karega
        maxAlarm = (int)ceil(1.5f * optimal) + 2;
        currentAlarm = 0;
        stepsTaken = 0;
        penalties = 0;
        score = 1000;
        stars = 0;
    }

    void addStep()//har move par steps aur alarm barhaye ga
    {
        stepsTaken++;
        currentAlarm++;
        score -= 10;
        if (score < 0)//score ko zero se neeche nahi jane dega
            score = 0;
    }

    void addPenalty(int alarmCost, int pointCost)//penalty lagne par alarm aur score update karega
    {
        penalties++;
        currentAlarm += alarmCost;
        score -= pointCost;
        if (score < 0)//score ko zero se neeche nahi jane dega
            score = 0;
    }

    bool isAlarmFull()//check karega alarm limit poori hui ya nahi
    {
        return currentAlarm >= maxAlarm;
    }

    int calculateStars()//performance ke hisaab se stars calculate karega
    {
        if (isAlarmFull())//alarm full hone par zero stars milenge
        {
            stars = 0;
        }
        else if (stepsTaken <= optimalSteps && penalties == 0)//perfect performance par 3 stars
        {
            stars = 3;
        }
        else if (stepsTaken <= (int)(1.25f * optimalSteps) && penalties <= 1)//achhi performance par 2 stars
        {
            stars = 2;
        }
        else//normal performance par 1 star
        {
            stars = 1;
        }
        return stars;
    }
};