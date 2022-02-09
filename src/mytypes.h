#pragma once

#ifndef MY_TYPES_H
#define MY_TYPES_H

#include <array>

namespace MyTypes{
    template<size_t N>
    using arrayd = std::array<double, N>;

    template<size_t N>
    arrayd<N> subtract(const arrayd<N> &a, const arrayd<N> &b){
        if(N == 2) return {{a[0]-b[0],a[1]-b[1]}};
        arrayd<N> res(a);
        for(int i = 0; i < N; i++){
            res[i] -= b[i];
        }
        return res;
    };

    template<size_t N>
    arrayd<N> add(const arrayd<N> &a, const arrayd<N> &b){
        if(N == 2) return {{a[0]+b[0],a[1]+b[1]}};
        arrayd<N> res(a);
        for(int i = 0; i < N; i++){
            res[i] += b[i];
        }
        return res;
    };

    template<size_t N>
    arrayd<N> times(const arrayd<N> &a, double b){
        if(N == 2) return {{a[0]*b,a[1]*b}};
        arrayd<N> res(a);
        for(int i = 0; i < N; i++){
            res[i] *= b;
        }
        return res;
    };
}

#endif