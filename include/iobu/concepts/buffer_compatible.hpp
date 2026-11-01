#pragma once

#include <concepts>
#include <cstddef>
#include <type_traits>

namespace iobu
{

namespace detail
{

template <typename ValTy>
concept buffer_compatible_value = std::same_as<std::remove_cvref_t<ValTy>, void> ||
                                  std::same_as<std::remove_cvref_t<ValTy>, signed char> ||
                                  std::same_as<std::remove_cvref_t<ValTy>, unsigned char> ||
                                  std::same_as<std::remove_cvref_t<ValTy>, char> ||
                                  std::same_as<std::remove_cvref_t<ValTy>, std::byte>;

template <typename PtrTy>
concept write_buffer_data_pointer =
    std::is_pointer_v<PtrTy> && (not std::is_const_v<std::remove_pointer_t<PtrTy>>) &&
    buffer_compatible_value<std::remove_pointer_t<PtrTy>>;

template <typename PtrTy>
concept read_buffer_data_pointer =
    std::is_pointer_v<PtrTy> && (std::is_const_v<std::remove_pointer_t<PtrTy>>) &&
    buffer_compatible_value<std::remove_pointer_t<PtrTy>>;

} // namespace detail

template <typename Ty>
concept write_buffer_compatible = requires(Ty &buf) {
    { buf.data() } noexcept -> detail::write_buffer_data_pointer;
    { buf.size() } noexcept -> std::convertible_to<std::size_t>;
};

template <typename BufTy>
concept read_buffer_compatible = write_buffer_compatible<BufTy> or requires(BufTy const &buf) {
    { buf.data() } noexcept -> detail::read_buffer_data_pointer;
    { buf.size() } noexcept -> std::convertible_to<std::size_t>;
};

} // namespace iobu
