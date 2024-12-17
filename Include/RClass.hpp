#pragma once

#include "RProperty.hpp"
#include <assert.h>

#define REFLECTED_CLASS(Self, Parent) \
	public: \
		using SELF = Self; \
		using SUPER = Parent; \
		virtual const RClassProperties& GetProperties() override \
		{ \
			return Self::StaticGetProperties(); \
		} \
		static const char* StaticGetName() \
		{ \
			return #Self; \
		} \
		virtual std::string GetName() override \
		{ \
			return #Self; \
		} \

class RClass
{
public:
	bool IsA(const char* ClassName)
	{
		return GetName() == ClassName;
	}

	template <typename T>
	bool IsA()
	{
		return T::StaticGetName() == GetName();
	}

	virtual std::string GetName()
	{
		return "RClass";
	}

	static const RClassProperties& StaticGetProperties() 
	{
		static RClassProperties Props;
		return Props;
	}

	virtual const RClassProperties& GetProperties() = 0;

	template <typename T>
	T& GetPropertyValue(const RProperty& Property)
	{
		void* PropertyData = (char*)this + Property.Offset;
		return *static_cast<T*>(PropertyData);
	}

	const RProperty* const GetProperty(const char* PropertyName)
	{
		const RClassProperties& Properties = GetProperties();

		for (const RProperty& Property : Properties)
		{
			if (Property.Name == PropertyName)
			{
				return &Property;
			}
		}
		return nullptr;
	}

	const RProperty& GetPropertyChecked(const char* PropertyName)
	{
		const RProperty* const Property = GetProperty(PropertyName);
		assert(Property);
		return *Property;
	}
};