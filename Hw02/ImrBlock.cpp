/*****************************************************************
Name : 
Date : 2016/12/08
By   : CharlotteHonG
Final: 2016/12/08
*****************************************************************/
#define BlkSize 16
namespace imr {
imgraw::ImrBlock::ImrBlock():img_p(BlkSize){}
imgraw::ImrBlock::ImrBlock(imgraw& img, imint pos):img_p(BlkSize){
    imint Sidelen = sqrt(BlkSize);
    // 取得對應的 x, y 位置
    imint pos_y=((pos/(img.high/Sidelen))*Sidelen);
    imint pos_x=((pos%(img.width/Sidelen))*Sidelen);
    // 存入參考
    for (unsigned j=0, c=0; j < Sidelen; ++j){
        for (unsigned i = 0; i < Sidelen; ++i){
            this->img_p[c++]=&img.at2d(j+pos_y, i+pos_x);
        }
    }
}
void imgraw::ImrBlock::info(){
    imint Sidelen = sqrt(BlkSize);
    for (unsigned i = 0, c = 0; i < Sidelen; ++i){
        for (unsigned j = 0; j < Sidelen; ++j){
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
// this(sou)的單一區塊與 img(ori)每個區塊比對
imint imgraw::ImrBlock::dif_squ(imgraw& img){
    imint Sidelen = sqrt(BlkSize);
    imint len = (img.width/Sidelen) * (img.high/Sidelen);
    // 最像的位置索引
    imint idx=0;
    // 取得區塊預載
    if(img.blk_p.size()==0){
        img.get_block();
    }
    // 比對 img 內的區塊，找出最小差平方和的位置
    long long int min=-1;
    for (unsigned j = 0; j < len; ++j){
        long int temp= 0;
        // 建立區塊(註解掉的是沒優化的)
        // auto&& img_b=img.block(j);
        auto&& img_b= img.blk_p[j];
        // 計算差平方和
        for (unsigned i = 0; i < BlkSize; ++i){
            temp += pow(((int)(*this)[i]
                - (int)img_b[i]), 2);
        }
        // 找 差平方和 最小的位置
        if(temp < min or min==-1){
            min= temp;
            idx= j;
        }
    }
    // 累加差平方和
    img.min_sum+=min;
    return idx;
}
} // imr