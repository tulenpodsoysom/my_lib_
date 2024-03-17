#pragma once
#include <iostream>
#include <vector>
#include <array>
#include <string>

template<size_t S>
class bitset
{
	std::array<char, size_t(S / 8 + (S % 8 ? 1 : 0)) > _bits{ 0 };

public:

	template<typename iter>
	constexpr bitset(const iter begin, const iter end)
	{
		if (end - begin > S) throw std::length_error("bitset size is too small");

		std::vector<bool> a{ begin,end };

		auto re = a.rend();
		auto rb = a.rbegin();

		for (auto i = rb; i < re; i++)
		{
			_bits[(i - rb) / 8] += (*i != 0 ? true : false) << (i - rb) % 8;
		}
	};

	constexpr bitset(std::initializer_list<bool> bits) {
		if (bits.size() > S) throw std::length_error("bitset size is too small");

		auto b = bits.begin();
		auto e = bits.end();

		for (auto i = e - 1; i >= b; i--)
			_bits[(e - 1 - i) / 8] += (*i != 0 ? true : false) << (e - 1 - i) % 8;
	};

	bool operator[](size_t index) {
		if (index > S) throw std::invalid_argument("outside the boundaries of bitset");
		return (_bits[index / 8] & 1 << (index % 8)) != 0 ? true : false;
	};

	bitset<S> operator! () {
		bitset a = *this;
		for (auto& i : a._bits) i = ~i;
		return a;
	};

	bitset operator^ (bitset b) {
		bitset a = *this;
		for (size_t i = 0; i < a._bits.size(); i++)
		{
			a._bits[i] ^= b._bits[i];
		}
		return a;
	};


	template <typename T = int>
	T value(size_t begin, size_t end)
	{
		if (begin > end) std::swap(begin, end);
		T value = 0;
		for (size_t i = begin; i < end; i++)
		{
			value += T((*this)[i] << (i - begin));
		}
		return value;
	};

	template <typename T = unsigned>
	T u_value(size_t begin, size_t end) { return value<T>(begin, end); };

	size_t size() { return S; };

	std::string to_string(bool do_notches = true, std::string brackets = "{}") {
		std::string str;
		if (!brackets.empty()) str += brackets[0];
		for (size_t i = 0; i < S; i++)
		{
			if ((S - i) % 8 == 0 && do_notches && i != 0) str += '\'';
			str += (*this)[S - i - 1] ? '1' : '0';
		}

		if (!brackets.empty()) str += brackets[1]; 
		str += ' ';
		return str;
	};

	std::string to_string_no_format() { return to_string(false, ""); };
};

template <size_t S>
std::ostream& operator << (std::ostream& stream, bitset<S> bitset)
{
	return stream << bitset.to_string();
}
