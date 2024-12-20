#pragma once

#include "RCore.hpp"
#include "RProperty.hpp"

#include <vector>
#include <functional>
#include <unordered_map>
#include <iostream>
#include <assert.h>

class RObject;

class RClass
{
public:
    template <typename T>
    bool IsA() const
    {
        return T::StaticGetClass().GetName() == GetName();
    }

	RClass()
	{
		InitProperties();
	}

    bool HasProperty(const RProperty& Property) const
    {
        for (const RProperty& ClassProperty : m_Properties)
        {
            if (Property == ClassProperty)
            {
                return true;
            }
        }

        return false;
    }

	virtual const RClass* GetParent() const = 0;

	virtual std::string GetName() const = 0;

    template <typename T>
    bool SetPropertyValue(RObject& Object, const RProperty& Property, const T& Value) const;

    template <typename T>
    void SetPropertyValueChecked(RObject& Object, const RProperty& Property, const T& Value) const;

    template <typename T>
    bool SetPropertyValue(RObject& Object, const std::string& PropertyName, const T& Value) const;

    template <typename T>
    void SetPropertyValueChecked(RObject& Object, const std::string& PropertyName, const T& Value) const;

    template <typename T>
    const T* GetPropertyValue(const RObject& Object, const RProperty& Property) const;

    template <typename T>
    const T& GetPropertyValueChecked(const RObject& Object, const RProperty& Property) const;

    template <typename T>
    const T* GetPropertyValue(const RObject& Object, const std::string& PropertyName) const;

    template <typename T>
    const T& GetPropertyValueChecked(const RObject& Object, const std::string& PropertyName) const;

    template <typename T>
    T* GetMutablePropertyValue(RObject& Object, const RProperty& Property) const;

    template <typename T>
    T& GetMutablePropertyValueChecked(RObject& Object, const RProperty& Property) const;

    template <typename T>
    T* GetMutablePropertyValue(RObject& Object, const std::string& PropertyName) const;

    template <typename T>
    T& GetMutablePropertyValueChecked(RObject& Object, const std::string& PropertyName) const;

    const RProperty* GetProperty(const std::string& PropertyName) const
    {
        for (const RProperty& Property : m_Properties)
        {
            if (Property.GetName() == PropertyName)
            {
                return &Property;
            }
        }

        return nullptr;
    }

    const RProperty& GetPropertyChecked(const std::string& PropertyName) const
    {
        const RProperty* Property = GetProperty(PropertyName);
        REFLECTION_ASSERT_MSG(Property, "Attempted to get property '" + PropertyName + "' which does not exist on class '" + GetName() + "'");
        return *Property;
    }

	const std::vector<RProperty>& GetProperties() const
	{
		return m_Properties;
	}

protected:
	virtual void InitProperties() {};

	std::vector<RProperty> m_Properties;
};

#include "RObject.hpp"

template <typename T>
bool RClass::SetPropertyValue(RObject& Object, const RProperty& Property, const T& Value) const
{
    T* MutableValue = GetMutablePropertyValue(Object, Property);
    if (MutableValue)
    {
        *MutableValue = Value;
        return true;
    }
    return false;
}

template <typename T>
void RClass::SetPropertyValueChecked(RObject& Object, const RProperty& Property, const T& Value) const
{
    T& MutableValue = GetMutablePropertyValueChecked<T>(Object, Property);
    *MutableValue = Value;
}

template <typename T>
bool RClass::SetPropertyValue(RObject& Object, const std::string& PropertyName, const T& Value) const
{
    T* MutableValue = GetMutablePropertyValue(Object, PropertyName);
    if (MutableValue)
    {
        *T = Value;
        return true;
    }
    return false;
}

template <typename T>
void RClass::SetPropertyValueChecked(RObject& Object, const std::string& PropertyName, const T& Value) const
{
    T& MutableValue = GetMutablePropertyValueChecked<T>(Object, PropertyName);
    MutableValue = Value;
}

