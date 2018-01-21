#pragma once
#include <type_traits>
#include <tuple>
#include <iostream>
#include <limits>

namespace ST
{
	/*************************************************************************************************************/
	/* Forward Declarations */
	template<typename T, T value> struct IntegralConstant;
	struct None;
	template<typename T> struct Tag;
	template<typename... Ts> struct List;

	/*************************************************************************************************************/
	/* Integral constants */

	template<typename T, T Value>
	struct IntegralConstant
	{
		using underlying_type = T;

		template<
			typename ToType,
			typename std::enable_if_t<std::is_convertible<T, ToType>::value, int> = 0>
			constexpr explicit operator ToType() const { return static_cast<ToType>(Value); }

		//for any IntegralConstant<non-bool>, explicit bool is defined. This is to prevent ambiguous conversion
		template<typename Self = T, typename = std::enable_if_t<!std::is_same<std::remove_cv_t<Self>, bool>::value>>
		constexpr explicit operator bool() const { return Value != 0; }

		constexpr IntegralConstant() = default;

		template<
			typename OtherT,
			OtherT OtherValue,
			typename std::enable_if_t<std::is_convertible<OtherT, T>::value && (Value == OtherValue), int> = 0>
			constexpr /*implicit*/ IntegralConstant(IntegralConstant<OtherT, OtherValue>) {}
	};

	template<bool Value>
	using BoolConstant = IntegralConstant<bool, Value>;

	using BoolConstantTrue = BoolConstant<true>;
	using BoolConstantFalse = BoolConstant<false>;

	constexpr BoolConstantTrue  true_c = {};
	constexpr BoolConstantFalse false_c = {};

	/*************************************************************************************************************/
	/* Integral constant literals */
	//TODO: support _uc for unsigned (rationale ?)

	// Signed integral constant with automatic type that is enough to hold the value
	template<char... Digits>
	constexpr auto operator"" _c();

	// Upper case _C is an alias to _c
	template<char... Digits>
	constexpr auto operator"" _C();

	/*************************************************************************************************************/
	/* Integral constant operators */

	/* Comparison operators */

	template<typename T1, typename T2, T1 value1, T2 value2>
	constexpr BoolConstant<value1 == value2>
		operator== (IntegralConstant<T1, value1>, IntegralConstant<T2, value2>) { return{}; }

	template<typename T1, typename T2, T1 value1, T2 value2>
	constexpr BoolConstant<value1 != value2>
		operator!= (IntegralConstant<T1, value1>, IntegralConstant<T2, value2>) { return{}; }

	template<typename T1, typename T2, T1 value1, T2 value2>
	constexpr BoolConstant <(value1 < value2)>
		operator< (IntegralConstant<T1, value1>, IntegralConstant<T2, value2>) { return{}; }

	template<typename T1, typename T2, T1 value1, T2 value2>
	constexpr BoolConstant<(value1 <= value2)>
		operator<= (IntegralConstant<T1, value1>, IntegralConstant<T2, value2>) { return{}; }

	template<typename T1, typename T2, T1 value1, T2 value2>
	constexpr BoolConstant<(value1 > value2)>
		operator> (IntegralConstant<T1, value1>, IntegralConstant<T2, value2>) { return{}; }

	template<typename T1, typename T2, T1 value1, T2 value2>
	constexpr BoolConstant<(value1 >= value2)>
		operator>= (IntegralConstant<T1, value1>, IntegralConstant<T2, value2>) { return{}; }

	// Arithmetic operators

	// unary '+'
	template<typename T, T value>
	constexpr IntegralConstant<decltype(+value), +value>
		operator+ (IntegralConstant<T, value>) { return {}; }

	// unary '-'
	template<typename T, T value>
	constexpr IntegralConstant<decltype(-value), -value>
		operator- (IntegralConstant<T, value>) { return {}; }

	// binary '+'
	template<typename T1, typename T2, T1 value1, T2 value2>
	constexpr IntegralConstant<decltype(value1 + value2), value1 + value2>
		operator+ (IntegralConstant<T1, value1>, IntegralConstant<T2, value2>) { return {}; }

	// binary '-'
	template<typename T1, typename T2, T1 value1, T2 value2>
	constexpr IntegralConstant<decltype(value1 - value2), value1 - value2>
		operator- (IntegralConstant<T1, value1>, IntegralConstant<T2, value2>) { return {}; }

