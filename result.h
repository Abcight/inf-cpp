#pragma once
#include <string>

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
	T m_value; // Note: Could be a union, but this is generally fine
	std::string m_error;
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
};