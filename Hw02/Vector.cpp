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
namespace imr {
    imgraw::~imgraw()=default;
} // imr
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
#include "ImrBlock.cpp"
//=========================================================
//------------------------Operator-------------------------
//=========================================================
#include "OpenRAW_fun\Operator.cpp"
//=========================================================
namespace imr {
//----------------------------------------------------------------
// 取得索引
void imgraw::get_idx(string sou_name, string ori_name){
    // 開圖檔
    imgraw sou(ImrSize(256, 256));
    sou.read(sou_name);
    imgraw ori(ImrSize(64, 64));
    ori.read(ori_name);
    // 找出最小平方差
    for (int i = 0; i < 4096; ++i){
        // sou的i區塊與ori每個區塊比對
        (*this)[i]=(imch)sou.block(i).dif_seq(ori);
    }
}
// 合併檔案
void imgraw::merge(string ori_name, string idx_name){
    ImrSize size(64, 64);
    // 開圖檔
    imgraw ori(size);
    ori.read(ori_name);
    imgraw idx(size);
    idx.read(idx_name);
    // 寫入檔案
    for (int j= 0, c =0; j < 4096; ++j)
        this->block(j) = ori.block(idx[c++]);
}
// 初始編碼簿 - 隨機取得
void imgraw::get_org(string sou_name){
    srand((unsigned)time(NULL));
    // 讀檔
    ImrSize size(256, 256);
    imgraw sou(size);
    sou.read(sou_name);
    // 隨機找256個點寫入1
    vector<imint> arr(4096);
    for (int i = 0; i < 256; ++i){
        imint rand = rand_int(0,4096);
        // 找過的標記避免重複
        if( arr[rand] == 0){
            arr[rand] = 1;
            this->block(i) = sou.block(rand);
        }
        // 發現已經找過跳過一個
        else{
            // 補回這次浪費的步數
            --i;
        }
    }
}
//----------------------------------------------------------------
// 複製區塊
imgraw::ImrBlock imgraw::block(imint pos){ 
    return ImrBlock((*this), pos);
}
// 取亂數(不包含up)
int imgraw::rand_int(int low, int up){
    int temp;
    if (low < 0)
        low-=1;
    temp = (int)((rand() / (RAND_MAX+1.0)) * (up - low) + low);
    return temp;
}
//----------------------------------------------------------------
} // imr
#endif