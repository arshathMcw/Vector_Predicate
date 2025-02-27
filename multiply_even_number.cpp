#include <iostream>
#include <c7x.h>
#include <c7x_scalable.h>

using namespace c7x;
using namespace std;
int main(){
    int height = 16,width = 18,cnt = 1;
    int32_t mat1[height][width],mat2[height][width],mat3[height][width],output[height][width];
    for(int h = 0;h < height;h++){
        for(int w = 0;w  < width;w++){
            mat1[h][w] = (rand() % 9) + 1;
            mat2[h][w] = (rand() % 9) + 1;
        }
    }


    for(int h = 0;h < height;h++){
        for(int w = 0;w  < width;w++){
            output[h][w] = mat1[h][w];
        }
    }

    __SE_TEMPLATE_v1 seTemplate = __gen_SE_TEMPLATE_v1();
    seTemplate.ELETYPE   = se_eletype<int_vec>::value;
    seTemplate.VECLEN    = se_veclen<int_vec>::value;
    seTemplate.DIMFMT = __SE_DIMFMT_2D;
    seTemplate.ICNT0 = width;
    seTemplate.ICNT1 = height;  
    seTemplate.DIM1 = width;
    __SA_TEMPLATE_v1 saTemplate = __gen_SA_TEMPLATE_v1();
    saTemplate.VECLEN  = sa_veclen<int_vec>::value;
    saTemplate.DIMFMT = __SA_DIMFMT_2D;
    saTemplate.ICNT0 = width;
    saTemplate.ICNT1 = height;  
    saTemplate.DIM1 = width;
    __SE0_OPEN((void *)&mat1[0][0], seTemplate);
    __SE1_OPEN((void *)&mat2[0][0], seTemplate);
    __SA0_OPEN(saTemplate);
    const int vec_len = element_count_of<float_vec>::value;
    cout<<"Matrix 1 : "<<endl;
    for(int h = 0; h < height;h++){
        for(int w = 0;w < width;w++){
            cout<<mat1[h][w]<<" ";
        }
        cout<<endl;
    }
    cout<<endl;
    cout<<"Matrix 2 : "<<endl;
    for(int h = 0; h < height;h++){
        for(int w = 0;w < width;w++){
            cout<<mat2[h][w]<<" ";
        }
        cout<<endl;
    }
    for(int idx = 0;idx < height * ceil(width / (float)vec_len) ;idx++){
        int_vec vIn1 = strm_eng<0, int_vec>::get_adv();
        int_vec vIn2 = strm_eng<1, int_vec>::get_adv();
        int_vec res = __vmpyww_vvv(vIn1,vIn2);
        int_vec even = __vandnw_vvv(vIn1,(int_vec)1);
        // 6 -> 110
        // 1 -> 001
        // AND = 0 , NAND = 1


        __vpred mask2 = strm_agen<0, int_vec>::get_vpred();
        __vpred mask = __vcmpeqw_vvp(even, int_vec(1)); 
        __vpred final_mask =  __and_ppp(mask, mask2);

        int_vec * addr = strm_agen<0, int_vec>::get_adv(&output[0][0]);
        __vstore_pred(final_mask, addr, res);
        
    }
    __SE0_CLOSE();
    __SE1_CLOSE();
    __SA0_CLOSE();
    cout<<"Output : "<<endl;
    for(int h = 0; h < height;h++){
        for(int w = 0;w < width;w++){
            cout<<output[h][w]<<" ";
        }
        cout<<endl;
    }
}

