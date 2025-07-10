#pragma once

#include <ostream>
#include <cstdint>
#include <utility>

enum class Error
{
    NoError,
    CorruptedArchive
};

class Serializer
{
    static constexpr char Separator = ' ';
public:
    explicit Serializer(std::ostream& out)
        : out_(out)
    {
    }

    template <class T>
    Error save(T& object)
    {
        return object.serialize(*this);
    }

    template <class... Args>
    Error operator()(Args&&... args)
    {
        return processAll(std::forward<Args>(args)...);
    }

private:
    std::ostream& out_;

    Error processAll() {
        return Error::NoError;
    }

    template <class T, class... Rest>
    Error processAll(T&& val, Rest&&... rest) {
        auto err = process(std::forward<T>(val));
        if (err != Error::NoError) {
            return err;
        }
        return processAll(std::forward<Rest>(rest)...);
    }

    Error process(uint64_t value) {
        out_ << value << Separator;
        return Error::NoError;
    }

    Error process(bool value) {
        out_ << (value ? "true" : "false") << Separator;
        return Error::NoError;
    }
};
