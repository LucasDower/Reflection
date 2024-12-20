#pragma once

#include "RClass.hpp"
#include "RObject.hpp"

#define OFFSET_OF(Type, Member) ((size_t) &(((Type*)0)->Member))

#define GET_CLASS_PROPERTY(ClassName, PropertyName) \
	((void)sizeof(ClassName::PropertyName), #PropertyName)

#define REFLECTION_CLASS_BEGIN(ClassName, SuperName) \
	public: \
		using SELF = ClassName; \
		using SUPER = SuperName; \
		class Class : public RClass \
		{ \
			friend SELF; \
			Class() \
			{ \
				InitProperties(); \
			} \
		public: \
			static const SUPER::Class& StaticGetParent() \
			{ \
				return SUPER::StaticGetClass(); \
			} \
			virtual const RClass* GetParent() const override \
			{ \
				return &StaticGetParent(); \
			} \
			virtual std::string GetName() const override \
			{ \
				return #ClassName; \
			} \

#define REFLECTION_CLASS_END \
	}; \
	static SELF::Class& StaticGetClass() \
	{ \
		static SELF::Class s_Class; \
		return s_Class; \
	} \
	virtual const RClass& GetClass() const override \
	{ \
		return StaticGetClass(); \
	} \

#define REFLECTION_PROPERTIES_BEGIN \
	protected: \
		virtual void InitProperties() override \
		{ \
			for (const RProperty& Property : SUPER::StaticGetClass().GetProperties()) \
			{ \
				m_Properties.push_back(Property); \
			} \

#define REFLECTION_PROPERTY(PropertyName) \
	m_Properties.push_back(RProperty(#PropertyName, RProperty::StaticGetDataType<decltype(PropertyName)>(), sizeof(PropertyName), OFFSET_OF(SELF, PropertyName))); \

#define REFLECTION_PROPERTIES_END \
	} \

#define REFLECTION_CLASS_REGISTER(ClassName) \
	namespace \
	{ \
		struct ClassName##Register \
		{ \
			ClassName##Register() \
			{ \
				RClassRegistry::Instance().Add(#ClassName, []() -> void* { return new ClassName(); }); \
			} \
		}; \
		static ClassName##Register s_ClassName##Register; \
	} \