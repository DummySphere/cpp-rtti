// TypeInfo.h, 2013-02-24 23:17:00
// TypeInfo - C++ non-intrusive RTTI Library
// Copyright (c) 2012-2013 Julien Duminil
// This file is released under the MIT License (http://opensource.org/licenses/MIT)

#ifndef __TYPEINFO_H__
#define __TYPEINFO_H__

#include <stddef.h>
#include "static_hash.h"
#include "va_macro.h"

namespace rtti
{
	class TypeInfo;
	template<typename T> const TypeInfo &GetTypeInfo();
	
	////////////////////////////////////////////////////////////////////////////////////////////////////
	// TypeInfo
	
	class TypeInfo
	{
	public:
		// name
		virtual const char *GetTypeName() const = 0;
		virtual const char *GetTypeFullName() const = 0;
		virtual const unsigned int GetTypeId() const = 0;
		// inheritance
		virtual unsigned int GetParentCount() const = 0;
		virtual const TypeInfo *GetParentType(unsigned int _parentIndex) const = 0;
		virtual void *Cast(void *_ptr, const TypeInfo &_ancestor) const = 0;
		// inheritance helpers
		virtual bool Inherit(const TypeInfo &_ancestor) const
		{
			for(unsigned int i = 0; i < GetParentCount(); ++i)
				if(GetParentType(i)->IsA(_ancestor))
					return true;
			return false;
		}
		template<typename T> inline bool Inherit() const { return Inherit(GetTypeInfo<T>()); }
		inline bool IsA(const TypeInfo &_ancestor) const { return (_ancestor == *this) || Inherit(_ancestor); }
		template<typename T> inline bool IsA() const { return IsA(GetTypeInfo<T>()); }
		// size
		virtual size_t GetSizeOf() = 0;
		// operators
		inline bool operator ==(const TypeInfo &_otherType) const { return _otherType.GetTypeId() == GetTypeId(); } // TODO: check != 0 ? assert size ?
		inline bool operator <(const TypeInfo &_otherType) const { return _otherType.GetTypeId() < GetTypeId(); }
	};
	
	////////////////////////////////////////////////////////////////////////////////////////////////////
	// StaticTypeInfo template
	
	template<typename T>
	class StaticTypeInfo : public TypeInfo
	{
	public:
		// name
		virtual inline const char *GetTypeName() const { return s_typeName; }
		virtual inline const char *GetTypeFullName() const { return s_typeFullName; }
		virtual inline const unsigned int GetTypeId() const { return s_typeId; }
		// inheritance
		virtual inline unsigned int GetParentCount() const { return s_parentCount; }
		virtual inline const TypeInfo *GetParentType(unsigned int _parentIndex) const { return (_parentIndex < s_parentCount) ? s_parentType[_parentIndex] : NULL; }
		virtual void *Cast(void *_ptr, const TypeInfo &_ancestor) const
		{
			if(_ancestor == *this)
				return _ptr;
			for(unsigned int i = 0; i < s_parentCount; ++i)
			{
				void *parent = s_parentType[i]->Cast(_ptr, _ancestor);
				if(parent != NULL)
					return parent;
			}
			return NULL;
		}
		// inheritance helpers
		virtual bool Inherit(const TypeInfo &_ancestor) const
		{
			for(unsigned int i = 0; i < s_parentCount; ++i)
				if(s_parentType[i]->IsA(_ancestor))
					return true;
			return false;
		}
		// size
		virtual inline size_t GetSizeOf() { return sizeof(T); }
		
	public:
		static const StaticTypeInfo<T> s_typeInfo;
		static const char *s_typeName;
		static const char *s_typeFullName;
		static const unsigned int s_typeId;
		static const unsigned int s_parentCount;
		static const TypeInfo **s_parentType;
		static void *(**s_parentCast)(void *);
	};
	
	template<typename T, typename U> void *CastFunc(void *_instance) { return (void *)static_cast<U *>((T *)_instance); }
	
	// template<typename T> const StaticTypeInfo<T> StaticTypeInfo<T>::s_typeInfo = StaticTypeInfo<T>();
	// template<typename T> const char *StaticTypeInfo<T>::s_typeName = "unknown type";
	// template<typename T> const char *StaticTypeInfo<T>::s_typeFullName = "unknown type";
	// template<typename T> const unsigned int StaticTypeInfo<T>::s_typeId = 0;
	// template<typename T> const unsigned int StaticTypeInfo<T>::s_parentCount = 0;
	// template<typename T> const TypeInfo **StaticTypeInfo<T>::s_parentType = NULL;
	// template<typename T> void *(**StaticTypeInfo<T>::s_parentCast)(void *) = NULL;

