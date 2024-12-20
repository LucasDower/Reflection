#pragma once

#include "RClass.hpp"

class RObject
{
public:
	class Class : public RClass
	{
	public:
		virtual const RClass* GetParent() const
		{
			return nullptr;
		}

		virtual std::string GetName() const override
		{
			return "RObject";
		}
	};

	static RObject::Class& StaticGetClass()
	{
		static RObject::Class s_Class;
		return s_Class;
	}

	virtual const RClass& GetClass() const = 0;
};