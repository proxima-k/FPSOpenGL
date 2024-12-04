#pragma once

#include <functional>
#include <iostream>

class Timer 
{
public:
	Timer() : inactive(false), currentTime(0) {}
    ~Timer() { }

    // constanty decrease the timer and when it reaches 0 call the callback
    void updateTimer(float deltaTime) {

        if (!inactive) {
            currentTime -= deltaTime;
        }

        if (isZero()) {
            if (callback) {
                callback();
            }
        }
    }

    void setCallback(std::function<void()> cb) 
    {
        callback = cb;
    }

	void setTimer(int time) { currentTime = time; }

    bool isZero() { return currentTime <= 0; }

	void startTimer() { inactive = false; }
	void stopTimer() { inactive = true; }

private:
	bool inactive = false;
	float currentTime = 0;

    std::function<void()> callback;
};