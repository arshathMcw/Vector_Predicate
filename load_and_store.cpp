#include <iostream>
#include <c7x.h>
#include <c7x_scalable.h>

using namespace c7x;
using namespace std;

int main(){
    int height = 10,width = 30;
    int32_t input[height][width],output[height][width];
    int cnt = 0;
    for(int h = 0;h < height ; h++){
        for(int w = 0;w < width;w++){
            input[h][w] = cnt++;
            // cout<<input[h][w]<<" ";
        }
        // cout<<endl;
    }
    __SE_TEMPLATE_v1 seTemplate = __gen_SE_TEMPLATE_v1();
    seTemplate.ELETYPE   = se_eletype<int_vec>::value;
    seTemplate.VECLEN    = se_veclen<int_vec>::value;
    seTemplate.DIMFMT = __SE_DIMFMT_2D;
    seTemplate.ICNT0 = width;
    seTemplate.ICNT1 = height;  
    seTemplate.DIM1 = width;
    __SE0_OPEN((void *)&input[0][0], seTemplate);
    const int vec_len = element_count_of<float_vec>::value;
    for(int32_t i = 0;i < height * ceil(width / (float)vec_len);i++){
        int_vec val = strm_eng<0, int_vec>::get_adv();
        // val.print();
    }
    __SA_TEMPLATE_v1 saTemplate = __gen_SA_TEMPLATE_v1();
    saTemplate.VECLEN  = sa_veclen<int_vec>::value;
    saTemplate.DIMFMT = __SA_DIMFMT_2D;
    saTemplate.ICNT0 = width;
    saTemplate.ICNT1 = height;  
    saTemplate.DIM1 = width;
    __SE0_OPEN((void *)&input[0][0], seTemplate);
    __SA0_OPEN(saTemplate);
    for(int32_t ctr = 0; ctr < (height * ceil(width/16.0)); ctr++) {
        int_vec vIn = strm_eng<0, int_vec>::get_adv();
        __vpred vpStore = strm_agen<0, int_vec>::get_vpred();
        int_vec * addr = strm_agen<0, int_vec>::get_adv(&output[0][0]);
        __vstore_pred(vpStore, addr, vIn);
    }
    for(int h = 0;h < height;h++){
        for(int w = 0 ;w < width;w++){
            cout<<output[h][w]<<" ";
        }
        cout<<endl;
    }
    __SE0_CLOSE();
    __SA0_CLOSE();
}
