/**********************************************************
Name : OpenRaw 2.5.2
Date : 2016/08/03
By   : CharlotteHonG
Final: 2016/10/05
**********************************************************/
#ifndef OPENRAW_CPP
#define OPENRAW_CPP


#include "OpenRAW_fun\OpenRAW"
//=========================================================
//-------------------------Structor------------------------
//=========================================================
#include "OpenRAW_fun\Structor.cpp"
//=========================================================
//------------------------ImrSize--------------------------
//=========================================================
#include "OpenRAW_fun\ImrSize.cpp"
//=========================================================
//------------------------ImrMask--------------------------
//=========================================================
#include "OpenRAW_fun\ImrMask.cpp"
//=========================================================
//------------------------ImrCoor--------------------------
//=========================================================
#include "OpenRAW_fun\ImrCoor.cpp"
//=========================================================
//------------------------imgraw---------------------------
//=========================================================
#include "OpenRAW_fun\imgraw.cpp"
//=========================================================
//------------------------Operator-------------------------
//=========================================================
#include "OpenRAW_fun\Operator.cpp"
//=========================================================
namespace imr{
#define byte 8
void imgraw::code(){
    const size_t size=3;
    imgraw img_c(ImrSize(0, 0));
    imint new_size = ceil((this->filesize*size)/byte);
    imint ori_size = this->filesize;
    img_c.resize_canvas(new_size);
    // 旗標
    int img_idx=0, bit_idx=0;
    // bit暫存
    bitset<byte> bit;
    // 讀取每個點
    for (int i = 0; i < (int)ori_size; ++i){
        // cout << "(*this)[i]=" << (bitset<byte>)((*this)[i]) << endl;
        // 讀取位元
        for (unsigned j = 0; j < size; ++j){
            // 向量編碼
            imch code = vec((*this)[i]);
            bit[bit_idx] = ((bitset<byte>)code)[j];
            // 旗標
            ++bit_idx;
            // 滿了換下一個
            if(bit_idx==byte) {
                // 輸出
                img_c[img_idx] = (imch)bit.to_ulong();
                ++img_idx;
                bit_idx=0;
            }
        }
    }
    (*this) = img_c;
}
void imgraw::uncode(){
    imgraw img_c(ImrSize(0, 0));
    const size_t size=3;
    imint new_size = ceil((this->filesize*byte)/size);
    imint ori_size = this->filesize;
    img_c.resize_canvas(new_size);
    // 旗標
    int bit_idx=0, img_idx=0;
    // bit暫存
    bitset<byte> bit;
    // 讀壓縮檔的點(點是分區的最低值)
    for (int i = 0; i < (int)ori_size; ++i){
    // for (int i = 0; i < (int)8; ++i){
        // 讀每一位元
        for (int j = 0; j < byte; ++j){
            // 起始位置
            size_t star_pos = 0;
            bit[bit_idx+star_pos] = ((bitset<byte>)((*this)[i]))[j];
            ++bit_idx;
            // 讀完一個單位
            if(bit_idx >= (int)size) {
                // 查看解出來的碼
                // cout << "bit=" << "[" << img_idx << "] " << bit << endl;
                // 解出來的碼
                imch uncode=(imch)bit.to_ulong();
                // 解向量
                img_c[img_idx] = uvec(uncode);
                // 重置
                bit_idx = 0;
                bit.reset();
                ++img_idx;
            }
        }
    }
    // cout << "img_c=" << (int)img_c[0] << endl;
    // cout << "img_c=" << (int)img_c[1] << endl;
    // cout << "img_c=" << (int)img_c[2] << endl;
    // 修復數據(取中間值)
    // bit.reset();
    // for (int i = 0; i < byte-(int)size-1; ++i){
    //     bit[i]=1;
    // }
    // for (int i = 0; i < (int)new_size; ++i){
    //     img_c[i] += (imch)bit.to_ulong();;
    //     // cout << "img_c[i]=" << (bitset<byte>)img_c[i] << endl;
    // }
    (*this) = img_c;
}
// 編碼
imch imgraw::vec(imch value){
    for(unsigned i = 0; i < c_b.size(); ++i) {
        if (value < c_b[i])
            return i;
    }return c_b.size();
}
// 解碼
imch imgraw::uvec(imch value){
    size_t idx=(size_t)value;
    if (idx == c_b.size()){
        return ((256)-(c_b[idx-1]))/2 + c_b[idx-1];
    } else if (idx > 0) {
        return ((c_b[idx])-(c_b[idx-1]))/2 + c_b[idx-1];
    }
    return c_b[idx]/2;
}





void imgraw::unscalar(imint size=4){
    imgraw img_c(ImrSize(0, 0));
    imint new_size = ceil((this->filesize*8)/size);
    imint ori_size = this->filesize;
    img_c.resize_canvas(new_size);
    // 旗標
    int bit_idx=0, img_idx=0;
    // bit暫存
    bitset<byte> bit;
    // 讀壓縮檔的點(點是分區的最低值)
    for (int i = 0; i < (int)ori_size; ++i){
        // 讀每一位元
        for (int j = 0; j < byte; ++j){
            bit[bit_idx+(byte-size)] = ((bitset<byte>)((*this)[i]))[j];
            ++bit_idx;
            // 旗標
            if(bit_idx >= (int)size) {
                // cout << "bit=" << "[" << img_idx << "] " << bit << endl;
                img_c[img_idx] = (imch)bit.to_ulong();
                bit_idx = 0;
                bit.reset();
                ++img_idx;
            }
        }
    }
    // 修復數據(取中間值)
    bit.reset();
    for (int i = 0; i < byte-(int)size-1; ++i){
        bit[i]=1;
    }
    for (int i = 0; i < (int)new_size; ++i){
        img_c[i] += (imch)bit.to_ulong();;
        // cout << "img_c[i]=" << (bitset<byte>)img_c[i] << endl;
    }
    (*this) = img_c;
}
void imgraw::scalar(imint size=4){
    imgraw img_c(ImrSize(0, 0));
    imint new_size = ceil((this->filesize*size)/byte);
    imint ori_size = this->filesize;
    img_c.resize_canvas(new_size);
    // 旗標
    int img_idx=0, bit_idx=0;
    // bit暫存
    bitset<byte> bit;
    // 讀取每個點
    for (int i = 0; i < (int)ori_size; ++i){
        // cout << "(*this)[i]=" << (bitset<byte>)((*this)[i]) << endl;
        // 讀取位元
        for (int j = byte-size; j < byte; ++j){
            bit[bit_idx] = ((bitset<byte>)((*this)[i]))[j];
            // 旗標
            ++bit_idx;
            if(bit_idx==byte) {
                // 輸出
                img_c[img_idx] = (imch)bit.to_ulong();
                ++img_idx;
                bit_idx=0;
            }
        }
    }
    (*this) = img_c;
}
void imgraw::resize_canvas(imint filesize) {
    this->filesize = (ifstream::pos_type)filesize;
    this->width = 0;
    this->high = 0;
    this->img_data.vector::resize(filesize);
}
} //imr

#endif