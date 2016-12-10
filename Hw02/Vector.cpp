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
// 找相似 sou[0]比對 *this[all]
    // comp(區塊頭參考)
imint imgraw::comp(imgraw& sou){
    imint idx=0;
    imgraw temp= (*this);
    for (int i = 0; i < 256; ++i){
        (*this)[i]= (imch)temp.comp_one(sou, i);
    }
    return idx;
}
// 找單一點
imint imgraw::comp_one(imgraw& sou, imint sou_idx){
    // 取數值(第幾個區塊, 第幾個點)
    auto&& Bf= [&](imint i, imint idx){
        int y= idx/4;
        int x= idx%4;
        return this->block(i, y, x);
    };
    auto&& Bf2= [&](imint i, imint idx){
        int y= idx/4;
        int x= idx%4;
        return sou.block(i, y, x);
    };
    // 使用範例
    // cout << (int)Bf(0, 10) << endl;

    // 找最相似
    int temp=pow((int)(Bf(0, 0)-Bf2(0, 0)), 2);
    int samll_idx=0;
    for (int j= 0, sum= 0; j < 256; ++j){
        sum= 0;
        // 算術平均差
        for (int i = 0; i < 16; ++i){
            sum+= pow((int)(Bf(j, i)-Bf2(sou_idx, i)), 2);
        }
        if (sum < temp){
            // cout << sum << " " << endl;
            temp=sum;
            samll_idx=j;
        }
    }
    // cout << "samll_idx=" << samll_idx << endl;
    return samll_idx;
}

// 訓練(返回找好的idx)
void imgraw::training(imgraw& sou, string ori){
    this->read(ori);
    this->comp(sou);
}
//----------------------------------------------------------------
// 讀區塊(返回ref)
imch & imgraw::block2(imint pos, imint idx){
    imint blk_y=((pos/64)*4);
    imint blk_x=((pos%64)*4);
    return this->maskVal(ImrCoor(blk_y, blk_x), 
        ImrCoor(idx/4, idx%4), ImrCoor(0, 0));
}
imch & imgraw::block(imint idx, imint idx_y, imint idx_x){
    imint blk_y=((idx/64)*4);
    imint blk_x=((idx%64)*4);
    return this->maskVal(ImrCoor(blk_y, blk_x), 
        ImrCoor(idx_y, idx_x), ImrCoor(0, 0));
}
// 讀區塊(返回 vlaue arr)
ImrMask imgraw::block(imint idx){
    // 初始化大小
    this->setMaskSize(ImrSize(4,4));
    // 讀取位置
    imint y=((idx/64)*4);
    imint x=((idx%64)*4);
    return this->getMask(ImrCoor(y, x), ImrCoor(0,0));
}
// 取得索引
void imgraw::get_idx(string sou_name, string ori_name){
    imgraw sou(ImrSize(256, 256));
    sou.read(sou_name);
    imgraw ori(ImrSize(64, 64));
    ori.read(ori_name);
    for (int i = 0; i < 256; ++i){
        (*this)[i];
        // sou的i區塊與ori每個區塊比對
        sou.block_copy(i).dif_seq(ori);
    }
}
// this的單一區塊與 img(ori)每個區塊比對
int imgraw::ImrBlock::dif_seq(imgraw& img){
    // 區塊點的 差平方和 算一次要存下來
    vector<long int> img_arr(256);
    long int num, temp=-1;
    // int idx=0;
    for (int j = 0; j < 256; ++j){
        // img(ori)區塊
        num=0;
        for (int i = 0; i < 16; ++i){
            num+=pow(((int)(*this)[i] - 
                (int)img.block_copy(j)[i]), 2);
        }
        // cout << "num=" << num << endl;

        if(num < temp or temp==-1){
            // cout << "num=" << num << "j=" << j << endl;
            temp=num;
        }
        // cout << endl;
        // cout << "    num=" << num << endl;
    }
    //為什麼全部都能找到0的 QuQ
    cout << "temp=" << temp << endl;
    // cout << endl;
    return 0;
}
//----------------------------------------------------------------
// 合併檔案
void imgraw::merge(string ori_name, string idx_name){
    ImrSize size(64, 64);
    imgraw ori(size);
    ori.read(ori_name);
    imgraw idx(size);
    idx.read(idx_name);
    // 寫入檔案
    for (int j= 0, c =0; j < 4096; ++j)
        this->block_copy(j) = ori.block_copy(idx[c++]);
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
        imint rand = rand_int(0,256);
        if( arr[rand] == 0){
            arr[rand] = 1;
            this->block_copy(i) = sou.block_copy(rand);
            // this->block_copy(i).info();
            // sou.block_copy(rand).info();
        }else{
            --i;
        }
    }
}
//----------------------------------------------------------------
// 複製區塊
imgraw::ImrBlock imgraw::block_copy(imint pos){ 
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