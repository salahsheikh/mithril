//
// Created by ssheikh on 2021-03-01.
//

#ifndef MITHRIL_INCLUDE_MITHRIL_MESSAGE_HH
#define MITHRIL_INCLUDE_MITHRIL_MESSAGE_HH

#include <utility>
#include <typeinfo>
#include <memory>

namespace mithril {

class bad_message_cast_exception final
    : public std::exception
{
  [[nodiscard]] const char* what() const noexcept final
  {
    return "Attempted to unwrap into the incorrect type";
  }
};

class double_message_unwrap_exception final
    : public std::exception
{
  [[nodiscard]] const char* what() const noexcept final
  {
    return "Attempted to unwrap message twice";
  }
};

/// A container for holding arbitrary types implemented via RTTI & type-erasure.
/// \sa class std::any
class message_box final
{
public:
  message_box(message_box&&) = default;

  message_box& operator=(message_box&&) = default;

  message_box(const message_box&) = delete;

  message_box& operator=(const message_box&) = delete;

  /// Creates a message from the r-value reference it was passed. The message
  /// will own the given data until it is unwrapped. Do not forward the data
  /// since lvalues aren't supported.
  template<typename ValueType>
  message_box(ValueType&& value)
  {
    msg = new message_box::impl<ValueType>(std::forward<ValueType>(value));
  }

  /// Forbid initializing via lvalue reference since \ref message "message" will
  /// outlive the given reference in nearly all intended cases.
  //  template<typename ValueType>
  //  explicit message(const ValueType&) = delete;

  template<typename ValueType>
  [[nodiscard]] bool holds() const
  {
    return msg->type() == typeid(ValueType);
  }

  template<typename ValueType>
  [[nodiscard]] ValueType&& unwrap()
  {
    if (!holds<ValueType>()) [[unlikely]] {
      throw bad_message_cast_exception();
    }
    if (unwrapped) [[unlikely]] {
      throw double_message_unwrap_exception();
    }
    unwrapped = true;
    return std::move(dynamic_cast<impl <ValueType>*>(msg)->get());
  }

  ~message_box()
  {
    delete msg;
  }

private:
  class placeholder
  {
  public:
    [[nodiscard]] virtual const std::type_info& type() const = 0;

    virtual ~placeholder() = default;
  };

  template<typename T>
  class impl final
      : virtual public placeholder
  {
  private:
    T value;
  public:
    explicit impl(T&& arg)
        : value(std::move(arg))
    {
    }

    [[nodiscard]] const std::type_info& type() const override
    {
      return typeid(T);
    }

    T&& get()
    {
      return std::move(value);
    }
  };

  placeholder* msg {nullptr};
  bool unwrapped {false};
};

using message = std::unique_ptr<message_box>;

template<typename ValueType>
message create_message(ValueType&& val)
{
  return std::make_unique<message_box>(std::forward<ValueType>(val));
}

}

#endif //MITHRIL_INCLUDE_MITHRIL_MESSAGE_HH