	// binary '*'
	template<typename T1, typename T2, T1 value1, T2 value2>
	constexpr IntegralConstant<decltype(value1 * value2), value1 * value2>
		operator* (IntegralConstant<T1, value1>, IntegralConstant<T2, value2>) { return {}; }

	// '/'
	template<typename T1, typename T2, T1 value1, T2 value2>
	constexpr IntegralConstant<decltype(value1 / value2), value1 / value2>
		operator/ (IntegralConstant<T1, value1>, IntegralConstant<T2, value2>) { return {}; }

	// '%'
	template<typename T1, typename T2, T1 value1, T2 value2>
	constexpr IntegralConstant<decltype(value1 % value2), value1 % value2>
		operator% (IntegralConstant<T1, value1>, IntegralConstant<T2, value2>) { return {}; }

	// '~'
	template<typename T, T value>
	constexpr IntegralConstant<decltype(~value), ~value>
		operator~ (IntegralConstant<T, value>) { return {}; }

	// '^'
	template<typename T1, typename T2, T1 value1, T2 value2>
	constexpr IntegralConstant<decltype(value1 ^ value2), value1 ^ value2>
		operator^ (IntegralConstant<T1, value1>, IntegralConstant<T2, value2>) { return {}; }

	// '&'
	template<typename T1, typename T2, T1 value1, T2 value2>
	constexpr IntegralConstant<decltype(value1 & value2), value1 & value2>
		operator& (IntegralConstant<T1, value1>, IntegralConstant<T2, value2>) { return {}; }

	// '|'
	template<typename T1, typename T2, T1 value1, T2 value2>
	constexpr IntegralConstant<decltype(value1 | value2), value1 | value2>
		operator| (IntegralConstant<T1, value1>, IntegralConstant<T2, value2>) { return {}; }

	// '<<'
	template<typename T1, typename T2, T1 value1, T2 value2>
	constexpr IntegralConstant<decltype(value1 << value2), (value1 << value2)>
		operator<< (IntegralConstant<T1, value1>, IntegralConstant<T2, value2>) { return {}; }

	// '>>'
	template<typename T1, typename T2, T1 value1, T2 value2>
	constexpr IntegralConstant<decltype(value1 >> value2), (value1 >> value2)>
		operator>> (IntegralConstant<T1, value1>, IntegralConstant<T2, value2>) { return {}; }

	// Logical operators
	// '!'
	template<typename T, T value>
	constexpr IntegralConstant<decltype(!value), !value>
		operator! (IntegralConstant<T, value>) { return {}; }

	// '&&'
	template<typename T1, typename T2, T1 value1, T2 value2>
	constexpr IntegralConstant<decltype(value1 && value2), value1 && value2>
		operator&& (IntegralConstant<T1, value1>, IntegralConstant<T2, value2>) { return {}; }

	// '||'
	template<typename T1, typename T2, T1 value1, T2 value2>
	constexpr IntegralConstant<decltype(value1 || value2), value1 || value2>
		operator|| (IntegralConstant<T1, value1>, IntegralConstant<T2, value2>) { return {}; }

	// User-defined literal NUMBERLITERAL_c
	template<char... Digits>
	constexpr auto operator"" _c();

	// User-defined literal NUMBERLITERAL_C
	template<char... Digits>
	constexpr auto operator"" _C();

	/*************************************************************************************************************/

#define POSSIBLE_RETURN(...) auto

	/*************************************************************************************************************/
	/* None tag */

	struct None {};

	constexpr None none = {};

	template<typename T>
	constexpr BoolConstantFalse operator==(None, T&&) { return {}; }

	template<typename T>
	constexpr BoolConstantFalse operator==(T&&, None) { return {}; }

	constexpr BoolConstantTrue operator==(None, None) { return {}; }

	template<typename T>
	constexpr BoolConstantTrue operator!=(None, T&&) { return {}; }

	template<typename T>
	constexpr BoolConstantTrue operator!=(T&&, None) { return {}; }

	constexpr BoolConstantFalse operator!=(None, None) { return {}; }

	/*************************************************************************************************************/
	/* Type Categories */

