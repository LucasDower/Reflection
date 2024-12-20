#pragma once

#include "RCore.hpp"

#include <string>
#include <stdint.h>

class RObject;

class RProperty final
{
public:
	enum class Type
	{
		Bool,
		Char,
		Short,
		Int,
		Long,
		LongLong,
		UnsignedChar,
		UnsignedShort,
		UnsignedInt,
		UnsignedLong,
		UnsignedLongLong,
		Float,
		Double,
		RObject,
	};

	template <typename T>
	static constexpr Type StaticGetDataType()
	{
		if constexpr (std::is_same<T, bool>())
		{
			return Type::Bool;
		}
		else if constexpr (std::is_same<T, char>())
		{
			return Type::Char;
		}
		else if constexpr (std::is_same<T, short>())
		{
			return Type::Short;
		}
		else if constexpr (std::is_same<T, int>())
		{
			return Type::Int;
		}
		else if constexpr (std::is_same<T, long>())
		{
			return Type::Long;
		}
		else if constexpr (std::is_same<T, long long>())
		{
			return Type::LongLong;
		}
		else if constexpr (std::is_same<T, unsigned char>())
		{
			return Type::UnsignedChar;
		}
		else if constexpr (std::is_same<T, unsigned short>())
		{
			return Type::UnsignedShort;
		}
		else if constexpr (std::is_same<T, unsigned int>())
		{
			return Type::UnsignedInt;
		}
		else if constexpr (std::is_same<T, unsigned long>())
		{
			return Type::UnsignedLong;
		}
		else if constexpr (std::is_same<T, unsigned long long>())
		{
			return Type::UnsignedLongLong;
		}
		else if constexpr (std::is_same<T, float>())
		{
			return Type::Float;
		}
		else if constexpr (std::is_same<T, double>())
		{
			return Type::Double;
		}
		else if constexpr (std::is_base_of<RObject, T>())
		{
			return Type::RObject;
		}
		else
		{
			static_assert(std::is_same_v<T, void>, "Unsupported type for reflection!");
		}
	}

	static std::string StaticGetDataTypeString(const Type InType)
	{
		if (InType == Type::Bool)
		{
			return "Bool";
		}
		else if (InType == Type::Char)
		{
			return "Char";
		}
		else if (InType == Type::Short)
		{
			return "Short";
		}
		else if (InType == Type::Int)
		{
			return "Int";
		}
		else if (InType == Type::Long)
		{
			return "Long";
		}
		else if (InType == Type::LongLong)
		{
			return "Long";
		}
		else if (InType == Type::UnsignedChar)
		{
			return "UnsignedChar";
		}
		else if (InType == Type::UnsignedShort)
		{
			return "UnsignedShort";
		}
		else if (InType == Type::UnsignedInt)
		{
			return "UnsignedInt";
		}
		else if (InType == Type::UnsignedLong)
		{
			return "UnsignedLong";
		}
		else if (InType == Type::UnsignedLongLong)
		{
			return "UnsignedLong";
		}
		else if (InType == Type::Float)
		{
			return "Float";
		}
		else if (InType == Type::Double)
		{
			return "Double";
		}
		else if (InType == Type::RObject)
		{
			return "RObject";
		}
		else
		{
			REFLECTION_ASSERT(false);
			return "Invalid";
		}
	}

	RProperty(const std::string InName, const Type InType, const size_t InSize, const size_t InOffset)
		: m_Name(InName)
		, m_Type(InType)
		, m_Size(InSize)
		, m_Offset(InOffset)
	{
	}

	const std::string& GetName() const
	{
		return m_Name;
	}

	Type GetType() const
	{
		return m_Type;
	}

	size_t GetOffset() const
	{
		return m_Offset;
	}

	bool operator==(const RProperty& Other) const {
		return m_Name == Other.m_Name && m_Type == Other.m_Type && m_Size == Other.m_Size && m_Offset == Other.m_Offset;
	}

private:
	std::string m_Name;
	Type m_Type;
	size_t m_Size;
	size_t m_Offset;
};