template <typename T>
const T* RClass::GetPropertyValue(const RObject& Object, const RProperty& Property) const
{
    if (Object.GetClass().HasProperty(Property))
    {
        if (RProperty::StaticGetDataType<T>() == Property.GetType())
        {
            void* PropertyData = (char*)&Object + Property.GetOffset();
            return static_cast<const T*>(PropertyData);
        }
    }
    return nullptr;
}

template <typename T>
const T& RClass::GetPropertyValueChecked(const RObject& Object, const RProperty& Property) const
{
    REFLECTION_ASSERT(Object.GetClass().HasProperty(Property), "Property " + " does not exist on " + RObject.GetClass().GetName());
    REFLECTION_ASSERT_MSG(RProperty::StaticGetDataType<T>() == Property.GetType(), "Incorrect template type, expected " + RProperty::StaticGetDataTypeString(Property.GetType()) + ", got " + RProperty::StaticGetDataTypeString(RProperty::StaticGetDataType<T>()));
    void* PropertyData = (char*)&Object + Property.GetOffset();
    return *static_cast<const T*>(PropertyData);
}

template <typename T>
const T* RClass::GetPropertyValue(const RObject& Object, const std::string& PropertyName) const
{
    if (const RProperty* Property = GetProperty(PropertyName))
    {
        return GetPropertyValue<T>(Object, *Property);
    }
    return nullptr;
}

template <typename T>
const T& RClass::GetPropertyValueChecked(const RObject& Object, const std::string& PropertyName) const
{
    const RProperty& Property = GetPropertyChecked(PropertyName);
    return GetPropertyValueChecked<T>(Object, Property);
}

template <typename T>
T* RClass::GetMutablePropertyValue(RObject& Object, const RProperty& Property) const
{
    if (Object.GetClass().HasProperty(Property))
    {
        if (RProperty::StaticGetDataType<T>() == Property.GetType())
        {
            void* PropertyData = (char*)&Object + Property.GetOffset();
            return *static_cast<T*>(PropertyData);
        }
    }
    return nullptr;
}

template <typename T> 
T& RClass::GetMutablePropertyValueChecked(RObject& Object, const RProperty& Property) const
{
    REFLECTION_ASSERT(Object.GetClass().HasProperty(Property));
    REFLECTION_ASSERT(RProperty::StaticGetDataType<T>() == Property.GetType());
    void* PropertyData = (char*)&Object + Property.GetOffset();
    return *static_cast<T*>(PropertyData);
}

template <typename T>
T* RClass::GetMutablePropertyValue(RObject& Object, const std::string& PropertyName) const
{
    if (const RProperty* Property = GetProperty(PropertyName))
    {
        if (RProperty::StaticGetDataType<T>() == Property.GetType())
        {
            return GetMutablePropertyValue<T>(Object, Property);
        }
    }
    return nullptr;
}

template <typename T>
T& RClass::GetMutablePropertyValueChecked(RObject& Object, const std::string& PropertyName) const
{
    const RProperty& Property = GetPropertyChecked(PropertyName);
    REFLECTION_ASSERT(RProperty::StaticGetDataType<T>() == Property.GetType());
    return GetMutablePropertyValue<T>(Object, Property);
}

class RClassRegistry
{
public:
    using Creator = std::function<void* ()>;

    static RClassRegistry& Instance()
    {
        static RClassRegistry instance;
        return instance;
    }

    void Add(const std::string& className, Creator creator) {
        registry_[className] = creator;
    }

    void* Create(const std::string& className) {
        if (registry_.find(className) != registry_.end()) {
            return registry_[className]();
        }
        return nullptr;
    }

    void PrintRegisteredClasses() {
        for (const auto& [className, _] : registry_) {
            std::cout << "Registered Class: " << className << std::endl;
        }
    }

private:
    std::unordered_map<std::string, Creator> registry_;
};