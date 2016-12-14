/*****************************************************************
Name : 
Date : 2016/12/08
By   : CharlotteHonG
Final: 2016/12/08
*****************************************************************/
namespace imr {
imgraw::ImrBlock::ImrBlock(){
    this->img_p.resize(16);
}
imgraw::ImrBlock::ImrBlock(imgraw& img, imint pos){
    // 取得對應的 x, y 位置
    imint pos_y=((pos/(img.high/4))*4);
    imint pos_x=((pos%(img.width/4))*4);
    this->img_p.resize(16);
    // 存入參考
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
// 複製實際地址參考到的內容物
imgraw::ImrBlock& imgraw::ImrBlock::operator=(const ImrBlock& obj){
    if (this == &obj)
        return *this;
    for (unsigned i = 0; i < this->img_p.size(); ++i){
        *(this->img_p[i]) = *(obj.img_p[i]);
    }
    return *this;
}
// 原本的等號功能
imgraw::ImrBlock& imgraw::ImrBlock::copy(const ImrBlock& obj){
    if (this == &obj)
        return *this;
    this->img_p = obj.img_p;
    return *this;
}
// this的單一區塊與 img(ori)每個區塊比對
imint imgraw::ImrBlock::dif_squ(imgraw& img){
    // 區塊點的 差平方和 算一次要存下來
    // vector<long int> img_arr(256);
    long int num, min=-1;
    imint idx=0;
    // 比對 img 內的區塊，找出最小差平方和的位置
    for (int j = 0; j < 256; ++j, num=0){
        // 建立區塊
        // auto&& img_b=img.block(j);
        auto&& img_b=img.blk_p[j];
        // 計算差平方和
        for (int i = 0; i < 16; ++i){
            num += pow(((int)(*this)[i]
                - (int)img_b[i]), 2);
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