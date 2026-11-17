#pragma once
#include <iostream>
#include <cmath>

using namespace std;

class ScoreSystem {
public:
    int currentAlarm;
    int maxAlarm;
    int optimalSteps;
    int stepsTaken;
    int penalties;
    int score;
    int stars;

    ScoreSystem() {
        reset(5);
    }

    void reset(int optimal) {
        optimalSteps = optimal;
        // Formula strictly from PDF Section 2.2: ceil(1.5 * optimal) + 2
        maxAlarm = (int)ceil(1.5f * optimal) + 2;
        currentAlarm = 0;
        stepsTaken = 0;
        penalties = 0;
        score = 1000;
        stars = 0;
    }

    void addStep() {
        stepsTaken++;
        currentAlarm++;
        score -= 10;
        if (score < 0) score = 0;
    }

    void addPenalty(int alarmCost, int pointCost) {
        penalties++;
        currentAlarm += alarmCost;
        score -= pointCost;
        if (score < 0) score = 0;
    }

    bool isAlarmFull() {
        return currentAlarm >= maxAlarm;
    }

    // Star calculation strictly from PDF Section 2.4
    int calculateStars() {
        if (isAlarmFull()) {
            stars = 0;
        }
        else if (stepsTaken <= optimalSteps && penalties == 0) {
            stars = 3;
        }
        else if (stepsTaken <= (int)(1.25f * optimalSteps) && penalties <= 1) {
            stars = 2;
        }
        else {
            stars = 1;
        }
        return stars;
    }
};