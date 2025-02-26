1. To create a own mask
```cpp
__vpred mask = __vpred{0xFFFFFFFF00000000ULL};
mask.print();
```
1. For Compare Greater Than
```cpp
__vpred compare = __vcmpgtw_vvp(vIn,(int_vec)0);
```
1. Addition with predicate
```cpp
int_vec result = __vaddcw_pvv(mask,vIn2,vIn1); // mask is for vIn2
```
int_vec even = __vandnw_vvv(vIn1,(int_vec)1);

1. For Compare Equal To
```cpp
__vpred mask = __vcmpeqw_vvp(even,(int_vec)1);
```