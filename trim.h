#include <string>
#include <iterator>
#include <algorithm>
#include <cctype>
#include <cwctype>

namespace {
	
	namespace detail {
		struct is_space_or_null {
			bool operator ()(char ch) const {
				return ch == '\0' || !!std::isspace(static_cast<unsigned char>(ch));
			}
			
			bool operator ()(wchar_t ch) const {
				return ch == L'\0' || !!std::iswspace(ch);
			}
		};
	}
	
	template<typename StringT, typename UnaryPredicate = detail::is_space_or_null>
	void ltrim_inplace(StringT& s, UnaryPredicate pred = {})
	{
		s.erase(s.begin(), std::find_if_not(s.begin(), s.end(), pred));
	}

	template<typename StringT, typename UnaryPredicate = detail::is_space_or_null>
	void rtrim_inplace(StringT& s, UnaryPredicate pred)
	{
		s.erase(std::find_if_not(s.rbegin(), s.rend(), pred).base(), s.end());
	}

	template<typename StringT, typename UnaryPredicate = detail::is_space_or_null>
	void trim_inplace(StringT& s, UnaryPredicate pred = {})
	{
		ltrim_inplace(s, pred);
		rtrim_inplace(s, pred);
	}

	template<typename StringT, typename UnaryPredicate = detail::is_space_or_null>
	StringT ltrim(StringT s, UnaryPredicate pred = {})
	{
		ltrim_inplace(s, pred);
		return s;
	}

	template<typename StringT, typename UnaryPredicate = detail::is_space_or_null>
	StringT rtrim(StringT s, UnaryPredicate pred = {})
	{
		rtrim_inplace(s, pred);
		return s;
	}

	template<typename StringT, typename UnaryPredicate = detail::is_space_or_null>
	StringT trim(StringT s, UnaryPredicate pred = {})
	{
		ltrim_inplace(s, pred);
		rtrim_inplace(s, pred);
		return s;
	}
}
