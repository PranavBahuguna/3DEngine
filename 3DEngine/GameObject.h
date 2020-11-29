#pragma once

#include "Component.h"
#include "Error.h"

#include <memory>
#include <vector>

class Component;

class GameObject {
public:
  void init(ERROR &errCode);
  void start(ERROR &errCode);
  void update(ERROR &errCode);

  template <class T, std::enable_if_t<std::is_base_of<Component, T>::value, bool> = true,
            class... Args>
  std::shared_ptr<T> AddComponent(Args... args) {
    // Check that this component has not already been added to this game object. This is to prevent
    // the same component from being added twice.
    for (auto &component : m_components) {
      if (std::dynamic_pointer_cast<T>(component))
        return std::dynamic_pointer_cast<T>(component);
    }

    // Game object does not already have this component, so add it
    std::shared_ptr<T> newComponent(
        new T(std::make_shared<GameObject>(*this), std::forward<Args>(args)...));
    m_components.push_back(newComponent);

    return newComponent;
  }

  template <class T, std::enable_if_t<std::is_base_of<Component, T>::value, bool> = true>
  std::shared_ptr<T> GetComponent() {
    // Check if we already have this component, and return it if found
    for (auto &component : m_components) {
      if (std::dynamic_pointer_cast<T>(component))
        return std::dynamic_pointer_cast<T>(component);
    }

    return nullptr;
  }

  template <class T, std::enable_if_t<std::is_base_of<Component, T>::value, bool> = true>
  T &GetComponentNoPtr() {
    return *GetComponent<T>();
  }

private:
  std::vector<std::shared_ptr<Component>> m_components;
};