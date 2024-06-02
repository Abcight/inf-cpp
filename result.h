#pragma once
#include <string>

/// <summary>
/// A simple, single header wrapper type that represents either
/// a value or an error. This allows for error-by-value evaluation,
/// which I consider cleaner than the throw-catch mechanism.
/// </summary>
/// <typeparam name="T">Value type, if successful.</typeparam>
/// <typeparam name="E">Error type, if not succesful.</typeparam>
template <typename T, typename E = std::string>
struct Result {
private:
	bool is_err = false;
	T m_value; // Note: Could be a union, but this is generally fine
	E m_error;
public:
	~Result() { }
	Result(T value) { m_value = value; }
	Result(E error) { is_err = true; m_error = error; }
	static Result Wrap(E error, E inner) { return Result(error + inner); }

	bool is_error() { return is_err; }
	bool is_value() { return !is_err; }

	T& unwrap() { return m_value; }
	E& error() { return m_error; }
};