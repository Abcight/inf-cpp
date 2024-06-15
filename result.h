#pragma once
#include <string>
#include <sol/sol.hpp>

/// <summary>
/// A simple, single header wrapper type that represents either
/// a value or an error. This allows for error-by-value evaluation,
/// which I consider cleaner than the throw-catch mechanism.
/// </summary>
/// <typeparam name="T">Value type, if successful.</typeparam>
template <typename T>
struct Result {
private:
	bool is_err = false;
	T m_value;
	std::string m_error;

	// This is a private unwrap behavior for Lua, where we have access
	// to a singular stdout stream, making the explicit consume redundant.
	// This essentially consolidates unwrap and consume.
	sol::optional<T> lua_unwrap() { return consume(std::cout) ? sol::optional<T>() : sol::optional<T>(m_value); }
public:
	~Result() { }
	Result(T value) { m_value = value; }
	Result(std::string error) { is_err = true; m_error = error; }
	static Result Wrap(std::string error, std::string inner) { return Result(error + "\n" + inner); }

	bool is_error() { return is_err; }
	bool is_value() { return !is_err; }

	T& unwrap() { return m_value; }
	std::string& error() { return m_error; }

	bool consume(std::ostream& os) {
		if (is_err) { os << m_error; }
		return is_err;
	}

	static void export_type(sol::state &target) {
		auto generated_name = std::string(typeid(T).name()) + "Result";
		sol::usertype<Result<T>> type = target.new_usertype<Result<T>>(generated_name);
		type["is_error"] = &Result<T>::is_error;
		type["is_value"] = &Result<T>::is_value;
		type["unwrap"] = &Result<T>::lua_unwrap;
	}
};