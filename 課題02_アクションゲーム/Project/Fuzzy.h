#pragma once
#include "Mof.h"
#include <array>

class CFuzzyUtilities
{
public:
	CFuzzyUtilities()  = delete;
	~CFuzzyUtilities() = delete;
	//ŽÎ•Ó
	static float Grade(float value, float x0, float x1);
	static float ReverceGrade(float value, float x0, float x1);
	//ŽOŠpŒ`
	static float Triangle(float value, float x0, float x1, float x2);
	//‘äŒ`
	static float Trapenoid(float value, float x0, float x1, float x2, float x3);

	static float HedgeVery(float value) { return std::powf(value, 2.0f); }
	static float HedgeNotVery(float value) { return std::powf(value, 0.5f); }

	static float And(float a, float b) { return std::fminf(a, b); }
	static float Or(float a, float b) { return std::fmaxf(a, b); }
	static float Not(float a) { return 1.0f - a; }
};

enum FuzzyFunc : MofU8 
{
	Fuzzy_Grade        = 0x01,
	Fuzzy_ReverceGrage = 0x02,
	Fuzzy_Triangle     = 0x04,
	Fuzzy_Trapenoid    = 0x08,

	Fuzzy_Mask         = 0x0F,

	Fuzzy_Very         = 0x10,
	Fuzzy_NotVery      = 0x20,
};

using FuncValues = std::pair<FuzzyFunc, std::array<float, 4>>;
class CFuzzyValues
{
private:
	
	std::vector<float>      m_Value;
	std::vector<FuncValues> m_Values;

	void Resize(int n);

public:

	void SetFunc(int n, FuzzyFunc func);

	void SetValues(int n, float* values, int value_count);

	void SetFuncValues(int n, FuzzyFunc func, float* values);

	void Calc(float value);

	int GetCount() const { return m_Value.size(); }
	float& operator[](int n) { return m_Value[n]; }
};

enum FuzzyRule : MofU8
{
	FuzzyRule_None   = 0x00,
	FuzzyRule_And    = 0x01,
	FuzzyRule_Or     = 0x02,
	FuzzyRule_Not    = 0x04,
};

class CFuzzyRule
{
public:
	struct Value
	{
		bool                   bEnable{ true };
		float                  value[2];
		float                  weight[2];
		FuzzyRule              rule{ FuzzyRule_None };
		Value(FuzzyRule rule = FuzzyRule_None, float w1 = 1.0f, float w2 = 1.0f, bool enable = true, float v1 = -1.0f, float v2 = -1.0f)
			: bEnable(enable)
			, rule(rule)
		{
			value[0]  = v1;
			value[1]  = v2;
			weight[0] = w1;
			weight[1] = w2;
		}
	};
private:

	std::vector<Value>     m_Values;
	std::vector<float>     m_Result;

	void Resize(int n);

public:

	void Add(const Value& v);
	void Add(FuzzyRule rule = FuzzyRule_None, float w1 = 1.0f, float w2 = 1.0f, bool enable = true, float v1 = -1.0f, float v2 = -1.0f);

	void SetValue(int n, const Value& v);
	void SetValue(int n, float v1, float w1, float v2, float w2, bool enable = true, FuzzyRule rule = FuzzyRule_None);

	void Calc();

	float& operator[](int n) { return m_Result[n]; }
};

