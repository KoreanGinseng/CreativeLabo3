#pragma once

#include	"Subject.h"

template <typename T>
class ReactiveParameter : public Subject<T> {
private:
	/** パラメーター */
	T		value;
public:
	/**
	 * コンストラクタ
	 */
	ReactiveParameter(const T& v)
		: Subject<T>()
		, value(v)
	{
	}
	/**
	 * コンストラクタ
	 */
	ReactiveParameter(const ReactiveParameter< T >& v)
		: Subject<T>()
		, value(v.value)
	{
	}
	/**
	 * デストラクタ
	 */
	virtual ~ReactiveParameter()
	{
	}

	/** キャスト */
	operator const T& () const {
		return value;
	}
	/** 代入 */
	ReactiveParameter< T >& operator = (const ReactiveParameter< T >& v) {
		value = v.value;
		this->Notify(value);
		return *this;
	}
	ReactiveParameter< T >& operator = (const T& v) {
		value = v;
		this->Notify(value);
		return *this;
	}

	/** 算術演算子 +　-　* /　*/
	T operator + (const ReactiveParameter< T >& v) const {
		return value + v.value;
	}
	T operator + (const T& v) const {
		return value + v;
	}
	T operator - (const ReactiveParameter< T >& v) const {
		return value - v.value;
	}
	T operator - (const T& v) const {
		return value - v;
	}
	T operator * (const ReactiveParameter< T >& v) const {
		return value * v.value;
	}
	T operator * (const T& v) const {
		return value * v;
	}
	T operator / (const ReactiveParameter< T >& v) const {
		return value / v.value;
	}
	T operator / (const T& v) const {
		return value / v;
	}

	/** 複合代入演算子 +=　-=　*=　/= */
	ReactiveParameter< T >& operator += (const ReactiveParameter< T >& v) {
		value += v.value;
		this->Notify(value);
		return *this;
	}
	ReactiveParameter< T >& operator += (const T& v) {
		value += v;
		this->Notify(value);
		return *this;
	}
	ReactiveParameter< T >& operator -= (const ReactiveParameter< T >& v) {
		value -= v.value;
		this->Notify(value);
		return *this;
	}
	ReactiveParameter< T >& operator -= (const T& v) {
		value -= v;
		this->Notify(value);
		return *this;
	}
	ReactiveParameter< T >& operator *= (const ReactiveParameter< T >& v) {
		value *= v.value;
		this->Notify(value);
		return *this;
	}
	ReactiveParameter< T >& operator *= (const T& v) {
		value *= v;
		this->Notify(value);
		return *this;
	}
	ReactiveParameter< T >& operator /= (const ReactiveParameter< T >& v) {
		value /= v.value;
		this->Notify(value);
		return *this;
	}
	ReactiveParameter< T >& operator /= (const T& v) {
		value /= v;
		this->Notify(value);
		return *this;
	}

	/** 比較演算子 == != */
	bool operator == (const ReactiveParameter< T >& v) const {
		return value == v.value;
	}
	bool operator == (const T& v) const {
		return value == v;
	}
	bool operator != (const ReactiveParameter< T >& v) const {
		return value != v.value;
	}
	bool operator != (const T& v) const {
		return value != v;
	}
};