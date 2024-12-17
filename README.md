![](Resources/reflection.gif)

# Reflection

This is a prototype reflection system built for C++ that supports member variable introspection. It is header-only and requires no extra external tooling.

# Usage

Adding reflection to a class is easy. Just add the following macros and ensure your class structure inherits from `RClass`.

```cpp
#include "RClass.hpp"

class RAsteroid : public RClass
{
    REFLECTED_CLASS(RAsteroid, RClass)

public:
    BEGIN_REFLECTED_PROPERTIES
        REFLECTED_PROPERTY(m_fX)
        REFLECTED_PROPERTY(m_fY)
        REFLECTED_PROPERTY(m_bContainsOres)
        REFLECTED_PROPERTY(m_iHealth)
    END_REFLECTED_PROPERTIES

    float m_fX = 0.0f;
    float m_fY = 0.0f;
    bool m_bContainsOres = true;
    int m_iHealth = 100.0f;
};
```

`REFLECTED_CLASS` expects the first argument to be the name of the class being reflected and the second argument as the parent `RClass`.

You can inspect a reflected object's properties statically or dynamically with `RClass::StaticGetProperties()` or `MyRObject->GetProperties()` respectively.

```cpp
// Static
for (const RProperty& Prop : RAsteroid::StaticGetProperties())
{
    std::cout << Prop.name << "\n";
    std::cout << "  type = " << Prop.type << "\n";
    std::cout << "  size = " << Prop.size << "\n";
    std::cout << "  offset = " << Prop.offset << "\n";
}

// Dynamic
std::unique_ptr<RClass> Asteroid = std::make_unique<RAsteroid>();
for (const RProperty& Prop : Asteroid->GetProperties())
{
    std::string Value;

    if (Prop.Type == EDataType::Int)
    {
        const int& Value = Asteroid->GetPropertyValue<int>(Prop);
        Value = std::to_string(Value);
        std::cout << Prop.name << " (int) = " << Value << "\n";
    }
    else if (Prop.Type == EDataType::Bool)
    {
        const bool& Value = Asteroid->GetPropertyValue<bool>(Prop); 
        std::cout << Prop.name << " (bool) = " << Value << "\n";
    }
    else if (Prop.Type == EDataType::Float)
    {
        const float& Value = Asteroid->GetPropertyValue<float>(Prop); 
        std::cout << Prop.name << " (float) = " << Value << "\n";
    }
    // ...
}
```

Currently there is reflection support for the following data types, (though this is trivial to expand):
* `Unsigned Char`
* `Int`
* `Boolean`
* `Float`
* `RClass`
* `RClass*`
* `std:vector<RClass*>`

The reflection system also supports inheritance.

```cpp
class RVector2f : public RClass
{
    REFLECTED_CLASS(RAsteroid, RClass)

public:
    BEGIN_REFLECTED_PROPERTIES
        REFLECTED_PROPERTY(m_fX)
        REFLECTED_PROPERTY(m_fY)
    END_REFLECTED_PROPERTIES

    float m_fX = 0.0f;
    float m_fY = 0.0f;
};

class RVector3f : public RVector2f
{
    REFLECTED_CLASS(RVector3f, RVector2f)

public:
    BEGIN_REFLECTED_PROPERTIES
        REFLECTED_PROPERTY(m_fZ)
    END_REFLECTED_PROPERTIES

    float m_fZ = 0.0f;
};
```
