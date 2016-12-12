/*****************************************************************
Name : 
Date : 2016/12/08
By   : CharlotteHonG
Final: 2016/12/08
*****************************************************************/
namespace imr {
imgraw::ImrBlock::ImrBlock(imgraw& img, imint pos){
    this->pos_y=((pos/(img.high/4))*4);
    this->pos_x=((pos%(img.width/4))*4);
    this->img_p.resize(16);
    for (int j=0, c=0; j < 4; ++j){
        for (int i = 0; i < 4; ++i){
            this->img_p[c++]=&img.at2d(j+pos_y, i+pos_x);
        }
    }
}
void imgraw::ImrBlock::info(){
    for (int i = 0, c = 0; i < 4; ++i){
        for (int j = 0; j < 4; ++j){
            cout << setw(4) << (int)*(this->img_p[c++]);
        }cout << endl;
    }
}
imch& imgraw::ImrBlock::operator[](const size_t __n){
    return *(this->img_p[__n]);
}
const imch& imgraw::ImrBlock::operator[](const size_t __n) const{
    return *(this->img_p[__n]);
}
void imgraw::ImrBlock::operator=(ImrBlock b){
    for (int i = 0; i < 16; ++i){
        *(this->img_p[i])=*(b.img_p[i]);
    }
}

// this的單一區塊與 img(ori)每個區塊比對
int imgraw::ImrBlock::dif_seq(imgraw& img){
    // 區塊點的 差平方和 算一次要存下來
    vector<long int> img_arr(256);
    long int num, min=-1;
    imint idx=0;
    // 比對 img 內的區塊，找出最小差平方和的位置
    for (int j = 0; j < 256; ++j, num=0){
        // img(ori)區塊
        // 計算 差平方和
        for (int i = 0; i < 16; ++i){
            num+=pow(((int)(*this)[i] - 
                (int)img.block(j)[i]), 2);
        }
        // 找 差平方和 最小的位置
        if(num < min or min==-1){
            min=num;
            idx=j;
        }
    }
    return idx;
}
} // imr