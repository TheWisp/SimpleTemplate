#pragma once
#include <type_traits>
#include <tuple>
#include <iostream>
#include <limits>

namespace ST
{
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

	/*************************************************************************************************************/
	/* Implementation */

	namespace Details
	{
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
			using type = std::int8_t;
		};

		template<long long N>
		struct minimal_integral_type<N, std::enable_if_t<!in_numeric_limits<std::int8_t>(N) && in_numeric_limits<std::int16_t>(N)>>
		{
			using type = std::int16_t;
		};

		template<long long N>
		struct minimal_integral_type<N, std::enable_if_t<!in_numeric_limits<std::int16_t>(N) && in_numeric_limits<std::int32_t>(N)>>
		{
			using type = std::int32_t;
		};

		template<long long N>
		struct minimal_integral_type<N, std::enable_if_t<!in_numeric_limits<std::int32_t>(N)>>
		{
			using type = std::int64_t;
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
			using min_type = typename minimal_integral_type<parse_result>::type;
			return IntegralConstant<min_type, parse_result>{};
		}

	} // namespace Details

	template<char... Digits>
	constexpr auto operator"" _c()
	{
		return Details::parse<Digits...>();
	}

	template<char... Digits>
	constexpr auto operator"" _C()
	{
		return operator""_c<Digits...>();
	}

	/*************************************************************************************************************/
	/* Base types */

	template<typename... Ts> struct TypeTagList;

	template<typename T, T value>//TODO can fwd decl be simplified?
	struct IntegralConstant;

	template<typename T>
	struct TypeTag
	{
		using type = T;
		static constexpr IntegralConstant<size_t, sizeof(T)> size = {};
		static constexpr auto category();
	};

	// These are left undefined for preventing wrong usage: the type is already a type tag
	template<typename T> struct TypeTag<TypeTag<T>>;
	template<typename T> struct TypeTag<TypeTag<T>&>;
	template<typename T> struct TypeTag<const TypeTag<T>>;
	template<typename T> struct TypeTag<const TypeTag<T>&>;

	//#hack: void cannot have sizeof
	template<> struct TypeTag<void>
	{
		using type = void;
		static constexpr auto category();
	};

	//#hack: T[] cannot have sizeof
	template<typename T> struct TypeTag<T[]>
	{
		using type = T[];
		static constexpr auto category();
	};

	template<typename R, typename... A> struct TypeTag<R(A...)>
	{
		using type = R(A...);
		static constexpr auto category();
	};

	template<template<typename...> class PartialT>
	struct PartialTypeTag
	{};

	template<typename T>
	constexpr TypeTag<T> typetag = {};

	//challenge: how to unify with 'typetag', template<auto>?
	template<template<typename...> class PartialT>
	constexpr PartialTypeTag<PartialT> partialtypetag = {};

	// The nonexistent type: no inner type and size are defined. The type tag can only be used in comparisons.
	// An attempt to access its other traits are considered a programming error.
	template<> struct TypeTag<struct __NonSuch>
	{
	};

	using NonSuch = TypeTag<struct __NonSuch>;
	constexpr NonSuch nonsuch = {};

	template<typename T>
	constexpr bool blackbox_false = false;

	template<typename T>
	T _to_type_impl(TypeTag<T>);//no definition

	template<typename T>
	constexpr auto _to_type_impl(T) { static_assert(blackbox_false<T>, "TOTYPE requires TypeTag type"); }

	/*************************************************************************************************************/
	/* Integral constant support for built-in operators */
	template<typename T>
	constexpr IntegralConstant<size_t, sizeof(T)> sizeoftype = {};
	template<typename T>
	constexpr IntegralConstant<size_t, alignof(T)> alignoftag(TypeTag<T>) { return {}; }//TODO remove in favor TypeTag::align?
	template<typename T>
	constexpr IntegralConstant<size_t, alignof(T)> alignoftype = {};
	template<typename... Ts>
	constexpr IntegralConstant<size_t, sizeof...(Ts)> countoftypes = {};

	/*************************************************************************************************************/
	/* Type equality comparison */

	template<typename T>
	constexpr BoolConstantTrue operator==(TypeTag<T>, TypeTag<T>) { return {}; }
	template<typename T1, typename T2>
	constexpr BoolConstantFalse operator==(TypeTag<T1>, TypeTag<T2>) { return {}; }
	template<typename T>
	constexpr BoolConstantFalse operator!=(TypeTag<T>, TypeTag<T>) { return {}; }
	template<typename T1, typename T2>
	constexpr BoolConstantTrue operator!=(TypeTag<T1>, TypeTag<T2>) { return {}; }


	/*************************************************************************************************************/
	/* Type properties */

	template<typename T>
	constexpr BoolConstant<std::is_integral<T>::value> is_integral(TypeTag<T>) { return {}; }

	template<typename T>
	constexpr BoolConstant<std::is_enum<T>::value> is_enum(TypeTag<T>) { return {}; }

	template<typename T>
	constexpr BoolConstant<std::is_class<T>::value> is_class(TypeTag<T>) { return {}; }

	template<typename T, size_t N>
	constexpr BoolConstantTrue is_array(TypeTag<T[N]>) { return {}; }

	template<typename T>
	constexpr BoolConstantTrue is_array(TypeTag<T[]>) { return {}; }

	template<typename T>
	constexpr BoolConstantFalse is_array(TypeTag<T>) { return {}; }

	template<typename T>
	constexpr BoolConstantFalse is_const(TypeTag<T>) { return {}; }
	template<typename T>
	constexpr BoolConstantTrue is_const(TypeTag<const T>) { return {}; }
	template<typename T>
	constexpr BoolConstantTrue is_const(TypeTag<const T&>) { return {}; }
	template<typename T>
	constexpr BoolConstantTrue is_const(TypeTag<const T&&>) { return {}; }

	template<typename T>
	constexpr BoolConstantFalse is_reference(TypeTag<T>) { return {}; }
	template<typename T>
	constexpr BoolConstantTrue is_reference(TypeTag<T&>) { return {}; }
	template<typename T>
	constexpr BoolConstantTrue is_reference(TypeTag<T&&>) { return {}; }

	template<typename T>
	constexpr auto is_reference_type = is_reference(typetag<T>);

	template<typename T>
	constexpr BoolConstant<std::is_default_constructible<T>::value> is_default_constructible(TypeTag<T>) { return {}; }


	/*************************************************************************************************************/
	/* type categories */

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

	struct PointerToMemberObjectTag : TypeCategoryTagBase<PointerToMemberObjectTag>{};
	constexpr PointerToMemberObjectTag pointer_to_member_object_tag = {};

	struct PointerToMemberFunctionTag : TypeCategoryTagBase<PointerToMemberFunctionTag> {};
	constexpr PointerToMemberFunctionTag pointer_to_member_function_tag = {};

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

	template<typename T>
	using TypeCategory = typename TypeCategoryImpl<T>::Category;

	template<typename T>
	constexpr auto type_category(TypeTag<T> = {})
	{
		return TypeCategory<T>{};
	}

	template<typename T>
	constexpr auto type_category(T&&)
	{
		return TypeCategory<T>{};
	}

	template<typename T>
	constexpr auto TypeTag<T>::category()
	{
		return TypeCategory<T>{};
	}

	/*************************************************************************************************************/
	/* type transformations */

	template<typename T>
	constexpr auto/*Must be auto deduced to avoid error*/
		underlying_type_impl(TypeTag<T>, BoolConstantTrue)
	{
		return typetag<std::underlying_type_t<T>>;
	}

	template<typename T>
	constexpr NonSuch underlying_type_impl(TypeTag<T>, BoolConstantFalse)
	{
		return nonsuch;
	}

	template<typename T>
	constexpr auto underlying_type(TypeTag<T>)
	{
		return underlying_type_impl(typetag<T>, is_enum(typetag<T>));
	}

	/*************************************************************************************************************/
	/* type operators */

	template<typename T>
	constexpr TypeTag<T&> operator+ (TypeTag<T>, LValueReferenceTag) { return typetag<T&>; }

	template<typename T>
	constexpr TypeTag<T&&> operator+ (TypeTag<T>, RValueReferenceTag) { return typetag<T&&>; }

	template<typename T>
	constexpr TypeTag<T> operator- (TypeTag<T>, ReferenceTag) { return typetag<T>; }

	template<typename T>
	constexpr TypeTag<T> operator- (TypeTag<T&>, ReferenceTag) { return typetag<T>; }

	template<typename T>
	constexpr TypeTag<T> operator- (TypeTag<T&&>, ReferenceTag) { return typetag<T>; }

	constexpr struct ConstQualifierTag {} const_qualifier = {};

	template<typename T>
	constexpr TypeTag<const T> operator+ (TypeTag<T>, ConstQualifierTag) { return typetag<const T>; }

	template<typename T>
	constexpr TypeTag<const T&> operator+ (TypeTag<T&>, ConstQualifierTag) { return typetag<const T&>; }

	template<typename T>
	constexpr TypeTag<const T&&> operator+ (TypeTag<T&&>, ConstQualifierTag) { return typetag<const T&&>; }

	template<typename T>
	constexpr TypeTag<T> operator- (TypeTag<T>, ConstQualifierTag) { return typetag<T>; }

	template<typename T>
	constexpr TypeTag<T> operator- (TypeTag<const T>, ConstQualifierTag) { return typetag<T>; }

	template<typename T>
	constexpr TypeTag<T&> operator- (TypeTag<const T&>, ConstQualifierTag) { return typetag<T&>; }

	template<typename T>
	constexpr TypeTag<T&&> operator- (TypeTag<const T&&>, ConstQualifierTag) { return typetag<T&&>; }

	/*************************************************************************************************************/
	/* Misc */

	template<typename T, typename ... ArgsT>
	inline auto create(TypeTag<T>, ArgsT&& ... args)
	{
		return T{ std::forward<ArgsT>(args)... };
	}

	// Type properties function taking expressions

	template<typename T>
	constexpr auto is_enum(T&&)
	{
		return is_enum(typetag<T> -reference_tag - const_qualifier);
	}

	template<typename T>
	constexpr auto underlying_type(T&&)
	{
		return underlying_type(typetag<T> -reference_tag - const_qualifier);
	}

	/*************************************************************************************************************/
	/* Type list */
	template<int N, typename... Ts> using NthTypeOf =
		typename std::tuple_element<N, std::tuple<Ts...>>::type;

	//TODO evaluate name 'TypeTagSequence' and 'TypeSequence'
	template<typename... Ts>
	struct TypeTagList
	{
		static constexpr IntegralConstant<size_t, sizeof...(Ts)> length = {};

		template<typename T, T N>
		constexpr auto operator[] (IntegralConstant<T, N>) const
		{
			return typetag<NthTypeOf<N, Ts...>>;
		}

		//TODO: has(TypeTag<T>)

	};

	template<typename ... Ts>
	constexpr TypeTagList<Ts...> typetaglist = {};

	template<typename ... Ts, typename T>
	constexpr auto operator+(TypeTag<T>, TypeTagList<Ts...>)
	{
		return typetaglist<T, Ts...>;
	}

	template<typename... Types1, typename... Types2>
	constexpr auto operator+(TypeTagList<Types1...>, TypeTagList<Types2...>)
	{
		return typetaglist<Types1..., Types2...>;
	}

	template<typename T, typename... Tails>
	constexpr auto operator-(TypeTagList<T, Tails...>, TypeTag<T>)
	{
		return typetaglist<Tails...>;
	}

	template<typename Head, typename... Tails, typename T>
	constexpr auto operator-(TypeTagList<Head, Tails...>, TypeTag<T>)
	{
		return typetag<Head> +(typetaglist<Tails...> -typetag<T>);
	}

	template<typename T>
	constexpr auto operator-(TypeTagList<>, TypeTag<T>)
	{
		return typetaglist<>;
	}

	//helper functions

	template<typename Head, typename... Tails>
	constexpr auto list_first_or_default(TypeTagList<Head, Tails...>)
	{
		return typetag<Head>;
	}

	constexpr auto list_first_or_default(TypeTagList<>) { return nonsuch; }

	template<typename Head, typename... Tails>
	constexpr auto list_without_first(TypeTagList<Head, Tails...>)
	{
		return typetaglist<Tails...>;
	}

	constexpr auto list_without_first(TypeTagList<> l) { return l; }

	template<typename... Ts, typename T>
	constexpr auto operator+(TypeTagList<Ts...>, TypeTag<T>)
	{
		return typetaglist<Ts..., T>;
	}

	template<typename... Types1, typename... Types2>
	constexpr auto operator==(TypeTagList<Types1...> l1, TypeTagList<Types2...> l2)
	{
		//can be further simplified with constexpr if
		return
			countoftypes<Types1...> == countoftypes<Types2...> &&
			list_first_or_default(l1) == list_first_or_default(l2) &&
			list_without_first(l1) == list_without_first(l2);
	}

	constexpr BoolConstantTrue operator==(TypeTagList<>, TypeTagList<>) { return true_c; }

	template<typename... Types1, typename... Types2>
	constexpr auto operator!=(TypeTagList<Types1...> l1, TypeTagList<Types2...> l2)
	{
		return !(l1 == l2);
	}

	template<typename... Ts>
	constexpr auto reverse(TypeTagList<Ts...> list)
	{
		return reverse(list_without_first(list)) + list_first_or_default(list);
	}

	constexpr auto reverse(TypeTagList<> list) { return list; }


	//TODO filter: use enum flag / property tags for common cases

	/*************************************************************************************************************/
	/* Branching */
	template<typename T1, typename T2>
	constexpr decltype(auto) select(BoolConstantTrue, T1 t1, T2) { return t1; }

	template<typename T1, typename T2>
	constexpr decltype(auto) select(BoolConstantFalse, T1, T2 t2) { return t2; }

	/*************************************************************************************************************/
	/* Type unpacking */

	template<template<typename...> class PartialT, typename... Ts>
	constexpr TypeTag<PartialT<Ts...>> combine(PartialTypeTag<PartialT>, TypeTagList<Ts...>) { return {}; }

	template<template<typename...> class PartialT, typename... Ts>
	constexpr TypeTag<PartialT<Ts...>> combine(PartialTypeTag<PartialT>, TypeTag<Ts>...) { return {}; }

} // namespace ST

#define TOTYPE(TYPETAG) decltype(ST::_to_type_impl(TYPETAG))