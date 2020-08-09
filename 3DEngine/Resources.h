#pragma once

#include <memory>
#include <unordered_map>

template <class T> using ResourceMap = std::unordered_map<std::string, std::weak_ptr<T>>;

template <class T> class Resources {
public:
  static bool Find(const std::string &name, std::shared_ptr<T> &item) {
    auto it = m_resourceMap.find(name);
    if (it == m_resourceMap.end())
      return false;

    auto foundItem = it->second;
    if (!foundItem.expired())
      item = foundItem.lock();

    return !foundItem.expired();
  }

  template <class... Args> static std::shared_ptr<T> Create(const std::string &name, Args... args) {
    auto &item = m_resourceMap[name];
    std::shared_ptr<T> newItem(new T(std::forward<Args>(args)...));
    item = newItem;
    return newItem;
  }

  template <class... Args> static std::shared_ptr<T> Get(const std::string &name, Args... args) {
    auto it = m_resourceMap.find(name);
    if (it != m_resourceMap.end() && !it->second.expired())
      return it->second.lock();

    return Create(name, args...);
  }

private:
  static inline ResourceMap<T> m_resourceMap;
};