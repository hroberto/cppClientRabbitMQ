#ifndef  __MARKVALUE_H__
#define __MARKVALUE_H__

#include <ostream>

namespace libapp {


template <class T>
class MarkValue {
public:
    MarkValue() {}
    // MarkValue(auto v) { value_ = T(v);  }
    MarkValue(const T& v) : value_(v) {}
    MarkValue(const MarkValue<T>& o) : value_{o.value_}, mark_{o.mark_} {}
    MarkValue(MarkValue<T>&& o) : value_{ std::move( o.value_) }, mark_{o.mark_} {}


    // bool operator<(MarkValue<T>& other ) noexcept { return ( value_ < other.value_ ); }
    bool operator<(const MarkValue<T>& other ) const noexcept { return ( value_ < other.value_ ); }

    // bool operator<(const MarkValue<T>& other ) const { return ( value_ < other.value_ ); }
    // bool operator<(const T& other ) const { return ( value_ < other ); }
    // bool operator<(const T& other ) const { return ( value_ < other ); }


    bool operator==(const MarkValue<T>& other)  { return value_ == other.value_; }
    // bool operator==(const MarkValue<const T>& other)  { return value_ == other.value_; }

    MarkValue<T>& operator=(const MarkValue<T>& v) { value_ = v.value_ ; mark_ = v.mark_; return *this; }
    MarkValue<T>& operator=(MarkValue<T>& v) { value_ = v.value_ ; mark_ = v.mark_; return *this; }

    bool operator!=(const MarkValue<T>& other)  { return value_ != other.value_; }
    // bool operator<=(const MarkValue<T>& other ) const { return value_ <= other.value_; }
    // bool operator>(const MarkValue<T>& other ) const { return value_ >   other.value_; }
    // bool operator>=(const MarkValue<T>& other ) const { return value_ >= other.value_; }



    T& get() & noexcept { return value_; }
    const T& get() const& noexcept { return value_; }

    T& operator()() & noexcept { return value_; }
    const T& operator()() const& noexcept { return value_; }

    constexpr const T& operator*() const& noexcept  { return value_; }
    constexpr T& operator*() & noexcept { return value_; }

    const T* operator->() const& noexcept { return &value_; }
    T* operator->() & noexcept { return &value_; }

    const bool& getMark() const { return mark_; }
    void mark() { mark_ = true; }
    void unmark() { mark_ = false; }
    

    T::iterator begin() noexcept { return value_.begin(); }
    T::const_iterator begin() const noexcept { return value_.begin(); }

    T::iterator end() noexcept { return value_.end(); }
    T::const_iterator end() const noexcept { return value_.end(); }

private:
    T value_{};
    bool mark_{false};
};

template <class T>
std::ostream& operator<<(std::ostream& os, genialcpp::MarkValue<T>& obj) {
    os << obj.get();
    return os;
}
template <class T>
std::ostream& operator<<(std::ostream& os, const genialcpp::MarkValue<T>& obj) {
    os << obj.get();
    return os;
}

}  // namespace genialcpp



#endif  // __MARKVALUE_H__