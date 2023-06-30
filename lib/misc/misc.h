#pragma once

#ifndef _MISC_H_
#define _MISC_H_

#include <algorithm>
#include <fmt/format.h>
#include <iostream>
#include <list>
#include <numeric>
#include <vector>
#include <boost/algorithm/string.hpp>

/**
 * @brief
 *
 * @param
 *
 * @return description of the return value
 */
int print_hello_world();

/**
 * @brief Factorial function
 *
 * @param number
 *
 * @return returns factorial
 */
unsigned int Factorial(unsigned int);

namespace misc
{
inline std::string yn_to_tf(std::string yn_str)
{
    if (yn_str == "y") {return "true";}
    else if (yn_str == "n") {return "false";}
    else if (yn_str == "")  {return "false";}
    else                    {throw std::runtime_error("\nin misc::yn_to_tf: yn_str is not in the right format: " + yn_str);}
}

inline std::string bool_to_str(const bool bool_value)
{
    return bool_value ? "true" : "false";
}

inline bool str_to_bool(std::string bool_str)
{
    std::string bool_tf_str = boost::algorithm::to_lower_copy(bool_str);

    if (bool_tf_str != "true" && bool_tf_str != "false") {
        bool_tf_str = misc::yn_to_tf(bool_tf_str);
    }

    if (bool_tf_str == "true")          {return true;}
    else if (bool_tf_str == "false")    {return false;}
    else                                {throw std::runtime_error("\nin misc::str_to_bool, bool_str is not in the right format: " + bool_str);}
}

template <typename T, typename Compare>
std::vector<std::size_t> sort_permutation(const std::vector<T> &vec, Compare &&compare)
{
    std::vector<std::size_t> p(vec.size());
    std::iota(p.begin(), p.end(), 0);
    std::sort(p.begin(), p.end(), [&](std::size_t i, std::size_t j) { return compare(vec[i], vec[j]); });
    return p;
}

template <typename T>
std::vector<T> apply_permutation(const std::vector<T> &vec, const std::vector<std::size_t> &p)
{
    std::vector<T> sorted_vec(vec.size());
    std::transform(p.begin(), p.end(), sorted_vec.begin(), [&](std::size_t i) { return vec[i]; });
    return sorted_vec;
}

template <typename T>
void apply_permutation_in_place(std::vector<T> &vec, const std::vector<std::size_t> &p)
{
    std::vector<bool> done(vec.size());
    for (std::size_t i = 0; i < vec.size(); ++i)
    {
        if (done[i])
        {
            continue;
        }
        done[i] = true;
        std::size_t prev_j = i;
        std::size_t j = p[i];
        while (i != j)
        {
            std::swap(vec[prev_j], vec[j]);
            done[j] = true;
            prev_j = j;
            j = p[j];
        }
    }
}

//! Write character stream manipulator
class WriteChar
{
public:
    explicit WriteChar(char ch) : _ch(ch)
    {
    }

    friend std::ostream &operator<<(std::ostream &stream, const WriteChar &writer)
    {
        return stream << '\'' << writer._ch << '\'';
    }
    friend std::wostream &operator<<(std::wostream &stream, const WriteChar &writer)
    {
        return stream << '\'' << writer._ch << '\'';
    }

private:
    char _ch;
};

//! Write wide character stream manipulator
class WriteWChar
{
public:
    explicit WriteWChar(wchar_t ch) : _ch(ch)
    {
    }

    friend std::ostream &operator<<(std::ostream &stream, const WriteWChar &writer)
    {
        return stream << '\'' << (char)writer._ch << '\'';
    }
    friend std::wostream &operator<<(std::wostream &stream, const WriteWChar &writer)
    {
        return stream << '\'' << writer._ch << '\'';
    }

private:
    wchar_t _ch;
};

//! Write string stream manipulator
class WriteString
{
public:
    template <size_t N>
    explicit WriteString(const char (&str)[N]) : _str(str), _size(N)
    {
    }

    friend std::ostream &operator<<(std::ostream &stream, const WriteString &writer)
    {
        stream << '"';
        stream.write(writer._str, writer._size);
        stream << '"';
        return stream;
    }
    friend std::wostream &operator<<(std::wostream &stream, const WriteString &writer)
    {
        stream << '"';
        for (size_t i = 0; i < writer._size; ++i)
            stream << writer._str[i];
        stream << '"';
        return stream;
    }

private:
    const char *_str;
    size_t _size;
};

//! Write wide string stream manipulator
class WriteWString
{
public:
    template <size_t N>
    explicit WriteWString(const wchar_t (&str)[N]) : _str(str), _size(N)
    {
    }

    friend std::ostream &operator<<(std::ostream &stream, const WriteWString &writer)
    {
        stream << '"';
        for (size_t i = 0; i < writer._size; ++i)
            stream << (char)writer._str[i];
        stream << '"';
        return stream;
    }
    friend std::wostream &operator<<(std::wostream &stream, const WriteWString &writer)
    {
        stream << '"';
        stream.write(writer._str, writer._size);
        stream << '"';
        return stream;
    }

private:
    const wchar_t *_str;
    size_t _size;
};


} // namespace misc


#endif