	template<typename T>
	struct TypeCategoryTagBase
	{
		constexpr BoolConstantTrue operator==(TypeCategoryTagBase<T>) { return {}; }
		constexpr BoolConstantFalse operator!=(TypeCategoryTagBase<T>) { return {}; }

		template<typename U>
		constexpr BoolConstantFalse operator==(TypeCategoryTagBase<U>) { return {}; }

		template<typename U>
		constexpr BoolConstantTrue operator!=(TypeCategoryTagBase<U>) { return {}; }
	};

	struct VoidTag : TypeCategoryTagBase<VoidTag> {};
	constexpr VoidTag void_tag = {};

	struct NullptrTag : TypeCategoryTagBase<NullptrTag> {};
	constexpr NullptrTag nullptr_tag = {};

	struct IntegralTag : TypeCategoryTagBase<IntegralTag> {};
	constexpr IntegralTag integral_tag = {};

	struct FloatingPointTag : TypeCategoryTagBase<FloatingPointTag> {};
	constexpr FloatingPointTag floating_point_tag = {};

	struct ArrayTag : TypeCategoryTagBase<ArrayTag> {};
	constexpr ArrayTag array_tag = {};

	struct EnumTag : TypeCategoryTagBase<EnumTag> {};
	constexpr EnumTag enum_tag = {};

	struct UnionTag : TypeCategoryTagBase<UnionTag> {};
	constexpr UnionTag union_tag = {};

	struct ClassTag : TypeCategoryTagBase<ClassTag> {};
	constexpr ClassTag class_tag = {};

	struct FunctionTag : TypeCategoryTagBase<FunctionTag> {};
	constexpr FunctionTag function_tag = {};

	struct PointerTag : TypeCategoryTagBase<PointerTag> {};
	constexpr PointerTag pointer_tag = {};

	struct ReferenceTag : TypeCategoryTagBase<ReferenceTag> {};
	constexpr ReferenceTag reference_tag = {};

	struct LValueReferenceTag : ReferenceTag {};
	constexpr LValueReferenceTag lvalue_reference_tag = {};

	struct RValueReferenceTag : ReferenceTag {};
	constexpr RValueReferenceTag rvalue_reference_tag = {};

	struct PointerToMemberObjectTag : TypeCategoryTagBase<PointerToMemberObjectTag> {};
	constexpr PointerToMemberObjectTag pointer_to_member_object_tag = {};

	struct PointerToMemberFunctionTag : TypeCategoryTagBase<PointerToMemberFunctionTag> {};
	constexpr PointerToMemberFunctionTag pointer_to_member_function_tag = {};

	template<typename T>
	constexpr auto type_category();

	template<typename T>
	using TypeCategory = decltype(type_category<T>());

	/*************************************************************************************************************/
	/* Type properties */
	template<typename E>
	constexpr POSSIBLE_RETURN(None, Tag<...>) enum_underlying_type();

	template<typename T>
	constexpr POSSIBLE_RETURN(None, Tag<...>) function_return_type();

	template<typename T>
	constexpr auto sizeof_type();

	template<typename... Ts>
	constexpr IntegralConstant<size_t, sizeof...(Ts)> countoftypes = {};

	/*************************************************************************************************************/
	/* type tag */

	template<typename T>
	struct Tag
	{
		using Type = T;

		//generic traits
		static constexpr POSSIBLE_RETURN(None, IntegralConstant<...>) size();
		static constexpr
			POSSIBLE_RETURN(VoidTag, NullptrTag, IntegralTag, FloatingPointTag, EnumTag,
				UnionTag, ClassTag, FunctionTag, PointerTag, LValueReferenceTag,
				RValueReferenceTag, PointerToMemberObjectTag, PointerToMemberFunctionTag)
			category();

		//enum traits
		static constexpr POSSIBLE_RETURN(None, Tag<...>) underlying_type();

		//function traits
		static constexpr POSSIBLE_RETURN(None, Tag<...>) return_type();
		//TODO static constexpr List<?> parameter_types()
	};

	// These are left undefined for preventing wrong usage: the type is already a type tag
	template<typename T> struct Tag<Tag<T>>;
	template<typename T> struct Tag<Tag<T>&>;
	template<typename T> struct Tag<const Tag<T>>;
	template<typename T> struct Tag<const Tag<T>&>;

