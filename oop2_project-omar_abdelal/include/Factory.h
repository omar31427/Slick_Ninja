#pragma once

#include "GameObject.h"
#include "Resources.h"

#include <SFML/Graphics.hpp>
#include <memory>
#include <unordered_map>



template <typename T>
class Factory
{
public:
    static Factory& instance()
    {
        static auto instance = Factory();
        return instance;
    }

    using FuncType = std::shared_ptr<T>(*)(sf::Vector2f position);
    bool registerType(obj_types t, FuncType f)
    {

        m_map.emplace(t, f);
        return true;
    }

    std::shared_ptr<T> create(obj_types t, sf::Vector2f position) const
    {
        if (!m_map.contains(t)) 
        {
            return nullptr;
        }

        return m_map.at(t)(position);
    }

private:
    Factory() = default;
    Factory(const Factory&) = delete;
    void operator=(const Factory&) = delete;

    std::unordered_map<obj_types, FuncType> m_map;
};
