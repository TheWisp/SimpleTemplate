## Documentation
For more details, visit the [Documentation](Documentation.md).

## What's the Simple Template Library?
Simple Template is a single-header, small, MSVC / GCC / Clang compatible, C++14 template metaprogramming library. 

The development started as I needed more powerful metaprogramming tools than `<type_traits>` in various projects. Boost::MPL felt too old-style as of C++14 era, and I was reluctant to pull in Boost dependencies anyway. On the other hand, MSVC was not ready for Boost::Hana due to its known lack of supports to several key language features. This library was heavily inspired by Boost::Hana's type constant approach and techniques from other template libraries.

## Motivation
* Types as constants: `tag<int> == tag<int>`
* Type traits as constexpr functions: `tag<int>.category() == integral_tag`
* Type transformation as constexpr operations: 
`tag<int&> - lvalue_reference_tag + const_qualifier_tag == tag<const int>`
* Extensible type computation as constexpr functions taking and returning type consts
* Type branching as tag dispatching or tag specialization

The library provides easy to use wrappers and functions for two main use cases in template metaprogramming: type transformation and type branching. Fundamentally, every type-related action in generic programming falls into one of the two categories. Type transformations usually involve trait classes and nested types, and branching or dispatching mainly use template specialization, overloading and SFINAE. Writing generic code using these techniques are difficult and tedious work. Simple Template aims to improve these common use cases by providing cleaner and more natural syntax based on C++14 template consts.

## Supported Compilers
Basically all compilers that are C++14 compliant. The development is driven by multi-platform unit testing to guarantee compatibility on all big-3:

* Visual C++: VS2015, recommended to install the latest VS2017
* GCC: > 5.0
* Clang: > 3.4

## How to Use
Include the single header, `simpletemplate.hpp` and you are good to go.

Do note you need to explicitly use the integral constant operator in order to use numeric compile-time constants such as `0_c`:

```cpp
using ST::operator""_c; //in your namespace
```

## Tutorial
`Tag<T>` and `tag<T>` are the basic building blocks here. For better distinction, TitleCase symbols here represent types and snake_cases represent values, which can be variables, consts or functions. `Tag<T>` is a wrapper type that contains type predicates and trait functions for `T`, and `tag<T>` is the only constexpr instance of the wrapper, that can be used as a value, passed around, or forcing template argument deduction.

### Getting the Inner Type
From `Tag<T>`, use `Tag<T>::Type` for the actual wrapped type. From an instance of `tag<T>` which can be an alias without mentioning `T`, use `decltype(tag)::Type`, this is useful when the type tag itself is the result of a more complex type deduction:

```cpp
constexpr auto some_advanced_type_tag = <...>;//omitted
using MyType = decltype(some_advanced_type_tag)::Type;
```

Alternatively, it is possible to use TOTYPE(tag) macro for this.

### Type Comparison
Type tag of the same type `T` are equal. Type tag of different types are unequal. Types are strictly compared, i.e. sensitive to const-volatile modifiers and references. The comparisons happens at compile time and can be tested in `static_assert`s:

```cpp
tag<int> == tag<int>                                //true
tag<T> == tag<int>                                  //true if T is int
tag<const std::string&> == tag<std::string>         //false
```

### Type Transformation
Now that we've converted types and their properties to tag values, type transformations can benefit from much better syntaxes with operators.
We can manipulate types just by `+/-` modifier flags, like this:

```cpp
tag<int> + lvalue_reference_tag == tag<int&>    //true
tag<int&> - lvalue_reference_tag == tag<int>    //true
tag<T> + const_qualifier_tag + lvalue_reference_tag == tag<const T&>    //true
```

This may look redundant at first, since one can refer to `const T&` when `T` is already in the context, but it will come in handy when the type itself is generic and compound, like in this case:

```cpp
template<class T>
void my_generic_func(T&& t)
{
    constexpr auto T_noref = tag<T> - reference_tag;
    my_generic_impl(std::forward<T>(t), T_noref.category(), T_noref.size);
}
```