	template<typename T>
	constexpr Tag<T> tag = {};

#define TOTYPE(TAG) typename std::decay_t<decltype(TAG)>::Type

	/*************************************************************************************************************/
	/* partial type tag */

	template<template<typename...> class PartialT>
	struct PartialTag
	{};

	template<template<typename...> class PartialT>
	constexpr PartialTag<PartialT> partialtypetag = {};

	/*************************************************************************************************************/
	/* Type equality comparison */

	template<typename T>
	constexpr BoolConstantTrue operator==(Tag<T>, Tag<T>) { return {}; }
	template<typename T1, typename T2>
	constexpr BoolConstantFalse operator==(Tag<T1>, Tag<T2>) { return {}; }
	template<typename T>
	constexpr BoolConstantFalse operator!=(Tag<T>, Tag<T>) { return {}; }
	template<typename T1, typename T2>
	constexpr BoolConstantTrue operator!=(Tag<T1>, Tag<T2>) { return {}; }

	/*************************************************************************************************************/
	/* type operators */

	template<typename T>
	constexpr Tag<T&> operator+ (Tag<T>, LValueReferenceTag) { return {}; }

	template<typename T>
	constexpr Tag<T&&> operator+ (Tag<T>, RValueReferenceTag) { return {}; }

	template<typename T>
	constexpr Tag<T> operator- (Tag<T>, ReferenceTag) { return {}; }
	template<typename T>
	constexpr Tag<T> operator- (Tag<T&>, ReferenceTag) { return {}; }
	template<typename T>
	constexpr Tag<T> operator- (Tag<T&>, LValueReferenceTag) { return {}; }
	template<typename T>
	constexpr Tag<T> operator- (Tag<T&>, RValueReferenceTag) = delete;
	template<typename T>
	constexpr Tag<T> operator- (Tag<T&&>, ReferenceTag) { return {}; }
	template<typename T>
	constexpr Tag<T> operator- (Tag<T&&>, LValueReferenceTag) = delete;
	template<typename T>
	constexpr Tag<T> operator- (Tag<T&&>, RValueReferenceTag) { return {}; }


	constexpr struct ConstQualifierTag {} const_qualifier_tag = {};

	template<typename T>
	constexpr Tag<const T> operator+ (Tag<T>, ConstQualifierTag) { return {}; }

	template<typename T>
	constexpr Tag<const T&> operator+ (Tag<T&>, ConstQualifierTag) { return {}; }

	template<typename T>
	constexpr Tag<const T&&> operator+ (Tag<T&&>, ConstQualifierTag) { return {}; }

	template<typename T>
	constexpr Tag<T> operator- (Tag<T>, ConstQualifierTag) { return {}; }

	template<typename T>
	constexpr Tag<T> operator- (Tag<const T>, ConstQualifierTag) { return {}; }

	template<typename T>
	constexpr Tag<T&> operator- (Tag<const T&>, ConstQualifierTag) { return {}; }

	template<typename T>
	constexpr Tag<T&&> operator- (Tag<const T&&>, ConstQualifierTag) { return {}; }

	template<typename T>
	constexpr Tag<std::add_pointer_t<T>> operator+ (Tag<T>, PointerTag) { return{}; }

	template<typename T>
	constexpr Tag<std::remove_pointer_t<T>> operator- (Tag<T>, PointerTag) { return{}; }

	/*************************************************************************************************************/
	/* Misc */

	template<typename T, typename ... ArgsT>
	inline auto create(Tag<T>, ArgsT&& ... args)
	{
		return T{ std::forward<ArgsT>(args)... };
	}

	/*************************************************************************************************************/
	/* Type list */
	template<int N, typename... Ts> using NthTypeOf =
		typename std::tuple_element<N, std::tuple<Ts...>>::type;

	template<typename... Ts>
	struct List
	{
		static constexpr IntegralConstant<size_t, sizeof...(Ts)> length = {};

		template<typename T, T N>
		constexpr auto operator[] (IntegralConstant<T, N>) const
		{
			return tag<NthTypeOf<N, Ts...>>;
		}
	};

	template<typename ... Ts>
	constexpr List<Ts...> list = {};

