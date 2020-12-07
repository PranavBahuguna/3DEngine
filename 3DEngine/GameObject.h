#pragma once

#include "Component.h"
#include "Error.h"
#include "Shader.h"

#include <memory>
#include <vector>

class Component;

class GameObject {
public:
  GameObject();

  void init(ERROR &errCode);
  void start(ERROR &errCode);
  void update(ERROR &errCode);
  void draw(ERROR &errCode, const Shader &shader);

  void setActive(bool value);
  void setVisible(bool value);
  bool isActive() const;
  bool isVisible() const;

  template <class T, std::enable_if_t<std::is_base_of<Component, T>::value, bool> = true,
            class... Args>
  T *AddComponent(Args... args) {
    // Check that this component has not already been added to this game object. This is to prevent
    // the same component from being added twice.
    for (auto &component : m_components) {
      T *foundComponent = dynamic_cast<T *>(component.get());

      if (foundComponent != nullptr)
        return foundComponent;
    }

    // Game object does not already have this component, so add it
    m_components.emplace_back(std::make_unique<T>(std::shared_ptr<GameObject>(this), std::forward<Args>(args)...));

    return dynamic_cast<T*>(m_components.back().get());
  }

  template <class T, std::enable_if_t<std::is_base_of<Component, T>::value, bool> = true>
  T *GetComponent() {
    // Check if we already have this component, and return it if found
    for (auto &component : m_components) {
      T *foundComponent = dynamic_cast<T *>(component.get());

      if (foundComponent != nullptr)
        return foundComponent;
    }

    return nullptr; // component not found
  }

private:
  bool m_isActive;
  bool m_isVisible;

  std::vector<std::unique_ptr<Component>> m_components;
};