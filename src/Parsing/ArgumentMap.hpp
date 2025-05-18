/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** ArgumentMap
*/

#pragma once

#include <unordered_map>
#include <any>
#include <iostream>

namespace RayTracer {
    class ArgumentMap {
        public:
            ArgumentMap();
            ~ArgumentMap();
            auto begin() { return _map.begin(); }
            auto end() { return _map.end(); }
            auto begin() const { return _map.begin(); }
            auto end() const { return _map.end(); }
            class Assigner {
                public:
                    template <typename T>
                    T &as() { return (std::any_cast<T&> (_val)); }

                    template <typename T>
                    bool exists() {return typeid(T) == _val.type(); }

                    const std::type_info & type() {
                        return _val.type();
                    }

                    Assigner(std::any &val) : _val(val) {}

                    template<typename T>
                    ArgumentMap::Assigner &operator=(T val)
                    {
                        _val = val;
                        return *this;
                    }
                private:
                    std::any &_val;
            };
            Assigner operator[](std::string key);
            std::unordered_map<std::string, std::any> getMap() {return _map;};
        private:
            std::unordered_map<std::string, std::any> _map;
    };
}