	template<typename ... Ts, typename T>
	constexpr auto operator+(Tag<T>, List<Ts...>)
	{
		return list<T, Ts...>;
	}

	template<typename... Types1, typename... Types2>
	constexpr auto operator+(List<Types1...>, List<Types2...>)
	{
		return list<Types1..., Types2...>;
	}

	template<typename T, typename... Tails>
	constexpr auto operator-(List<T, Tails...>, Tag<T>)
	{
		return list<Tails...>;
	}

	template<typename Head, typename... Tails, typename T>
	constexpr auto operator-(List<Head, Tails...>, Tag<T>)
	{
		return tag<Head> +(list<Tails...> -tag<T>);
	}

	template<typename T>
	constexpr auto operator-(List<>, Tag<T>)
	{
		return list<>;
	}

	//helper functions

	template<typename Head, typename... Tails>
	constexpr auto list_first_or_default(List<Head, Tails...>)
	{
		return tag<Head>;
	}

	constexpr auto list_first_or_default(List<>) { return none; }

	template<typename Head, typename... Tails>
	constexpr auto list_without_first(List<Head, Tails...>)
	{
		return list<Tails...>;
	}

	constexpr auto list_without_first(List<> l) { return l; }

	template<typename... Ts, typename T>
	constexpr auto operator+(List<Ts...>, Tag<T>)
	{
		return list<Ts..., T>;
	}

	template<typename... Types1, typename... Types2>
	constexpr auto operator==(List<Types1...> l1, List<Types2...> l2)
	{
		//can be further simplified with constexpr if
		return
			countoftypes<Types1...> == countoftypes<Types2...> &&
			list_first_or_default(l1) == list_first_or_default(l2) &&
			list_without_first(l1) == list_without_first(l2);
	}

	constexpr BoolConstantTrue operator==(List<>, List<>) { return true_c; }

	template<typename... Types1, typename... Types2>
	constexpr auto operator!=(List<Types1...> l1, List<Types2...> l2)
	{
		return !(l1 == l2);
	}

	template<typename... Ts>
	constexpr auto reverse(List<Ts...> list)
	{
		return reverse(list_without_first(list)) + list_first_or_default(list);
	}

	constexpr auto reverse(List<> list) { return list; }

	//TODO filter: use enum flag / property tags for common cases

	/*************************************************************************************************************/
	/* Branching */
	template<typename T1, typename T2>
	constexpr auto select(BoolConstantTrue, T1 t1, T2) { return t1; }

	template<typename T1, typename T2>
	constexpr auto select(BoolConstantFalse, T1, T2 t2) { return t2; }

	/*************************************************************************************************************/
	/* Type unpacking */

	template<template<typename...> class PartialT, typename... Ts>
	constexpr Tag<PartialT<Ts...>> combine(PartialTag<PartialT>, List<Ts...>) { return {}; }

	template<template<typename...> class PartialT, typename... Ts>
	constexpr Tag<PartialT<Ts...>> combine(PartialTag<PartialT>, Tag<Ts>...) { return {}; }

#undef POSSIBLE_RETURN

	/*************************************************************************************************************/
	/* IMPLEMENTATION */
	/*************************************************************************************************************/
	namespace Details
	{

		/** Integral constant **/

		constexpr char dec_to_value(char c)
		{
			switch (c)
			{
			case '0': return 0;
			case '1': return 1;
			case '2': return 2;
			case '3': return 3;
			case '4': return 4;
			case '5': return 5;
			case '6': return 6;
			case '7': return 7;
			case '8': return 8;
			case '9': return 9;
			default: return '-';
			}
		}

		template<char... Digits>
		struct decimal_parser;

		template<char D, char... Digits>
		struct decimal_parser<D, Digits...>
		{
			static constexpr long long parse(long long prev)
			{
				if (dec_to_value(D) != '-')
					return decimal_parser<Digits...>::parse(prev * 10 + dec_to_value(D));
				else
					return decimal_parser<Digits...>::parse(prev);
			}
		};

		template<>
		struct decimal_parser<>
		{
			static constexpr long long parse(long long prev) { return prev; }
		};

