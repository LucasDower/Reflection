![](Resources/reflection.gif)

# Reflection

This is a prototype reflection system built for C++ that supports member variable introspection. It is header-only and requires no extra external tooling.

# Usage

Note: The included images show an example application how a reflection system can be visualised in a GUI. This is not included in the base library inside `/Include` but can be viewed inside `/Examples

Adding reflection to a class is easy. Just add the following macros and ensure your class structure inherits from `RObject`.

```cpp
#include "Reflection.hpp"

class RPlayer : public RObject
{
    REFLECTION_CLASS_BEGIN(RPlayer, RObject)
        REFLECTION_PROPERTIES_BEGIN
            REFLECTION_PROPERTY(m_Health)
            REFLECTION_PROPERTY(m_Mana)
            REFLECTION_PROPERTY(m_Level)
            REFLECTION_PROPERTY(m_Experience)
        REFLECTION_PROPERTIES_END
    REFLECTION_CLASS_END

public:
    int m_Health = 100;
    int m_Mana = 200;
};
```

`REFLECTION_CLASS_BEGIN` expects the first argument to be the name of the class being reflected and the second argument as the parent `RClass`. Properties that should be reflected require the corresponding `REFLECTION_PROPERTY`.

All classes with reflection steup have include a static `RClass` accessible statically with `RClass::StaticGetClass()` or dynamically with `GetClass()`.

```cpp
// Statically (RPlayer::Class is derived from RClass)
RPlayer::Class& PlayerClass = RPlayer::StaticGetClass();
// or

// Dynamically
std::shared_ptr<RClass> SomeObject = std::make_shared<RPlayer>();
const RClass& SomeClass = SomeObject->GetClass();
```

You can query the inheritance structure with `GetParent()`:

```cpp
static_assert(RPlayer::StaticGetClass().GetParent() == RClass::StaticGetClass());
```

An RObject's class can be compared with `IsA<T>()`, for example:

```cpp
if (SomeClass.GetClass().IsA<RPlayer>())
{
    std::shared_ptr<RPlayer> Player = std::static_pointer_cast<RPlayer>(SomeObject);
    std::cout << "PlayerObject is a RPlayer object, it's health is " << Player._MHealth << std::endl;
}
```

You can inspect a class's properties statically or dynamically with `RClass::StaticGetProperties()` or `SomeClass.GetProperties()` respectively. Once you have a class's set of properties, you can identify types and get/set values.

```cpp
// Static
for (const RProperty& Prop : RPlayer::StaticGetClass().StaticGetProperties())
{
    std::cout << Prop.name << "\n";
    std::cout << "  type = " << Prop.type << "\n";
    std::cout << "  size = " << Prop.size << "\n";
    std::cout << "  offset = " << Prop.offset << "\n";
}

// Dynamic
std::unique_ptr<RPlayer> Player = std::make_unique<RPlayer>();
const std::vector<RProperty>& Properties = Player->GetClass().GetProperties();
```

```cpp
std::unique_ptr<RPlayer> Player = std::make_unique<RPlayer>();
const RProperty& HealthProperty = Player->GetClass().GetPropertyChecked("m_Health");
Player->GetClass().SetPropertyValueChecked(*Player, HealthProperty, 0);
```

Currently there is reflection support for the following data types, (though this is trivial to expand):

* Bool,
* Char,
* Short,
* Int,
* Long,
* LongLong,
* UnsignedChar,
* UnsignedShort,
* UnsignedInt,
* UnsignedLong,
* UnsignedLongLong,
* Float,
* Double

Reflected classes can be derived from other RObjects, for example:

```cpp
class RPlayerWithLevels : public RObject
{
    REFLECTION_CLASS_BEGIN(RPlayerWithLevels, RPlayer)
        REFLECTION_PROPERTIES_BEGIN
            REFLECTION_PROPERTY(m_Level)
            REFLECTION_PROPERTY(m_Experience)
        REFLECTION_PROPERTIES_END
    REFLECTION_CLASS_END

public:
    int m_Level = 0;
    float m_Experience = 0.0f;
};
```