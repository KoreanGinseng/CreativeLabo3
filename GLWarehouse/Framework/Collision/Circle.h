#pragma once

#include	"../Vector3.h"

namespace Sample {

	template < typename T >
	class Circle {
	public:
		//-------------------------------------------------
		//�v���p�e�B
		//-------------------------------------------------
		union {
			struct {
				T	X;
				T	Y;
				T	Radius;
			};
			T		Value[3];
		};

		//----------------------------------------------------------------------------
		//�R���X�g���N�^�E�f�X�g���N�^
		//----------------------------------------------------------------------------
		Circle()
		: X(0)
		, Y(0)
		, Radius(0) {
		}
		Circle(const T x, const T y, const T r)
		: X(x)
		, Y(y)
		, Radius(r) {
		}
		Circle(T v[3])
		: X(v[0])
		, Y(v[1])
		, Radius(v[2]) {
		}
		Circle(const Circle< T >& v)
		: X(v.X)
		, Y(v.Y)
		, Radius(v.Radius) {
		}
		~Circle() = default;

		//----------------------------------------------------------------------------
		//Operator
		//----------------------------------------------------------------------------
		//�����̃|�C���^�ւ̃L���X�g
		operator T* () {
			return Value;
		}
		operator const T* () const {
			return Value;
		}
		operator const Circle<float> () const {
			return Circle<float>(
				static_cast<float>(X),
				static_cast<float>(Y),
				static_cast<float>(Radius));
		}

		//���
		Circle< T >& operator = (const Circle< T >& v) {
			X = v.X;		Y = v.Y;		Radius = v.Radius;
			return *this;
		}

		//�Z�p���Z�q +�@-�@*�@/
		Circle< T > operator + (const Vector3F& v) const {
			return Circle< T >(X + v.x, Y + v.y, Radius);
		}

		//����������Z�q +=�@-=�@*=�@/=
		Circle< T >& operator += (const Vector3F& v) {
			X += v.x;		Y += v.y;
			return *this;
		}

		//�Y�����Z�q []
		T& operator [] (int i) {
			return Value[i];
		}
		const T& operator [] (int i) const {
			return Value[i];
		}

		//��r���Z�q == !=
		bool operator == (const Circle< T >& v) const {
			constexpr T e = std::numeric_limits<T>::epsilon();
			return (std::abs(X - v.X) <= e) && (std::abs(Y - v.Y) <= e) && (std::abs(Radius - v.Radius) <= e);
		}
		bool operator != (const Circle< T >& v) const {
			constexpr T e = std::numeric_limits<T>::epsilon();
			return (std::abs(X - v.X) > e) || (std::abs(Y - v.Y) > e) || (std::abs(Radius - v.Radius) > e);
		}

		//----------------------------------------------------------------------------
		//���\�b�h
		//----------------------------------------------------------------------------
		//�ڐG����
		bool Collision(const float px, const float py) const {
			float sx = X - px;
			float sy = Y - py;
			float d = (sx * sx) + (sy * sy);
			return d < (Radius * Radius);
		}
		//�ڐG����
		bool Collision(const Circle< T >& r) const {
			float sx = X - r.X;
			float sy = Y - r.Y;
			float d = (sx * sx) + (sy * sy);
			float rr = Radius + r.Radius;
			return d < (rr * rr);
			return false;
		}

		//������ւ̕ϊ�
		std::string ToString() const {
			std::ostringstream ss;
			ss << X << ", " << Y << ", " << Radius;
			return ss.str();
		}
		//�Ǝ��̂i�r�n�m�o�͎���
		void ToJson(nlohmann::json& os) const
		{
			os = { {"X", X}, {"Y",Y}, {"Radius",Radius} };
		}
		void FromJson(nlohmann::json& os) {
			os["X"].get_to(X);
			os["Y"].get_to(Y);
			os["Radius"].get_to(Radius);
		}

		//�����񂩂�̕ϊ�
		static Circle< T > FromString(const char* str) {
			Circle < T > v;
			std::istringstream ss(str);
			ss >> v.X;
			ss.ignore();
			ss >> v.Y;
			ss.ignore();
			ss >> v.Radius;
			return v;
		}
		static Circle< T > FromString(const std::string& str) {
			return FromString(str.c_str());
		}
		static Circle< T > FromJsonFile(nlohmann::json& os) {
			Circle < T > v;
			os["X"].get_to(v.X);
			os["Y"].get_to(v.Y);
			os["Radius"].get_to(v.Radius);
			return v;
		}
	};

	//�^�u������
	using CircleI = Circle< int >;
	using CircleF = Circle< float >;
	using CircleD = Circle< double >;

}