		constexpr char hex_to_value(char c)
		{
			switch (c)
			{
			case '0': return 0;
			case '1': return 1;
			case '2': return 2;
			case '3': return 3;
			case '4': return 4;
			case '5': return 5;
			case '6': return 6;
			case '7': return 7;
			case '8': return 8;
			case '9': return 9;
			case 'A': return 0xA;
			case 'a': return 0xA;
			case 'B': return 0xB;
			case 'b': return 0xB;
			case 'C': return 0xC;
			case 'c': return 0xC;
			case 'D': return 0xD;
			case 'd': return 0xD;
			case 'E': return 0xE;
			case 'e': return 0xE;
			case 'F': return 0xF;
			case 'f': return 0xF;
			default: return '-';
			}
		}

		template<char... Digits>
		struct hex_parser;

		template<char D, char... Digits>
		struct hex_parser<D, Digits...>
		{
			static constexpr long long parse(long long prev)
			{
				if (hex_to_value(D) != '-')
					return hex_parser<Digits...>::parse(prev * 16 + hex_to_value(D));
				else
					return hex_parser<Digits...>::parse(prev);
			}
		};

		template<>
		struct hex_parser<>
		{
			static constexpr long long parse(long long prev) { return prev; }
		};

		constexpr char oct_to_value(char c)
		{
			switch (c)
			{
			case '0': return 0;
			case '1': return 1;
			case '2': return 2;
			case '3': return 3;
			case '4': return 4;
			case '5': return 5;
			case '6': return 6;
			case '7': return 7;
			default: return '-';
			}
		}

		template<char... Digits>
		struct oct_parser;

		template<char D, char... Digits>
		struct oct_parser<D, Digits...>
		{
			static constexpr long long parse(long long prev)
			{
				if (oct_to_value(D) != '-')
					return oct_parser<Digits...>::parse(prev * 8 + oct_to_value(D));
				else
					return oct_parser<Digits...>::parse(prev);
			}
		};

		template<>
		struct oct_parser<>
		{
			static constexpr long long parse(long long prev) { return prev; }
		};

		constexpr char bin_to_value(char c)
		{
			switch (c)
			{
			case '0': return 0;
			case '1': return 1;
			default: return '-';
			}
		}

		template<char... Digits>
		struct bin_parser;

		template<char D, char... Digits>
		struct bin_parser<D, Digits...>
		{
			static constexpr long long parse(long long prev)
			{
				if (bin_to_value(D) != '-')
					return bin_parser<Digits...>::parse(prev * 2 + bin_to_value(D));
				else
					return bin_parser<Digits...>::parse(prev);
			}
		};

		template<>
		struct bin_parser<>
		{
			static constexpr long long parse(long long prev) { return prev; }
		};


		template<typename T>
		constexpr bool in_numeric_limits(long long N)
		{
			return N >= std::numeric_limits<T>::min() && N <= std::numeric_limits<T>::max();
		}

		template<long long N, typename = void>
		struct minimal_integral_type;

		template<long long N>
		struct minimal_integral_type<N, std::enable_if_t<in_numeric_limits<std::int8_t>(N)>>
		{
			using Type = std::int8_t;
		};

		template<long long N>
		struct minimal_integral_type<N, std::enable_if_t<!in_numeric_limits<std::int8_t>(N) && in_numeric_limits<std::int16_t>(N)>>
		{
			using Type = std::int16_t;
		};

		template<long long N>
		struct minimal_integral_type<N, std::enable_if_t<!in_numeric_limits<std::int16_t>(N) && in_numeric_limits<std::int32_t>(N)>>
		{
			using Type = std::int32_t;
		};

		template<long long N>
		struct minimal_integral_type<N, std::enable_if_t<!in_numeric_limits<std::int32_t>(N)>>
		{
			using Type = std::int64_t;
		};

		struct tag_hex {};
		struct tag_dec {};
		struct tag_oct {};
		struct tag_bin {};

		template<char... Digits>
		struct digit_base_parser
		{
			using tag = tag_dec;
		};

		template<char D>
		struct digit_base_parser<D>
		{
			using tag = tag_dec;
		};

		template<char D, char... Digits>
		struct digit_base_parser<'0', 'x', D, Digits...>
		{
			using tag = tag_hex;
		};

		template<char D, char... Digits>
		struct digit_base_parser<'0', 'X', D, Digits...>
		{
			using tag = tag_hex;
		};

