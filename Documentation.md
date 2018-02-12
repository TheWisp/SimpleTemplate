# Table of Contents
* [`None` (Class), `none` (Constant)](#none-class-none-constant)
* [`Tag<T>` (Template Class), `tag<T>` (Variable Template)](#tagt-template-class-tagt-variable-template)
    * [Tag Comparison](#tag-comparison)
    * [`size` (Static Member Function)](#size-static-member-function)
    * [`category` (Static Member Function)](#category-static-member-function)
* [Type Categories](#type-categories)
    * [List of Type Categories](#list-of-type-categories)
    * [type_category (Template Function)](#type_category-template-function)
    * [TypeCategory (Alias Template)](#typecategory-alias-template)
* [Type Category and Qualifier Transformation](#type-category-and-qualifier-transformation)

# `None` (Class), `none` (Constant)
`struct None`, A special tag indicating "nothing" or the neutral type. Can be returned by type trait functions when failing to get meaningful result. `None` is designed to be a more user-friendly replacement to compilation errors; It gives the user a chance to validate the type result. 

The only value of `None` is declared as following:
```cpp
constexpr None none;
```
`none` can be compared to any other value, and it is not equal to anything but `none` itself. The result from a comparison is `{true_c|false_c}` of type `IntegralConstant<bool, {true|false}>`

 | Expression            | Value        |
 | :-------------------- | :----------- |
 | `none == none`        | `true_c`     |
 | `none == (any value)` | `false_c`    |

You can only compare `none` with other tags, trying to access any trait causes compilation error, because `None` is an empty class that has no member.

# `Tag<T>` (Template Class), `tag<T>` (Variable Template)
```cpp
template<typename T> struct Tag;
template<typename T> constexpr Tag<T> tag;
```
The basic building block and the central type in the library. `Tag<T>` is a wrapper type that contains type predicates and trait functions for `T`, and `tag<T>` is the only constexpr instance of the wrapper, that can be used as a value, passed around, or forcing template argument deduction.

## Tag Comparison
Type tag of the same type T are equal. Type tag of different types are unequal. Types are strictly compared, i.e. sensitive to const-volatile modifiers and references. The result of comparison is one of the `Integral_Constant<bool, ?>` values.

 | Expression                     | Value        |
 | :----------------------------- | :----------- |
 | `tag<int> == tag<int>`         | `true_c`     |
 | `tag<int> == tag<bool>`        | `false_c`    |
 | `tag<int> == tag<const int>`   | `false_c`    |

## `size` (Static Member Function)
Gets the size of the wrapped type as an `IntegralConstant`. Integral constants are more helpful than plain `constexpr` numbers when used in further compile-time deduction and branching scenarios.
When the type's size cannot be taken, the result is of type `None`.

 | Expression                     | Value        |
 | :----------------------------- | :----------- |
 | `tag<std::uint32_t>.size()`    | `4_c`        |
 | `Tag<std::uint32_t>::size()`   | `4_c`        |
 | `tag<void>.size()`             | `none`       |
 | `tag<T[]>.size()`              | `none`       |

## `category` (Static Member Function)
Equivalent to calling [type_category (Template Function)](#type_category-template-function) for the wrapped type.

# Type Categories
A group of tags that indicate the main category of a type, for example, `IntegralTag` is the category of `int`, `long` and `char` types. Type category tags are simple structs (i.e. no template parameter) and have no members. There is a `constexpr` constant representing the only possible value for each type category.

## List of Type Categories

 | Type Category Type           | Type Category Constant            | Matching Types
 | :--------------------------- | :-------------------------------- | :---
 | `VoidTag`                    | `void_tag`                        | `void`
 | `NullptrTag`                 | `nullptr_tag`                     | `std::nullptr_t`
 | `IntegralTag`                | `integral_tag`                    | `bool, char, char16_t, char32_t, wchar_t, short, int, long, long long`
 | `FloatingPointTag`           | `floating_point_tag`              | `float`, `double`, `long double`
 | `ArrayTag`                   | `array_tag`                       | `T[]`, `T[N]` for any type `T` and size `N` 
 | `EnumTag`                    | `enum_tag`                        | Any `enum` or `enum class` type
 | `UnionTag`                   | `union_tag`                       | Any `union` type
 | `ClassTag`                   | `class_tag`                       | Any `class` or `struct` type
 | `FunctionTag`                | `function_tag`                    | `R(A...)`*q* for any type `R` and `A...` with any qualifier *q*
 | `PointerTag`                 | `pointer_tag`                     | `T*` for any type `T`
 | `LValueReferenceTag`         | `lvalue_reference_tag`            | `T&` for any type `T`
 | `RValueReferenceTag`         | `rvalue_reference_tag`            | `T&&` for any type `T`
 | `PointerToMemberObjectTag`   | `pointer_to_member_object_tag`    | `T C::*` for any non-static member data of type `T` in class `C`
 | `PointerToMemberFunctionTag` | `pointer_to_member_function_tag`  | `T (C::*)(A...)`*q* for any non-static member function of type `T(A...)` in class `C` with any qualifier *q*

Some type category tags - `PointerTag`, `LValueReferenceTag` and `RValueReferenceTag` also provide operations with type tags, see [Type Category and Qualifier Transformation](#type-category-and-qualifier-transformation)

Type categories are meant to be compared to each other for type branching purpose.
Any type category tag equals to itself but unequals to any other category, for instance:

 | Expression                                 | Value                    |
 | :----------------------------------------- | :----------------------- |
 | `void_tag == void_tag`                     | `true_c`                 |
 | `void_tag == (any other type category)`    | `false_c`                |
 | `void_tag == (any non-type category value)`| `<Compilation error>`    |

## `type_category` (Template Function)

A template function that returns the type category constant of template argument `T`, declared as following:
```cpp
template<typename T>
constexpr auto type_category();
```
The result is one of the constants listed in [List of Type Categories](#list-of-type-categories).

## `TypeCategory` (Alias Template)

An alias template defined as the type category of template argument `T`:
```cpp
template<typename T>
using TypeCategory = decltype(type_category<T>());
```
The result is one of the types listed in [List of Type Categories](#list-of-type-categories).

# Type Category and Qualifier Transformation
A type tag can be transformed to a const qualified or unqualified, added reference or removed reference type tag. The conversions are defined as binary operators between a type tag and one of the category or qualifier tags, as the example shows:

 | Expression                                           | Value                    |
 | :--------------------------------------------------- | :----------------------- |
 | `tag<int> + lvalue_reference_tag`                    | `tag<int&>`              |
 | `tag<int&> - lvalue_reference_tag`                   | `tag<int>`               |
 | `tag<T> + const_qualifier_tag + lvalue_reference_tag`| `tag<const T&>`          |
 | `tag<T*> - pointer_tag`                              | `tag<T>`                 |
 
The operations are defined between `TypeTag<T>` and `LValueReferenceTag`, `RValueReferenceTag`, `PointerTag`, `ConstQualifierTag`.

