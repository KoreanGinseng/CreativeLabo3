#pragma once

#include	"Common.h"

namespace Sample {

	/** メソッド存在確認 */
#define		HAS_FUNCNAME(Name)													\
	template<class ClassType> struct Has##Name									\
	{																			\
		template<class InnerClass, int dummy = (&InnerClass::Name, 0)>			\
		static std::true_type  check(InnerClass*);								\
		static std::false_type check(...);										\
		static ClassType* mClass;												\
	public:																		\
		static const bool value = decltype(check(mClass))::value;				\
	};

	/**
	 * @brief		ToJsonの存在確認用
	 */
	HAS_FUNCNAME(ToJson)
	/**
	 * @brief		FromJsonの存在確認用
	 */
	HAS_FUNCNAME(FromJson)

	/** Json変換用 */
	template < typename T >
	class JsonUtillity {
	private:
		template < bool bp = HasToJson<T>::value >
		static void ToJsonImpl(const T& o, nlohmann::json& os)
		{
			os = o;
		}
		template <  >
		static void ToJsonImpl<true>(const T& o, nlohmann::json& os)
		{
			o.ToJson(os);
		}

		template < bool bp = HasToJson<T>::value >
		static void ToJsonImpl(const T& o, const std::string key, nlohmann::json& os)
		{
			os[key] = o;
		}
		template <  >
		static void ToJsonImpl<true>(const T& o, const std::string key, nlohmann::json& os)
		{
			o.ToJson(os[key]);
		}

		template < bool bp = HasFromJson<T>::value>
		static void FromJsonImpl(T& o, nlohmann::json& os)
		{
			os.get_to(o);
		}
		template <  >
		static void FromJsonImpl<true>(T& o, nlohmann::json& os)
		{
			o.FromJson(os);
		}

		template < bool bp = HasFromJson<T>::value>
		static void FromJsonImpl(T& o, const std::string key, nlohmann::json& os)
		{
			os[key].get_to(o);
		}
		template <  >
		static void FromJsonImpl<true>(T& o, const std::string key, nlohmann::json& os)
		{
			o.FromJson(os[key]);
		}

	public:
		static void ToJson(const T& o, nlohmann::json& os)
		{
			ToJsonImpl(o, os);
		}

		static void ToJson(const T& o, const std::string key, nlohmann::json& os)
		{
			ToJsonImpl(o, key, os);
		}

		static void FromJson(T& o, nlohmann::json& os)
		{
			FromJsonImpl(o, os);
		}
		static void FromJson(T& o, const std::string key, nlohmann::json& os)
		{
			FromJsonImpl(o, key, os);
		}

	};
}