		template<char D, char... Digits>
		struct digit_base_parser<'0', 'b', D, Digits...>
		{
			using tag = tag_bin;
		};

		template<char D, char... Digits>
		struct digit_base_parser<'0', 'B', D, Digits...>
		{
			using tag = tag_bin;
		};

		template<char D, char... Digits>
		struct digit_base_parser<'0', D, Digits...>
		{
			using tag = tag_oct;
		};

		template<char... Digits>
		constexpr auto parse_impl(tag_dec)
		{
			return decimal_parser<Digits...>::parse(0);
		}

		template<char/*ignored*/, char/*ignored*/, char... Digits>
		constexpr auto parse_impl(tag_hex)
		{
			return hex_parser<Digits...>::parse(0);
		}

		template<char/*ignored*/, char/*ignored*/, char... Digits>
		constexpr auto parse_impl(tag_bin)
		{
			return bin_parser<Digits...>::parse(0);
		}

		template<char/*ignored*/, char... Digits>
		constexpr auto parse_impl(tag_oct)
		{
			return oct_parser<Digits...>::parse(0);
		}

		template<char... Digits>
		constexpr auto parse()
		{
			using base_tag = typename digit_base_parser<Digits...>::tag;
			constexpr auto parse_result = parse_impl<Digits...>(base_tag{});
			using min_type = typename minimal_integral_type<parse_result>::Type;
			return IntegralConstant<min_type, parse_result>{};
		}

		/** Integral constant **/

		/** Primitive integral constant support **/

		template<typename T>
		struct SizeOfTypeImpl
		{
			static constexpr auto size = IntegralConstant<size_t, sizeof(T)>{};
		};

		template<typename T>
		struct SizeOfTypeImpl<T[]>
		{
			static constexpr auto size = none;
		};

		template<>
		struct SizeOfTypeImpl<void>
		{
			static constexpr auto size = none;
		};

		/** Primitive integral constant support **/

		/** Type categories **/

		template<typename T, typename = void>
		struct TypeCategoryImpl;

#define _ST_SPECIALIZE_TAG(T, TAG)		\
	template<>							\
	struct TypeCategoryImpl<T>			\
	{									\
		using Category = TAG;			\
	};

#define _ST_SPECIALIZE_TAG_CV(T, TAG)            \
	_ST_SPECIALIZE_TAG(T, TAG);                  \
	_ST_SPECIALIZE_TAG(const T, TAG);            \
	_ST_SPECIALIZE_TAG(volatile T, TAG);         \
	_ST_SPECIALIZE_TAG(const volatile T, TAG);

		//void type
		_ST_SPECIALIZE_TAG_CV(void, VoidTag);
		//nullptr type
		_ST_SPECIALIZE_TAG_CV(std::nullptr_t, NullptrTag);

#define _ST_SPECIALIZE_INTEGRAL_TAG_CV_SIGN(T)			 \
	_ST_SPECIALIZE_TAG_CV(T, IntegralTag);               \
	_ST_SPECIALIZE_TAG_CV(unsigned T, IntegralTag);

		//Integral types
		_ST_SPECIALIZE_TAG_CV(bool, IntegralTag);
		_ST_SPECIALIZE_TAG_CV(char16_t, IntegralTag);
		_ST_SPECIALIZE_TAG_CV(char32_t, IntegralTag);
		_ST_SPECIALIZE_TAG_CV(wchar_t, IntegralTag);
		_ST_SPECIALIZE_INTEGRAL_TAG_CV_SIGN(char);
		_ST_SPECIALIZE_INTEGRAL_TAG_CV_SIGN(short);
		_ST_SPECIALIZE_INTEGRAL_TAG_CV_SIGN(int);
		_ST_SPECIALIZE_INTEGRAL_TAG_CV_SIGN(long);
		_ST_SPECIALIZE_INTEGRAL_TAG_CV_SIGN(long long);

		//Floating point types
		_ST_SPECIALIZE_TAG_CV(float, FloatingPointTag);
		_ST_SPECIALIZE_TAG_CV(double, FloatingPointTag);
		_ST_SPECIALIZE_TAG_CV(long double, FloatingPointTag);

		//Array types
		template<typename T>
		struct TypeCategoryImpl<T, std::enable_if_t<std::is_array<T>::value>>
		{
			using Category = ArrayTag;
		};

