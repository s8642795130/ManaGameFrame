#pragma once

#include <tuple>
#include <type_traits>
#include <vector>
#include <memory>

#include "StructSchema.h"
#include "DataType.h"
#include "ByteBuffer.h"

namespace detail
{
    template <typename Fn, typename Tuple, std::size_t... I>
    inline constexpr void ForEachTuple(Tuple&& tuple,
        Fn&& fn,
        std::index_sequence<I...>)
    {
        using Expander = int[];
        (void)Expander
        {
            0, ((void)fn(std::get<I>(std::forward<Tuple>(tuple))), 0)...
        };
    }

    template <typename Fn, typename Tuple>
    inline constexpr void ForEachTuple(Tuple&& tuple, Fn&& fn)
    {
        ForEachTuple(
            std::forward<Tuple>(tuple), std::forward<Fn>(fn),
            std::make_index_sequence<std::tuple_size<std::decay_t<Tuple>>::value>{});
    }

    template <typename T>
    struct is_field_pointer : std::false_type {};

    template <typename C, typename T>
    struct is_field_pointer<T C::*> : std::true_type {};

    template <typename T>
    constexpr auto is_field_pointer_v = is_field_pointer<T>::value;

}  // namespace detail

namespace data_fn
{
    template <typename Name>
    void fn(std::string& data, Name&& field, std::shared_ptr<ByteBuffer>& byte_buffer)
    {
        int str_len = byte_buffer->GetInt();
        data = byte_buffer->GetString(str_len);
    }

    template <typename Name>
    void fn(int& data, Name&& field, std::shared_ptr<ByteBuffer>& byte_buffer)
    {
        data = byte_buffer->GetInt();
    }

    template <typename Name>
    void fn(char& data, Name&& field, std::shared_ptr<ByteBuffer>& byte_buffer)
    {
        data = byte_buffer->GetChar();
    }

    template <typename Name>
    void fn(bool& data, Name&& field, std::shared_ptr<ByteBuffer>& byte_buffer)
    {
        data = byte_buffer->GetBool();
    }

    template <typename Name>
    void fn(std::vector<int>& data, Name&& field, std::shared_ptr<ByteBuffer>& byte_buffer)
    {
        int vec_len = byte_buffer->GetInt();
        for (auto i = 0; i < vec_len; ++i)
        {
            int element = byte_buffer->GetInt();
            data.push_back(element);
        }
    }

    template <typename Name>
    void fn(std::vector<char>& data, Name&& field, std::shared_ptr<ByteBuffer>& byte_buffer)
    {
        // allocate memory to vector
        int vec_len = byte_buffer->GetInt();
        data.reserve(vec_len);

        // copy data to vector
        auto cur_pos = byte_buffer->GetCurPos();
        std::memcpy(data.data(), byte_buffer->GetBuffer() + cur_pos, vec_len);
        
        // step buffer to new position
        byte_buffer->StepBufferPos(vec_len);
    }

    template <typename T, typename Name>
    void fn(std::vector<T>& data, Name&& field, std::shared_ptr<ByteBuffer>& byte_buffer)
    {
        int vec_len = byte_buffer->GetInt();
        for (auto i = 0; i < vec_len; ++i)
        {
            T t;
            UnpackStructForEachField(t, byte_buffer);
            data.push_back(t);
        }
    }
}

template <typename T>
inline constexpr void UnpackStructForEachField(T& value, std::shared_ptr<ByteBuffer>& byte_buffer)
{
    // get a tuple
    constexpr auto struct_schema = StructSchema<std::decay_t<T>>();

    // const std::tuple<bool SimpleStruct::* ,const char *>
    static_assert(std::tuple_size<decltype(struct_schema)>::value != 0,
        "StructSchema<T>() for type T should be specialized to return "
        "FieldSchema tuples, like ((&T::field, field_name), ...)");

    // each tuple ()
    detail::ForEachTuple(struct_schema, [&value, &byte_buffer](auto&& field_schema)
        {
            using FieldSchema = std::decay_t<decltype(field_schema)>;
            static_assert(std::tuple_size<FieldSchema>::value >= 2 && detail::is_field_pointer_v<std::tuple_element_t<0, FieldSchema>>, "FieldSchema tuple should be (&T::field, field_name)");

            data_fn::fn(value.*(std::get<0>(std::forward<decltype(field_schema)>(field_schema))), std::get<1>(std::forward<decltype(field_schema)>(field_schema)), byte_buffer);

            // std::cout << value.*(std::get<0>(std::forward<decltype(field_schema)>(field_schema))) << std::endl;
            // std::cout << static_cast<int>(std::get<1>(std::forward<decltype(field_schema)>(field_schema))) << std::endl;
        }
    );
}