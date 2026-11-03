## Why does write buffer sequence require use of mutable reference, and read sequence requires use of const reference?

Because otherwise, `std::string` is never a valid write buffer since `std::string::data() const&` returns a `char const *`,
same goes for vectors. Types with const-indifferent `data()` are generally already reference types, like Asio's buffers or `std::span`.
Supporting a subset of possible types for the sake of easier in-library implementation does not sit right with me.
If everything is attempted to be made `const &`, then the following code becomes ill-formed:
```cpp
std::string src = "Hello!";
std::vector<std::byte> dest;
dest.resize(1'024, 0);
std::size_t n = iobu::buffers_copy(src, dest);
```
