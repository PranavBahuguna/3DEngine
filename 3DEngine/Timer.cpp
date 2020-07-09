#include "Timer.h"

static float deltaTime = 0.0f;
static float lastTime = 0.0f;

void Timer::Update() {
  float timeNow = static_cast<float>(glfwGetTime());
  deltaTime = timeNow - lastTime;
  lastTime = timeNow;
}

float Timer::GetDeltaTime() { return deltaTime; }