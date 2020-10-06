#pragma once

#include "Error.h"
#include "Resource.h"

#include <memory>
#include <unordered_map>

/**
 * This is your brain on C++ templates...
 *
 * The resource manager provides a common access pattern for large memory game resources (e.g.
 * texture files, shaders, object meshes etc.). It contains a map linking human-readable names
 * string to a weak pointer for that resource. The class is templated so that each class can have
 * its own resource map to access. In addition, the templates only allow classes inheriting from the
 * Resource class to make use of ResourceManager - this is intended only for large memory resources
 * and not as a general-purpose object manager.
 *
 * Accessing objects can be done with the Find method, which will attempt to locate a given resource
 * by name and check that it is still alive. If successful, the resource will be returned in the
 * item out parameter. A bool return value is also returned to indicate the success of the
 * operation:
 *
 *     std::shared_ptr<Texture> texture;
 *     bool success = ResourceManager<Texture>::Find("img01.png", texture);
 *
 * A variant of this method (FindOrError) works similarly to Find, but will instead return an error
 * code determining whether the resource location attempt was successful.
 *
 * The Create method can be used to create new resources. As with Find, a name parameter is passed
 * along with a variadic set of parameters that are forwarded to the object's constructor. If a name
 * entry for the map already exists, the Create method will replace it. The method will also return
 * a shared pointer to the newly created resource:
 *
 *     std::shared_ptr<Mesh> mesh =      [  name  ]  [        additional arguments        ]
 *         ResourceManager<Mesh>::Create("shader01", vertices, normals, texCoords, indices);
 *
 * The Get method is similar to Create. A key difference is that it will attempt to locate and
 * return an existing resource, only creating a new one if not found. It can be used in the same way
 * as create.
 */

template <class T, typename = std::enable_if_t<std::is_base_of<Resource, T>::value>>
using ResourceMap = std::unordered_map<std::string, std::weak_ptr<T>>;

template <class T, typename = std::enable_if_t<std::is_base_of<Resource, T>::value>>
class ResourceManager {
public:
  // Find resource by name, return true if successful, false otherwise.
  static bool Find(const std::string &name, std::shared_ptr<T> &item) {
    auto it = m_resourceMap.find(name);
    if (it == m_resourceMap.end())
      return false;

    auto foundItem = it->second;
    if (!foundItem.expired())
      item = foundItem.lock();

    return !foundItem.expired();
  }

  // Finds resource by name, returns appropriate error code if successful or not.
  static ERROR FindOrError(const std::string &name, std::shared_ptr<T> &item) {
    ERROR errCode = Find(name, item) ? ERROR_OK : ERROR_RESOURCE_NOT_FOUND;
    return printErrorMsg(errCode, name.c_str());
  }

  // Create a new resource and add to the map.
  template <class... Args> static std::shared_ptr<T> Create(const std::string &name, Args... args) {
    if (name.empty())
      return nullptr;

    auto &item = m_resourceMap[name];
    std::shared_ptr<T> newItem(new T(name, std::forward<Args>(args)...));
    item = newItem;
    return newItem;
  }

  // Try locating an existing resource first, create if not found.
  template <class... Args> static std::shared_ptr<T> Get(const std::string &name, Args... args) {
    auto it = m_resourceMap.find(name);
    if (it != m_resourceMap.end() && !it->second.expired())
      return it->second.lock();

    return Create(name, args...);
  }

private:
  static inline ResourceMap<T> m_resourceMap;
};