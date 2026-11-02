# IOBU

IOBU is an independent C++ library providing generic _zero-copy buffer utilities_. It is compatible with the vast majority
of STL and 3rd-party buffer-like types, including `std::vector<std::byte>`, `std::string`, `asio::mutable/const_buffer`, `std::span<uint8_t>`.
No additional conversion operators, no polluting the call site with additional constructors, calls to `make_buffer` or `std::views` adaptors:
if you have a _buffer-compatible_ type or a _buffer sequence_ - you _deserve_ to use it directly, no questions asked, and no data copied.

## Quick start

### Requirements

- CMake 3.13 or later
- C++20 compiler
- Any CMake generator (Ninja is recommended)

### Standalone build

```bash
git clone https://github.com/gubnik/iobu.git
cd iobu
cmake --preset x64-linux
cmake --build build/x64-linux --config Debug
```

### vcpkg

Add a portfile of this library to your ports directory and declare it as a dependency as
```json
{
    "name": "iobu"
}
```
Currently, IOBU is _not_ available at vcpkg registry.

### CMake FetchContent

```cmake
include(FetchContent)
FetchContent_Declare(iobu
    GIT_REPOSITORY https://github.com/gubnik/iobu.git
    GIT_TAG develop
    GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(iobu)

target_link_libraries(my_app iobu::iobu)
```

## What are buffers

In IOBU, a type is treated as _buffer-compatible_ if it exposes a
`data() noexcept -> buffer-compatible-data-pointer` and a `size() noexcept -> convertible-to-std-size_t` methods,
specialized by const-qualifiers for read and write buffers accordingly. A `buffer-compatible-data-pointer` is a pointer
to either `std::byte`, `signed char`, `unsigned char`, `char` or `void`. This makes it trivial for any library to be IOBU-compatible,
simply by following the already established STL naming convention.

IOBU also inherits Asio concept of _buffer sequences_. A _buffer sequence_ is a type which models a _bidirectional range_ the
value type of which is _buffer-compatible_. All IOBU algorithms are designed to operate on _buffer sequences_, with uniform return types
- if a function returns a `iobu::buffer_view<BufSeqTy>` you can expect it to have the exact same API whether the given sequence is a
single buffer or a range of buffers.

## Read and write buffers

IOBU buffers are separated into read and write buffers: a read buffer is a buffer that exposes the data in any form, const or not,
and write buffers require the data to be non-const - from this it follows that _any_ write buffer can be treated as a read buffer, but
_no_ read buffer can be treated as a write buffer.

## Zero-copy and lifetime guarantees

IOBU operates in terms of _non-owning buffer views_ enabling extremely efficient zero-copy usage patterns.
This, however, requires the caller to keep the underlying storage alive and at a stable address for as long as an operation may access it.
For asynchronous operations, that usually means keeping the storage alive until completion; using a coroutine-native API such as
[Boost.Capy](https://github.com/cppalliance/capy) can help organize that lifetime, but does not manage it automatically.
The caller remains responsible for ownership.
