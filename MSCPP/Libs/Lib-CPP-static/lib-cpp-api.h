#pragma once

#include <string>
#include <vector>
#include <cstddef>
#include <stdexcept>


namespace LibCpp
{
    class Widget
    {
    public:
        Widget();
        explicit Widget(std::string name);

        const std::string& name() const noexcept;
        void set_name(std::string n);

        const std::vector<int>& values() const noexcept;
        void add_value(int v);
        int  sum() const noexcept;

        void require_min_size(std::size_t n) const; // throw

    private:
        std::string name_;
        std::vector<int> values_;
    };

    Widget make_widget(std::string name, std::vector<int> init);
    std::vector<int> make_range(int start, int count);
}