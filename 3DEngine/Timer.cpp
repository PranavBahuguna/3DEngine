#include "Timer.h"

static float _deltaTime = 0.0f;
static float _lastTime = 0.0f;

void Timer::Update() {
  float timeNow = static_cast<float>(glfwGetTime());
  _deltaTime = timeNow - _lastTime;
  _lastTime = timeNow;
}

float Timer::GetDeltaTime() { return _deltaTime; }