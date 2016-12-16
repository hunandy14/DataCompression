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
            --i;// 補回這次浪費的步數
        }
    }
}
// 取得索引
void imgraw::get_idx(string sou_name, string ori_name){
    // 開圖檔
    imgraw sou(ImrSize(256, 256));
    sou.read(sou_name);
    imgraw ori(ImrSize(64, 64));
    ori.read(ori_name);
    // 區塊的`最小差平方和`的和歸零
    ori.min_sum = 0;
    // 找出最小 差平方和 並返還編碼簿索引
    for (int i = 0; i < 4096; ++i){
        // sou的i區塊與ori每個區塊比對
        (*this)[i]=(imch)sou.block(i).dif_squ(ori);
    }ori.min_sum /= 4096;
    // 256個`差平方和`的和
    cout << "min_sum=" << ori.min_sum << endl;
}
// 訓練編碼簿
void imgraw::get_con(string sou_name, string ori_name, string idx_name){
    // 開圖檔
    imgraw sou(ImrSize(256, 256));
    sou.read(sou_name);
    imgraw ori(ImrSize(64, 64));
    ori.read(ori_name);
    imgraw idx(ImrSize(64, 64));
    idx.read(idx_name);
    // 預載區塊
    sou.get_block();
    this->get_block();
    // 平均(把sou[i]頻均 後寫入tra[i])
    for (int j = 0; j < 256; ++j){ // idx索引上的編號
        // 找索引j的位置在哪裡並求 ori[i]和
        long long int sum[16]{};
        imint cnt=0;
        // 找出相同的 j 有幾個並累加 block
        for (int i = 0; i < 4096; ++i){ // idx的索引
            if (idx[i]==j){
                ++cnt;
                // 每個區塊16個點個別累加
                for (int k = 0; k < 16; ++k){
                    sum[k] += sou.blk_p[i][k];
                }
            }
        }
        // 根據找出的算出平均數並填入tra
        if (cnt != 0){
            // 算出平均
            for (int i = 0; i < 16; ++i){
                sum[i] = sum[i]/cnt;
            }
            // 把紀錄的總和平均後填入n個tra[idx]
            for (unsigned i = 0; i < cnt; ++i){
                for (int k = 0; k < 16; ++k){
                    this->blk_p[j][k]=(imch)sum[k];
                }
            }
        }
    }
    // 區塊的`最小差平方和`的和歸零
    this->min_sum = 0;
    // // 找出最小平方差並返還編碼簿索引
    for (int i = 0; i < 4096; ++i){
        // sou的i區塊與ori每個區塊比對
        idx[i]=(imch)sou.block(i).dif_squ(*this);
    }this->min_sum /= 4096;

    // 256個`差平方和`的和
    cout << "min_sum=" << ori.min_sum << endl;
    cout << "min_sum=" << this->min_sum << endl;
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
    for (int j= 0, c =0; j < 4096; ++j){
        this->block(j) = ori.block(idx[c++]);
    }
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