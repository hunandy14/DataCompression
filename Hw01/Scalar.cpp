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
        int bit_idx=0, img_idx=0;
        // bit暫存
        bitset<8> bit;

            cout << "ori[0]=" << ((bitset<8>)((*this)[0])) << endl;
            cout << "ori[1]=" << ((bitset<8>)((*this)[1])) << endl;
            cout << "ori[3]=" << ((bitset<8>)((*this)[2])) << endl;
        // 讀壓縮檔的點
        for (int i = 0; i < 3; ++i){
            // 讀每一位元
            for (int j = 0; j < 8; ++j){
                // cout << "j=" << j << endl;
                // bit[j] = ((bitset<8>)((*this)[img_idx]))[bit_idx];
                // 旗標
                ++bit_idx;
                if(bit_idx >= (int)size) {
                    bit_idx = 0;
                    // (*this)[img_idx] = (imch)bit.to_ulong();
                    cout << "bit[img_idx]=" << img_idx << "=" << bit << endl;
                    ++img_idx;
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
        int img_idx=0, bit_idx=0;
        // bit暫存
        bitset<8> bit;
        // 讀取每個點
        for (int i = 0; i < (int)ori_size; ++i){
            // cout << "(*this)[i]=" << (bitset<8>)((*this)[i]) << endl;
            // 讀取位元
            for (int j = 8-size; j < 8; ++j){
                bit[bit_idx] = ((bitset<8>)((*this)[i]))[j];
                // 旗標
                ++bit_idx;
                if(bit_idx==8) {
                    // 輸出
                    img_c[img_idx] = (imch)bit.to_ulong();
                    ++img_idx;
                    bit_idx=0;
                }
            }
        }
        (*this) = img_c;
    }
}


#endif