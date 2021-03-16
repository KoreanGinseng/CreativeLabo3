#include "Fuzzy.h"

float CFuzzyUtilities::Grade(float value, float x0, float x1)
{
	assert(x0 <= x1);
	if (value < x0) { return 0.0f; }
	if (value >= x1) { return 1.0f; }
	if (x0 <= value && value < x1) {
		return (value - x0) / (x1 - x0);
	}
	return -1.0f;
}

float CFuzzyUtilities::ReverceGrade(float value, float x0, float x1)
{
	return Not(Grade(value, x0, x1));
}

float CFuzzyUtilities::Triangle(float value, float x0, float x1, float x2)
{
	assert(x0 <= x1);
	assert(x1 <= x2);
	if (value < x0) { return 0.0f; }
	if (value >= x2) { return 0.0f; }
	if (x0 <= value && value < x1) {
		return Grade(value, x0, x1);
	}
	if (x1 <= value && value < x2) {
		return Not(Grade(value, x1, x2));
	}
	return -1.0f;
}

float CFuzzyUtilities::Trapenoid(float value, float x0, float x1, float x2, float x3)
{
	assert(x0 <= x1);
	assert(x1 <= x2);
	assert(x2 <= x3);
	if (value < x0) { return 0.0f; }
	if (value >= x3) { return 0.0f; }
	if (x1 <= value && value < x2) { return 1.0f; }
	if (x0 <= value && value < x1) {
		return Grade(value, x0, x1);
	}
	if (x2 <= value && value < x3) {
		return Not(Grade(value, x2, x3));
	}
	return -1.0f;
}

void CFuzzyValues::Resize(int n)
{
	if (m_Value.size() >= n)
	{
		return;
	}
	m_Value.resize(n);
	m_Values.resize(n);
}

void CFuzzyValues::SetFunc(int n, FuzzyFunc func)
{
	Resize(n);
	m_Values[n].first = func;
}

void CFuzzyValues::SetValues(int n, float * values, int value_count)
{
	Resize(n);
	std::array<float, 4>& ary = m_Values[n].second;
	for (int i = 0; i < value_count; i++)
		ary[i] = values[i];
}

void CFuzzyValues::SetFuncValues(int n, FuzzyFunc func, float * values)
{
	int cnt = ((func & Fuzzy_Grade) || (func & Fuzzy_ReverceGrage)) 
		? 2 :  (func & Fuzzy_Triangle)
		? 3 :  (func & Fuzzy_Trapenoid)
		? 4 : 0;
	SetFunc(n, func);
	SetValues(n, values, cnt);
}

void CFuzzyValues::Calc(float value)
{
	const int size = m_Value.size();
	for (int i = 0; i < size; i++)
	{
		FuzzyFunc&            func   = m_Values[i].first;
		std::array<float, 4>& values = m_Values[i].second;
		float tmp;
		switch (func & Fuzzy_Mask)
		{
		case Fuzzy_Grade:
			tmp = CFuzzyUtilities::Grade(value, values[0], values[1]);
			break;
		case Fuzzy_ReverceGrage:
			tmp = CFuzzyUtilities::ReverceGrade(value, values[0], values[1]);
			break;
		case Fuzzy_Triangle:
			tmp = CFuzzyUtilities::Triangle(value, values[0], values[1], values[2]);
			break;
		case Fuzzy_Trapenoid:
			tmp = CFuzzyUtilities::Trapenoid(value, values[0], values[1], values[2], values[3]);
			break;
		}
		if (func & Fuzzy_Very)    tmp = CFuzzyUtilities::HedgeVery(tmp);
		if (func & Fuzzy_NotVery) tmp = CFuzzyUtilities::HedgeNotVery(tmp);
		m_Value[i] = tmp;
	}
}

void CFuzzyRule::Resize(int n)
{
	if (m_Values.size() >= n)
	{
		return;
	}
	m_Values.resize(n);
	m_Result.resize(n);
}

void CFuzzyRule::Add(const Value & v)
{
	m_Values.push_back(v);
	m_Result.push_back(-1.0f);
}

void CFuzzyRule::Add(FuzzyRule rule, float w1, float w2, bool enable, float v1, float v2)
{
	Value f(rule, w1, w2, enable, v1, v2);
	Add(f);
}

void CFuzzyRule::SetValue(int n, const Value & v)
{
	m_Values[n] = v;
}

void CFuzzyRule::SetValue(int n, float v1, float w1, float v2, float w2, bool enable, FuzzyRule rule)
{
	FuzzyRule r = rule;
	if (r == FuzzyRule::FuzzyRule_None) 
	{
		r = m_Values[n].rule;
	}
	Value v(r, w1, w2, enable, v1, v2);
	SetValue(n, v);
}

void CFuzzyRule::Calc()
{
	const int size = m_Values.size();
	for (int i = 0; i < size; i++)
	{
		Value& v = m_Values[i];
		if (!v.bEnable)
		{
			m_Result[i] = -1.0f;
			continue;
		}
		float tmp;
		switch (v.rule)
		{
		case FuzzyRule_And:
			tmp = CFuzzyUtilities::And(v.value[0] * v.weight[0], v.value[1] * v.weight[1]);
			break;
		case FuzzyRule_Or:
			tmp = CFuzzyUtilities::Or(v.value[0] * v.weight[0], v.value[1] * v.weight[1]);
			break;
		case FuzzyRule_Not:
			tmp = CFuzzyUtilities::Not(v.value[0] * v.weight[0]);
			break;
		case FuzzyRule_None:
			tmp = v.value[0] * v.weight[0];
			break;
		}
		m_Result[i] = tmp;
	}
}
