#pragma once

#include <chrono>
#include <iostream>

class FPSCounter
{
  public:
    FPSCounter() : frameCount(0), lastTime(std::chrono::high_resolution_clock::now())
    {}

    void update()
    {
        frameCount++;
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> elapsed = currentTime - lastTime;

        if (elapsed.count() >= 1.0f) {
            fps = frameCount / elapsed.count();
            frameCount = 0;
            lastTime = currentTime;
        }
    }

    float getFPS() const
    {
        return fps;
    }

  private:
    int frameCount;
    float fps;
    std::chrono::high_resolution_clock::time_point lastTime;
};

FPSCounter fpsCounter;