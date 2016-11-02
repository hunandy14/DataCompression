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
namespace imr {
    // 初始編碼簿 - 隨機取得
    void imgraw::get_org(const imgraw &source){
        srand((unsigned)time(NULL));
        // 畫布長度
        // int len = this->high*this->width;
        // cout << "len=" << this->width << endl;
        // int len_sou = source.high*source.width;
        // 建立臨時畫布
        ImrSize source_size(source.high, source.width);
        ImrMask round(source_size);
        // 儲存座標
        ImrCoor idx[256];
        // 隨機找256個點寫入1
        for (int i = 0; i < 256; ++i){
            int y = rand_int(0,63);
            int x = rand_int(0,63);
            if (round.at2d(y, x) != 1){
                round.at2d(y, x) = (imch)1;
                idx[i].x = x*4;
                idx[i].y = y*4;
            }else {--i;}
            // idx[i].info();
        }
        // 寫入編碼簿
        for (int j=0, c=0; j < 16; ++j){
            for (int i = 0; i < 16; ++i){
                // 編碼簿上的區塊位置
                ImrCoor ori(j*4, i*4);
                // mask位移位置
                ImrCoor shi(0, 0);
                // 以4*4為單位寫入
                for (int k = 0; k < 4; ++k){
                    for (int l = 0; l < 4; ++l){
                        ImrCoor mas(k, l);
                        // 編碼簿上的區塊 << 座標上的區塊
                        this->maskVal(ori, mas, shi) = 
                        source.maskVal(idx[c], mas, shi);
                    }
                } c++;
            }
        }
    }

    void imgraw::training(const imgraw &origin){
        
    }










    // 取亂數(不包含up)
    int imgraw::rand_int(int low, int up){
        int temp;
        if (low < 0)
            low-=1;
        temp = (int)((rand() / (RAND_MAX+1.0)) * (up - low) + low);
        return temp;
    }
} // imr

#endif