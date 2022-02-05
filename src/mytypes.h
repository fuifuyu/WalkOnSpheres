#pragma once

#ifndef MY_TYPES_H
#define MY_TYPES_H

#include <array>

namespace MyTypes{
    template<size_t N>
    using arrayd = std::array<double, N>;
}

#endif