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
// 位元組長度
const size_t byte=8;
// 量化
void imgraw::scalar(imgraw img, size_t size=4){
    // 調整大小
    size_t new_size = ceil((img.filesize*size)/8);
    (*this).resize_canvas(new_size);
    // 旗標
    size_t img_idx=0, bit_idx=0;
    // bit暫存
    bitset<byte> bit;
    // 讀取每個點
    for (unsigned i = 0; i < img.filesize; ++i){
        // cout << "img[i]=" << (bitset<byte>)(img[i]) << endl;
        // 讀取位元起始位置
        size_t read_idx = byte-size;
        // 讀取位元
        for (unsigned j = read_idx; j < byte; ++j){
            bit[bit_idx] = ((bitset<byte>)img[i])[j];
            // 旗標
            ++bit_idx;
            // 滿了換下一個
            if(bit_idx==byte) {
                // 輸出
                (*this)[img_idx] = (imch)bit.to_ulong();
                ++img_idx;
                bit_idx=0;
            }
        }
    }
}
// 解量化
void imgraw::unscalar(imgraw img_vec, size_t size=4){
    // 調整大小
    size_t new_size = ceil((img_vec.filesize*byte)/size);
    (*this).resize_canvas(new_size);
    // 旗標
    size_t bit_idx=0, img_idx=0;
    // bit暫存
    bitset<byte> bit;
    // 讀壓縮檔的點(點是分區的最低值)
    for (unsigned i = 0; i < img_vec.filesize; ++i){
    // for (unsigned i = 0; i < 3; ++i){
        // 讀每一位元
        for (unsigned j = 0; j < byte; ++j){
            // 寫入的位元起始位置
            size_t write_idx = bit_idx+(byte-size);
            bit[write_idx] = ((bitset<byte>)img_vec[i])[j];
            ++bit_idx;
            // 讀完一個單位，寫入
            if(bit_idx >= size) {
                // cout << "bit=" << "[" << img_idx << "] " << bit << endl;
                // 修正數據(取中間值)
                for (unsigned k = 0; k < byte-size-1; ++k)
                    bit.set(k, 1);
                // cout << "bit=" << "[" << img_idx << "] " << bit << endl;
                // 寫入
                (*this)[img_idx] = (imch)bit.to_ulong();
                bit_idx = 0;
                bit.reset();
                ++img_idx;
            }
        }
    }
}
// 重設大小
void imgraw::resize_canvas(size_t filesize) {
    this->filesize = (ifstream::pos_type)filesize;
    this->width = 0;
    this->high = 0;
    this->img_data.vector::resize(filesize);
}
} // imr


#endif