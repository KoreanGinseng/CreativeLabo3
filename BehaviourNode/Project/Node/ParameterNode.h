#pragma once

#include		"Node.h"

namespace Behaviour {

	template < typename T, typename V >
	class IParameterNode
	{
	protected:
	public:
		virtual const V GetValue(T& actor) const = 0;
	};
	template< typename T, typename V > using ParameterNodePtr = std::shared_ptr<IParameterNode<T, V>>;

	/**
	 * @brief		単純なパラメーター取得用ノード
	 */
	template < typename T, typename V >
	class ValueNode : public IParameterNode<T, V>
	{
	protected:
		V		_value;
	public:
		ValueNode(const V& v)
			: _value(v) {
		}
		virtual ~ValueNode() = default;
		const V GetValue(T& actor) const override {
			return _value;
		}
	};

	/**
	 * @brief		アクターのパラメーター取得用ノード
	 */
	template < typename T, typename V >
	class GetNode : public IParameterNode<T, V>
	{
	protected:
		/** ゲッター */
		using OnGetFunction = std::function< V (T&) >;
		/** ゲット */
		OnGetFunction				OnGet;
	public:
		GetNode(OnGetFunction get)
			: OnGet(get) {
		}
		virtual ~GetNode() = default;

		const V GetValue(T& actor) const override {
			return OnGet(actor);
		}
	};
}