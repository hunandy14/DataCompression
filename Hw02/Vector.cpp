/**********************************************************
Name : OpenRaw 2.5.2
Date : 2016/08/03
By   : CharlotteHonG
Final: 2016/12/15
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
// 初始編碼簿 - 隨機取得
imgraw & imgraw::get_org(imgraw& sou){
    srand((unsigned)time(NULL));
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
            --i;// 補回這次浪費的步數
        }
    }
    return (*this);
}
// 取得索引
imgraw & imgraw::get_idx(imgraw& sou, imgraw& ori){
    // 找出最小 差平方和 並返還編碼簿索引
    ori.min_sum = 0;// 區塊的`最小差平方和`的和歸零
    for (int i = 0; i < 4096; ++i){
        // sou的i區塊與ori每個區塊比對
        (*this)[i]=(imch)sou.block(i).dif_squ(ori);
    } this->min_avg= (double)ori.min_sum / (double)4096;
    // 4096個 `差平方和`的和平均
    // cout << "min_avg=" << this->min_avg << endl;
    return (*this);
}
// 訓練編碼簿
double imgraw::tra_code(imgraw& sou, imgraw& idx){
    // 預載區塊
    if(sou.blk_p.size()==0)
        sou.get_block();
    if((*this).blk_p.size()==0)
        (*this).get_block();
    // 平均(把sou[i]平均 後寫入tra[i])
    for (int j = 0; j < 256; ++j){ // idx索引上的編號
        // 找種類 j的位置在哪裡並求 sou[i]和
        long long int sum[16]{};
        imint cnt=0;
        // 找出相同的 j有幾個並累加 block
        for (int i = 0; i < 4096; ++i){ // idx的索引
            if (idx[i]==(imch)j){
                ++cnt;
                // 每個區塊16個點個別累加
                for (int k = 0; k < 16; ++k)
                    sum[k] += sou.blk_p[i][k];
            }
        }
        // 根據找出的數，算出平均數並填入tra
        if (cnt != 0){
            // 算出平均
            for (int i = 0; i < 16; ++i)
                sum[i] = sum[i]/cnt;
            // 把紀錄的總和平均後，填入tra[idx]
            for (unsigned i = 0; i < cnt; ++i)
                for (int k = 0; k < 16; ++k)
                    (*this).blk_p[j][k]=(imch)sum[k];
        }
    }
    // 4096個 `差平方和`的和平均
    double avg_ori = idx.min_avg;
    // 重新創建idx
    idx.get_idx(sou, (*this));
    // `差平方和`的和平均 差值
    // cout << avg_ori-idx.min_avg << endl;
    return avg_ori-idx.min_avg;
}
// 合併檔案
imgraw & imgraw::merge(imgraw& cb, imgraw& idx){
    // 寫入檔案
    for (int j= 0, c =0; j < 4096; ++j)
        this->block(j) = cb.block(idx[c++]);
    return *this;
}
//----------------------------------------------------------------
// 取得區塊預載
void imgraw::get_block(){
    imint len = (this->width/4)*(this->high/4);
    // cout << "len=" << len << endl;
    this->blk_p.resize(len);
    // blk_p 預載 len 個區塊
    for (unsigned i = 0; i < len; ++i){
        this->blk_p[i].copy(ImrBlock((*this), i));
    }
}
// 複製區塊(這裡不能是參考，第一次創建)
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