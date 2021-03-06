#ifndef ORZ_TYPES_H
#define ORZ_TYPES_H

#include <string>
#include <cstdint>
#include <vector>
#include <sstream>

// TODO: Remove these and use uint8_t, etc instead.
typedef int64_t FileInt;

std::wstring ToWString(uint8_t b);
std::wstring ToWString(int i);
std::wstring ToWString(uint32_t d);
std::wstring ToWString(const char* i);

template <typename T>
std::wstring ToWString(T i) {
	std::wstringstream ss;
	ss << i;
	return ss.str();
}

std::string ToAString(int i);
std::string ToAString(const std::wstring& s);

template <typename T>
std::string ToAString(T i) {
	std::stringstream ss;
	ss << i;
	return ss.str();
}


template <typename T>
T FromAString(const std::string& s) {
	std::stringstream ss(s);
	T i;
	ss >> i;
	return i;
}

std::wstring UTF8ToWString(const std::string& utf8);
std::string WStringToUTF8(const std::wstring& utf16);

int RoundCast(float rhs);

namespace Util {
	template <typename T> const T& Constrain(const T& lower, const T& val, const T& upper) {
		return std::min<T>(std::max<T>(lower, val), upper);
	}
}

namespace Num {
	const float Pi = 3.14159265358979323846f;
	const float Pi_2 = 1.57079632679489661923f;
	const float Pi_4 = 0.785398163397448309616f;
}

template <typename C>
std::basic_string<C> Implode(const std::vector<std::basic_string<C>>& arr, const C* glue = 0) {
	std::basic_stringstream<C> toret;
	bool first = true;
	for (const auto& item : arr) {
		if (!first) {
			toret << glue;
		}
		toret << item;
		first = false;
	}
	return toret.str();
}


#endif
