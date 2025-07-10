# C++ Serializer and Deserializer

A minimal header-only library for fast, reliable text-based serialization of `uint64_t` and `bool`, featuring explicit error handling and zero external dependencies.

## Error Handling

Serializer emits each field as a space‑separated token without failure under normal conditions. Deserializer enforces strict validation: any malformed integer token or anything other than `true`/`false` for booleans triggers `Error::CorruptedArchive` immediately. Callers can inspect the returned `Error` to handle incomplete or corrupted streams gracefully. Extra data beyond the expected fields remains untouched in the stream for further processing.

## Building and Testing

To run all tests after building, invoke `make test` (Google Test). If you need to clear build artifacts, use `make clean`.

```bash
make
make test
make clean
```

## Usage

Construct a `Serializer` around an `std::ostream` or file stream to call `save()`, and similarly use a `Deserializer` on an `std::istream` to call `load()`. Your data type must implement a `serialize` template method that forwards its members to the serializer or deserializer.

```cpp
#include "serializer.hpp"
#include "deserializer.hpp"
#include "data.hpp"

std::stringstream stream;
Data original{42, true, 99};
Serializer writer(stream);
writer.save(original);

Data restored{0, false, 0};
Deserializer reader(stream);
auto result = reader.load(restored);
// result will be Error::NoError if all tokens match expected types
```

After saving, the stream holds a string like

```
42 true 99 
```