All possible combinations are listed here: (still under development)

 | Type                        | Constant                        |
 | :-------------------------- | :------------------------------ |
 | ReferenceTag                | reference_tag                   |
 | PointerTag                  | pointer_tag                     |
 | LValueReferenceTag          | lvalue_reference_tag            |
 | RValueReferenceTag          | rvalue_reference_tag            |
 | ConstQualifierTag           | const_qualifier_tag             |


### Type Traits
`Tag<T>::size()` is a constexpr function returning sizeof(T) as an integral constant.

```cpp
static_assert(tag<int>.size() == 4_c, ""); //should be true most of the time :)
```

`Tag<T>::category()` is a constexpr function that returns one of these type category tags:

 | Type                        | Constant                        |
 | :-------------------------- | :------------------------------ |
 | VoidTag                     | void_tag                        |
 | NullptrTag                  | nullptr_tag                     |
 | IntegralTag                 | integral_tag                    |
 | FloatingPointTag            | floating_point_tag              |
 | ArrayTag                    | array_tag                       |
 | EnumTag                     | enum_tag                        |
 | UnionTag                    | union_tag                       |
 | ClassTag                    | class_tag                       |
 | FunctionTag                 | function_tag                    |
 | PointerTag                  | pointer_tag                     |
 | LValueReferenceTag          | lvalue_reference_tag            |
 | RValueReferenceTag          | rvalue_reference_tag            |
 | PointerToMemberFunctionTag  | pointer_to_member_object_tag    |
 | PointerToMemberObjectTag    | pointer_to_member_function_tag  |


```cpp
tag<int>.category() == integral_tag //true
tag<void>.category() == void_tag //true
tag<decltype(&C::f)>.category() == pointer_to_member_function_tag     //true for member function f in class C
```

Alternatively, the library provides an equivalent free function `type_category`.
Furthermore, a helper type alias `TypeCategory<T>` refers to the same result but as a type:

```cpp
TypeCategory<float> //FloatingPointTag, same as decltype(type_category<float>())
```

It is easily noticeable these also match up with type category predicates defined in the `<type_traits>` header.
The advantage of making them types (and typed constants) rather than true/false, is that we can easily use tag-dispatching instead of type trait concatenation and SFINAE. This is shown in the following tag-dispatching example:

```cpp
//catches integral types
template<typename T>
constexpr int generic_func_example_impl(T&& t, IntegralTag) { return 1; }

//catches enum types
template<typename T>
constexpr int generic_func_example_impl(T&& t, EnumTag) { return 2; }

//catches and discards everything else
constexpr int generic_func_example_impl(...) { return -1; }

template<typename T>
constexpr int generic_func_example(T&& t)
{
	return generic_func_example_impl(t, type_category<T>());
}

static_assert(generic_func_example(32) == 1, "");
enum class E {A};
static_assert(generic_func_example(E::A) == 2, "");
static_assert(generic_func_example(3.f) == -1, "");
```

Another class specialization example where SFINAE is avoided:

```cpp
template<typename T, typename = TypeCategory<T>>
struct GenericClassExample {
	static constexpr int x = 0;
};

template<typename T>
struct GenericClassExample<T, EnumTag> {
	static constexpr int x = 1;
};

template<typename T>
struct GenericClassExample<T, FloatingPointTag> {
	static constexpr int x = 2;
};

template<typename T>
struct GenericClassExample<T, PointerTag> {
	static constexpr int x = 3;
};

static_assert(GenericClassExample<int>::x == 0, "");
static_assert(GenericClassExample<double>::x == 2, "");
```

Tag-dispatching is generally cleaner and easier to use than SFINAE, especially when there is more than an instance / a set of trait to test, imagine:

```cpp
template<class T, class U> 
void generic_impl(T t, IntegralTag, U u, IntegralTag); //does something with integral op integral

template<class T, class U> 
void generic_impl(T t, ClassTag, U u, IntegralTag); //does something with class.func(integral)

template<class T, class U> 
void generic(T t, U u) { return generic_impl(t, type_category(t), u, type_category(u)); }
```

Such code would be very difficult and even ugly to write with SFINAE.

## Upcoming Changes
The library is under active development so API breaking changes can be expected.
* Type list
* More type traits
* Compile time type names (compiler dependent magic!)
* Documentation
* C++17 support (template auto integral constants)

## Conclusion
Happy templating!

