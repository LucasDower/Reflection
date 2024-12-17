#pragma once

#include <string>
#include <vector>

enum class EDataType
{
	UnsignedChar,
	Int,
	Boolean,
	Float,
	RObject,
	RObjectPointer,
	RObjectVector,
};

struct RProperty
{
	std::string Name;
	EDataType Type;
	size_t Offset;
	size_t Size;
};

class RClassProperties
{
public:
	void Add(const RProperty& Prop)
	{
		m_Variables.push_back(Prop);
	}

	void Add(const char* VariableName, const EDataType Type, const size_t Offset, const size_t Size)
	{
		RProperty Property;
		Property.Name = VariableName;
		Property.Type = Type;
		Property.Offset = Offset;
		Property.Size = Size;
		m_Variables.push_back(Property);
	}

	bool IsEmpty() const
	{
		return m_Variables.size() == 0;
	}

	auto begin() { return m_Variables.begin(); }
	auto end() { return m_Variables.end(); }
	auto begin() const { return m_Variables.begin(); }
	auto end() const { return m_Variables.end(); }

private:
	std::vector<RProperty> m_Variables;
};

class RClass;

template <typename T>
constexpr EDataType GetDataType()
{
	if constexpr (std::is_same<T, unsigned char>())
	{
		return EDataType::UnsignedChar;
	}
	else if constexpr (std::is_same<T, int>())
	{
		return EDataType::Int;
	}
	else if constexpr (std::is_same<T, bool>())
	{
		return EDataType::Boolean;
	}
	else if constexpr (std::is_same<T, float>())
	{
		return EDataType::Float;
	}
	else if constexpr (std::is_base_of<RClass, T>())
	{
		return EDataType::RObject;
	}
	else if constexpr (std::is_base_of<RClass, std::remove_pointer_t<T>>())
	{
		return EDataType::RObjectPointer;
	}
	else if constexpr (std::is_same<T, std::vector<typename T::value_type>>())
	{
		return EDataType::RObjectVector;
	}
	else
	{
		static_assert(std::is_same_v<T, void>, "Unsupported type for reflection!");
	}
}

#define OFFSET_OF(type, member) ((size_t) &(((type *)0)->member))


#define BEGIN_REFLECTED_PROPERTIES \
	static const RClassProperties& StaticGetProperties() \
	{ \
		static RClassProperties RC; \
		if (RC.IsEmpty()) \
		{ \
			for (const RProperty& SuperProp : SUPER::StaticGetProperties()) \
			{ \
				RC.Add(SuperProp); \
			} \

#define REFLECTED_PROPERTY(VariableName) \
	RC.Add(#VariableName, GetDataType<decltype(VariableName)>(), OFFSET_OF(SELF, VariableName), sizeof(VariableName)); \

#define END_REFLECTED_PROPERTIES \
		} \
		return RC; \
	}