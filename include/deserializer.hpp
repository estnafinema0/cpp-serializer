#pragma once

#include <istream>
#include <string>
#include <cstdint>
#include "serializer.hpp" 

class Deserializer
{
    static constexpr char Separator = ' ';
public:
    explicit Deserializer(std::istream& in)
        : in_(in)
    {
    }

    template <class T>
    Error load(T& object)
    {
        return object.serialize(*this);
    }

    template <class... Args>
    Error operator()(Args&... args)
    {
        return processAll(args...);
    }

private:
    std::istream& in_;

    Error processAll()
    {
        return Error::NoError;
    }

    template <class T, class... Rest>
    Error processAll(T& val, Rest&... rest)
    {
        auto err = process(val);
        if (err != Error::NoError) {
            return err;
        }
        return processAll(rest...);
    }

    Error process(uint64_t& value)
    {
        std::string token;
        if (!(in_ >> token)) {
            return Error::CorruptedArchive;
        }
        try {
            size_t pos;
            unsigned long long tmp = std::stoull(token, &pos);
            if (pos != token.size()) {
                return Error::CorruptedArchive;
            }
            value = tmp;
        } catch (...) {
            return Error::CorruptedArchive;
        }
        return Error::NoError;
    }

    Error process(bool& value)
    {
        std::string token;
        if (!(in_ >> token)) {
            return Error::CorruptedArchive;
        }
        if (token == "true") {
            value = true;
        } else if (token == "false") {
            value = false;
        } else {
            return Error::CorruptedArchive;
        }
        return Error::NoError;
    }
};