		//Enum types
		template<typename T>
		struct TypeCategoryImpl<T, std::enable_if_t<std::is_enum<T>::value>>
		{
			using Category = EnumTag;
		};

		//Union types
		template<typename T>
		struct TypeCategoryImpl<T, std::enable_if_t<std::is_union<T>::value>>
		{
			using Category = UnionTag;
		};

		//Class types
		template<typename T>
		struct TypeCategoryImpl<T, std::enable_if_t<std::is_class<T>::value>>
		{
			using Category = ClassTag;
		};

		//Function types
		template<typename T>
		struct TypeCategoryImpl<T, std::enable_if_t<std::is_function<T>::value>>
		{
			using Category = FunctionTag;
		};

		//Pointer types
		template<typename T>
		struct TypeCategoryImpl<T, std::enable_if_t<std::is_pointer<T>::value>>
		{
			using Category = PointerTag;
		};

		//Reference types
		template<typename T>
		struct TypeCategoryImpl<T, std::enable_if_t<std::is_lvalue_reference<T>::value>>
		{
			using Category = LValueReferenceTag; //convertible to ReferenceTag
		};

		template<typename T>
		struct TypeCategoryImpl<T, std::enable_if_t<std::is_rvalue_reference<T>::value>>
		{
			using Category = RValueReferenceTag; //convertible to ReferenceTag
		};

		//Pointer to member types
		template<typename T>
		struct TypeCategoryImpl<T, std::enable_if_t<std::is_member_object_pointer<T>::value>>
		{
			using Category = PointerToMemberObjectTag;
		};

		template<typename T>
		struct TypeCategoryImpl<T, std::enable_if_t<std::is_member_function_pointer<T>::value>>
		{
			using Category = PointerToMemberFunctionTag;
		};

#undef _ST_SPECIALIZE_TAG
#undef _ST_SPECIALIZE_TAG_CV
#undef _ST_SPECIALIZE_INTEGRAL_TAG_CV_SIGN
		/** Type categories **/

		/** Enum traits **/

		template<typename E, typename = TypeCategory<E>>
		struct UnderlyingTypeImpl
		{
			static constexpr auto tag = none;
		};

		template<typename E>
		struct UnderlyingTypeImpl<E, EnumTag>
		{
			static constexpr auto tag = ST::tag<std::underlying_type_t<E>>;
		};

		/** Enum traits **/

		/** Function traits **/

		template<typename T> struct FunctionTypeImpl;
		template<typename R, typename ... A> struct FunctionTypeImpl<R(A...)>
		{
			static constexpr auto return_type = tag<R>;
		};

		/** Function traits **/


	} // namespace Details

	/** Integral Constants **/

	template<char... Digits>
	constexpr auto operator"" _c()
	{
		return Details::parse<Digits...>();
	}

	// User-defined literal NUMBERLITERAL_C
	template<char... Digits>
	constexpr auto operator"" _C()
	{
		return operator""_c<Digits...>();
	}

	/** Integral Constants **/

	/** Free functions **/

	template<typename T>
	constexpr auto sizeof_type()
	{
		return Details::SizeOfTypeImpl<T>::size;
	}

	template<typename T>
	constexpr auto type_category()
	{
		return typename Details::TypeCategoryImpl<T>::Category{};
	}

	template<typename E>
	constexpr auto enum_underlying_type()
	{
		return Details::UnderlyingTypeImpl<E>::tag;
	}

	template<typename T>
	constexpr auto function_return_type()
	{
		return Details::FunctionTypeImpl<T>::return_type;
	}

	/** Free functions **/

	/** struct Tag **/

	template<typename T> constexpr auto Tag<T>::size()
	{
		return sizeof_type<T>();
	}

	template<typename T> constexpr auto Tag<T>::category()
	{
		return type_category<Type>();
	}

	template<typename T> constexpr auto Tag<T>::underlying_type()
	{
		return enum_underlying_type<T>();
	}

	template<typename T> constexpr auto Tag<T>::return_type()
	{
		return function_return_type<T>();
	}

	/** struct Tag **/

	/* END OF IMPLEMENTATION */
	/*************************************************************************************************************/

} // namespace ST