#include "stdafx.h"
#include "lib-cpp-api.h"


namespace LibCpp
{
    Widget::Widget() : name_("default") { values_.reserve(16); }
    Widget::Widget(std::string name) : name_(std::move(name)) { values_.reserve(16); }

    const std::string& Widget::name() const noexcept { return name_; }
    void Widget::set_name(std::string n) { name_ = std::move(n); }

    const std::vector<int>& Widget::values() const noexcept { return values_; }
    void Widget::add_value(int v) { values_.push_back(v); }

    int Widget::sum() const noexcept
    {
        return std::accumulate(values_.begin(), values_.end(), 0);
    }

    void Widget::require_min_size(std::size_t n) const
    {
        if (values_.size() < n)
            throw std::runtime_error("Widget::require_min_size failed");
    }

    Widget make_widget(std::string name, std::vector<int> init)
    {
        Widget w(std::move(name));
        for (int x : init) w.add_value(x);
        return w;
    }

    std::vector<int> make_range(int start, int count)
    {
        std::vector<int> v;
        v.reserve(count > 0 ? (std::size_t)count : 0);
        for (int i = 0; i < count; ++i) v.push_back(start + i);
        return v;
    }
}