1. To create a own mask
```cpp
__vpred mask = __vpred{0xFFFFFFFF00000000ULL};
mask.print();
```
1. For Compare Greater Than
```cpp
__vpred compare = __vcmpgtw_vvp(vIn,(int_vec)0);
```
