#ifndef I_SYSTEM_H
#define I_SYSTEM_H

#include <typeindex>
#include <map>
#include <memory>
#include <LuaBridge\LuaBridge.h>
extern "C" {
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
}
#include <rapidjson/document.h>
#include "BB/World/Component/IComponent.h"

namespace bb {
  class ISystem {
  public:
    ISystem() {
    }
    ~ISystem() {
    }
    virtual IComponent* createTemplate(luabridge::LuaRef luaC) = 0;
    virtual IComponent* createComponent(IComponent* component, rapidjson::Value& jsonE, int id = -1) = 0;
    virtual void addComponent(IComponent* component, int id) = 0;
    virtual void removeComponent(int id) = 0;
    virtual bool update() = 0;
  protected:
    std::map<int, std::unique_ptr<IComponent>> m_components;
  };
}

#endif