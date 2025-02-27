    #include <iostream>
    #include <c7x.h>
    #include <c7x_scalable.h>

    using namespace c7x;
    using namespace std;
    int main(){
    int height = 18,width = 18,cnt = 1;
    int32_t mat1[height][width],mat2[height][width],output[height][width];
    for(int h = 0;h < height;h++){
        for(int w = 0;w  < width;w++){
            mat1[h][w] = rand() % 10;
            mat2[h][w] = (rand() % 21) - 10;
        }
    }

    for(int h = 0;h < height;h++){
        for(int w = 0;w  < width;w++){
            output[h][w] = 0;
        }
    }

    __SE_TEMPLATE_v1 seTemplate = __gen_SE_TEMPLATE_v1();
    seTemplate.ELETYPE   = se_eletype<int_vec>::value;
    seTemplate.VECLEN    = se_veclen<int_vec>::value;
    seTemplate.DIMFMT = __SE_DIMFMT_2D;
    seTemplate.ICNT0 = width;
    seTemplate.ICNT1 = height;  
    seTemplate.DIM1 = width;
    __SE0_OPEN((void *)&mat1[0][0], seTemplate);
    __SE1_OPEN((void *)&mat2[0][0], seTemplate);

    __SA_TEMPLATE_v1 saTemplate = __gen_SA_TEMPLATE_v1();
    saTemplate.VECLEN  = sa_veclen<int_vec>::value;
    saTemplate.DIMFMT = __SA_DIMFMT_2D;
    saTemplate.ICNT0 = width;
    saTemplate.ICNT1 = height;  
    saTemplate.DIM1 = width;
    __SA0_OPEN(saTemplate);
    const int vec_len = element_count_of<float_vec>::value;
    for(int idx = 0;idx < height * ceil(width / (float)vec_len) ;idx++){
        int_vec vIn1 = strm_eng<0, int_vec>::get_adv();
        int_vec vIn2 = strm_eng<1, int_vec>::get_adv();
        __vpred mask = __vcmpgtw_vvp(vIn2,(int_vec)0);
        int_vec result = __vaddcw_pvv(mask,vIn2,vIn1);
        __vpred vpStore = strm_agen<0, int_vec>::get_vpred();
        int_vec * addr = strm_agen<0, int_vec>::get_adv(&output[0][0]);
        __vstore_pred(vpStore, addr, result);
    }
    cout<<"Mat1 and Mat2: "<<endl;
    for(int h = 0; h < height;h++){
        for(int w = 0;w < width;w++){
            cout<<mat1[h][w]<<"+"<<mat2[h][w]<<" ";
        }
        cout<<endl;
    }

    cout<<"Output : "<<endl;
    for(int h = 0; h < height;h++){
        for(int w = 0;w < width;w++){
            cout<<output[h][w]<<" ";
        }
        cout<<endl;
    }
}