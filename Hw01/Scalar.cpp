/**********************************************************
Name : OpenRaw 2.5.2
Date : 2016/08/03
By   : CharlotteHonG
Final: 2016/10/05
**********************************************************/
#ifndef OPENRAW_CPP
#define OPENRAW_CPP
// using namespace imr;

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
    void imgraw::unscalar(imint size=4){
        imgraw img_c(ImrSize(0, 0));
        imint new_size = ceil((this->filesize*8)/size);
        // imint ori_size = this->filesize;
        img_c.resize_canvas(new_size);
        // 旗標
        int bit_idx=0, c=0;
        // bit暫存
        bitset<8> bit;
        // 讀取每個點
        for (int i = 0; i < (int)new_size; ++i){
            for (int j = 8-size; j < 8; ++j){
                bit[c] = ((bitset<8>)((*this)[i]))[j];
                // 旗標
                ++c;
                if(c==8) {
                    c=0;
                    ++bit_idx;
                    // 輸出
                    img_c[bit_idx] = (imch)bit.to_ulong();
                }
            }
        }
        (*this) = img_c;
    }
    void imgraw::scalar(imint size=4){
        imgraw img_c(ImrSize(0, 0));
        imint new_size = ceil((this->filesize*size)/8);
        imint ori_size = this->filesize;
        img_c.resize_canvas(new_size);
        // 旗標
        int bit_idx=0, c=0;
        // bit暫存
        bitset<8> bit;
        // 讀取每個點
        for (int i = 0; i < (int)ori_size; ++i){
            // cout << "(*this)[i]=" << (bitset<8>)((*this)[i]) << endl;
            // 讀取位元
            for (int j = 8-size; j < 8; ++j){
                bit[c] = ((bitset<8>)((*this)[i]))[j];
                // 旗標
                ++c;
                if(c==8) {
                    c=0;
                    ++bit_idx;
                    // 輸出
                    img_c[bit_idx] = (imch)bit.to_ulong();
                }
            }
        }
        (*this) = img_c;
    }
}


#endif