	// #define DECL_TYPE_INFO(_NAMESPACE, _CLASS)
		// template<> extern const StaticTypeInfo<_NAMESPACE::_CLASS> StaticTypeInfo<_NAMESPACE::_CLASS>::s_typeInfo;
		// template<> extern const char *StaticTypeInfo<T>::s_typeName;
		// template<> extern const char *StaticTypeInfo<T>::s_typeFullName;
		// template<> extern const unsigned int StaticTypeInfo<T>::s_typeId;
		// template<> extern const unsigned int StaticTypeInfo<T>::s_parentCount;
		// template<> extern const TypeInfo **StaticTypeInfo<T>::s_parentType;
		// template<> extern void *(**StaticTypeInfo<T>::s_parentCast)(void *);
	
	#define IMPL_TYPE_INFO(_NAMESPACE, _CLASS, ...)\
		template<> const rtti::StaticTypeInfo<_NAMESPACE::_CLASS> rtti::StaticTypeInfo<_NAMESPACE::_CLASS>::s_typeInfo = rtti::StaticTypeInfo<_NAMESPACE::_CLASS>();\
		template<> const char *rtti::StaticTypeInfo<_NAMESPACE::_CLASS>::s_typeName = #_CLASS;\
		template<> const char *rtti::StaticTypeInfo<_NAMESPACE::_CLASS>::s_typeFullName = #_NAMESPACE "::" #_CLASS;\
		template<> const unsigned int rtti::StaticTypeInfo<_NAMESPACE::_CLASS>::s_typeId = STATIC_HASH(hash_func::crc32, #_NAMESPACE "::" #_CLASS);\
		template<> const unsigned int rtti::StaticTypeInfo<_NAMESPACE::_CLASS>::s_parentCount = VA_ARGS_COUNT(__VA_ARGS__);\
		TOKEN_PASTING_SUBSTITUTION(IMPL_TYPE_INFO_INHERITANCE_, VA_ARGS_COUNT(__VA_ARGS__))(_NAMESPACE, _CLASS, (__VA_ARGS__))
	
	#define IMPL_TYPE_INFO_INHERITANCE_0(_NAMESPACE, _CLASS, _PARENT_LIST)\
		template<> const rtti::TypeInfo **rtti::StaticTypeInfo<_NAMESPACE::_CLASS>::s_parentType = NULL;\
		template<> void *(**rtti::StaticTypeInfo<_NAMESPACE::_CLASS>::s_parentCast)(void *) = NULL;

	#define IMPL_TYPE_INFO_INHERITANCE_1(_NAMESPACE, _CLASS, _PARENT_LIST)\
		namespace\
		{\
			const rtti::TypeInfo *_CLASS##_ParentType[1] = { &rtti::GetTypeInfo<VA_ARGS_1 _PARENT_LIST>() };\
			void *(*_CLASS##_ParentCast[1])(void *) = { &rtti::CastFunc<_NAMESPACE::_CLASS, VA_ARGS_1 _PARENT_LIST> };\
		}\
		template<> const rtti::TypeInfo **rtti::StaticTypeInfo<_NAMESPACE::_CLASS>::s_parentType = _CLASS##_ParentType;\
		template<> void *(**rtti::StaticTypeInfo<_NAMESPACE::_CLASS>::s_parentCast)(void *) = _CLASS##_ParentCast;

	#define IMPL_TYPE_INFO_INHERITANCE_2(_NAMESPACE, _CLASS, _PARENT_LIST)\
		namespace\
		{\
			const rtti::TypeInfo *_CLASS##_ParentType[2] = { &rtti::GetTypeInfo<VA_ARGS_1 _PARENT_LIST>(), &rtti::GetTypeInfo<VA_ARGS_2 _PARENT_LIST>() };\
			void *(*_CLASS##_ParentCast[2])(void *) = { &rtti::CastFunc<_NAMESPACE::_CLASS, VA_ARGS_1 _PARENT_LIST>, &rtti::CastFunc<_NAMESPACE::_CLASS, VA_ARGS_2 _PARENT_LIST> };\
		}\
		template<> const rtti::TypeInfo **rtti::StaticTypeInfo<_NAMESPACE::_CLASS>::s_parentType = _CLASS##_ParentType;\
		template<> void *(**rtti::StaticTypeInfo<_NAMESPACE::_CLASS>::s_parentCast)(void *) = _CLASS##_ParentCast;

	template<typename T> const TypeInfo &GetTypeInfo()
	{
		return StaticTypeInfo<T>::s_typeInfo;
	}
	
	////////////////////////////////////////////////////////////////////////////////////////////////////
	// Identifiable
	
	class Identifiable
	{
	public:
		// type
		virtual const TypeInfo &GetTypeInfo() const = 0;
		// name
		inline const char *GetTypeName() const { return GetTypeInfo().GetTypeName(); }
		inline const char *GetTypeFullName() const { return GetTypeInfo().GetTypeFullName(); }
		inline const unsigned int GetTypeId() const { return GetTypeInfo().GetTypeId(); }
		// inheritance
		inline bool IsA(const TypeInfo &_ancestor) const { return GetTypeInfo().IsA(_ancestor); }
		template<typename T> inline bool IsA() const { return GetTypeInfo().IsA<T>(); }
		// cast
		template<typename T> inline T *Cast() { return (T *)GetTypeInfo().Cast(GetInstancePtr(), rtti::GetTypeInfo<T>()); }
		template<typename T> inline const T *Cast() const { return (T *)GetTypeInfo().Cast(GetInstancePtr(), rtti::GetTypeInfo<T>()); }
		// template<typename T> inline operator T &() { return *Cast<T>(this); }
	protected:
		// instance
		virtual void *GetInstancePtr() = 0;
		virtual const void *GetInstancePtr() const = 0;
	};
	// template<typename T> inline operator T *(Identifiable *_identifiable) { return &(T &)*_identifiable; }
	
	#define DECL_IDENTIFIABLE()\
		virtual const rtti::TypeInfo &GetTypeInfo() const;\
		virtual void *GetInstancePtr();\
		virtual const void *GetInstancePtr() const;

	#define IMPL_IDENTIFIABLE(_NAMESPACE, _CLASS, ...)\
		IMPL_TYPE_INFO(_NAMESPACE, _CLASS, ##__VA_ARGS__)\
		const rtti::TypeInfo &_NAMESPACE::_CLASS::GetTypeInfo() const { return rtti::GetTypeInfo<_NAMESPACE::_CLASS>(); }\
		void *_NAMESPACE::_CLASS::GetInstancePtr() { return (void *)this; }\
		const void *_NAMESPACE::_CLASS::GetInstancePtr() const { return (const void *)this; }

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// Identifiable utils
	
	class IdentifiablePtr : public Identifiable
	{
	public:
		// constructor
		IdentifiablePtr(void *_ptr, const TypeInfo &_type) : m_ptr(_ptr), m_type(&_type) {}
		IdentifiablePtr(const IdentifiablePtr &_other) : m_ptr(_other.m_ptr), m_type(_other.m_type) {}
		template<typename T> IdentifiablePtr(T *_ptr = NULL) : m_ptr(_ptr), m_type(&rtti::GetTypeInfo<T>()) {}
		// setter
		inline void Set(void *_ptr, const TypeInfo &_type) { m_ptr = _ptr; m_type = &_type; }
		template<typename T> inline void Set(T *_ptr) { m_ptr = _ptr; m_type = &rtti::GetTypeInfo<T>(); }
		// type
		virtual inline const TypeInfo &GetTypeInfo() const { return *m_type; }
		// cast
		template<typename T> inline T *GetPtr() { return m_type->Cast(m_ptr, rtti::GetTypeInfo<T>()); }
		template<typename T> inline const T *GetPtr() const { return m_type->Cast(m_ptr, rtti::GetTypeInfo<T>()); }
		// operators
		inline IdentifiablePtr &operator =(const IdentifiablePtr &_other) { m_ptr = _other.m_ptr; m_type = _other.m_type; return *this; }
		template<typename T> inline IdentifiablePtr &operator =(T *_ptr) { m_ptr = _ptr; m_type = &rtti::GetTypeInfo<T>(); return *this; }
	protected:
		// instance
		virtual inline void *GetInstancePtr() { return m_ptr; }
		virtual inline const void *GetInstancePtr() const { return m_ptr; }
	private:
		// ptr
		void *m_ptr;
		const TypeInfo *m_type;
	};
	template<> inline void *IdentifiablePtr::GetPtr<void>() { return m_ptr; }
	template<> inline const void *IdentifiablePtr::GetPtr<void>() const { return m_ptr; }
	
	class IdentifiableConstPtr : public Identifiable
	{
	public:
		// constructor
		IdentifiableConstPtr(const void *_ptr, const TypeInfo &_type) : m_ptr(_ptr), m_type(&_type) {}
		IdentifiableConstPtr(const IdentifiableConstPtr &_other) : m_ptr(_other.m_ptr), m_type(_other.m_type) {}
		IdentifiableConstPtr(const IdentifiablePtr &_other) : m_ptr(_other.GetPtr<void>()), m_type(&_other.GetTypeInfo()) {}
		template<typename T> IdentifiableConstPtr(const T *_ptr = NULL) : m_ptr(_ptr), m_type(&rtti::GetTypeInfo<T>()) {}
		// setter
		inline void Set(const void *_ptr, const TypeInfo &_type) { m_ptr = _ptr; m_type = &_type; }
		template<typename T> inline void Set(const T *_ptr) { m_ptr = _ptr; m_type = &rtti::GetTypeInfo<T>(); }
		// type
		virtual inline const TypeInfo &GetTypeInfo() const { return *m_type; }
		// cast
		template<typename T> inline const T *GetPtr() const { return m_type->Cast(m_ptr, rtti::GetTypeInfo<T>()); }
		// operators
		inline IdentifiableConstPtr &operator =(const IdentifiableConstPtr &_other) { m_ptr = _other.m_ptr; m_type = _other.m_type; return *this; }
		inline IdentifiableConstPtr &operator =(const IdentifiablePtr &_other) { m_ptr = _other.GetPtr<void>(); m_type = &_other.GetTypeInfo(); return *this; }
		template<typename T> inline IdentifiableConstPtr &operator =(const T *_ptr) { m_ptr = _ptr; m_type = &rtti::GetTypeInfo<T>(); return *this; }
	protected:
		// instance
		virtual inline void *GetInstancePtr() { return NULL; }
		virtual inline const void *GetInstancePtr() const { return m_ptr; }
	private:
		// ptr
		const void *m_ptr;
		const TypeInfo *m_type;
	};
	
	template<typename T>
	class IdentifiableRef : public Identifiable
	{
	public:
		// constructor
		IdentifiableRef(T &_ref) : m_ref(_ref) {}
		// type
		virtual inline const TypeInfo &GetTypeInfo() const { return rtti::GetTypeInfo<T>(); }
		// cast
		inline T &GetRef() { return m_ref; }
		inline const T &GetRef() const { return m_ref; }
		inline T *GetPtr() { return &m_ref; }
		inline const T *GetPtr() const { return &m_ref; }
		inline T *operator ->() { return &m_ref; }
		inline const T *operator ->() const { return &m_ref; }
	protected:
		// instance
		virtual inline void *GetInstancePtr() { return &m_ref; }
		virtual inline const void *GetInstancePtr() const { return &m_ref; }
	private:
		// ref
		T &m_ref;
	};
	
	template<typename T>
	class IdentifiableConstRef : public Identifiable
	{
	public:
		// constructor
		IdentifiableConstRef(const T &_ref) : m_ref(_ref) {}
		// type
		virtual inline const TypeInfo &GetTypeInfo() const { return rtti::GetTypeInfo<T>(); }
		// cast
		inline const T &GetRef() const { return m_ref; }
		inline const T *GetPtr() const { return &m_ref; }
		inline const T *operator ->() const { return &m_ref; }
	protected:
		// instance
		virtual inline void *GetInstancePtr() { return NULL; }
		virtual inline const void *GetInstancePtr() const { return &m_ref; }
	private:
		// ref
		const T &m_ref;
	};
	
	template<typename T>
	class IdentifiableObject : public Identifiable
	{
	public:
		// type
		virtual inline const TypeInfo &GetTypeInfo() const { return rtti::GetTypeInfo<T>(); }
		// cast
		inline T &GetRef() { return m_object; }
		inline const T &GetRef() const { return m_object; }
		inline T *GetPtr() { return &m_object; }
		inline const T *GetPtr() const { return &m_object; }
		inline T *operator ->() { return &m_object; }
		inline const T *operator ->() const { return &m_object; }
	protected:
		// instance
		virtual inline void *GetInstancePtr() { return &m_object; }
		virtual inline const void *GetInstancePtr() const { return &m_object; }
	private:
		// object
		T m_object;
	};
	
}

#endif // __TYPEINFO_H__
