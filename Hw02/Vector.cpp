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
//=========================================================
//------------------------Operator-------------------------
//=========================================================
#include "OpenRAW_fun\Operator.cpp"
//=========================================================
namespace imr {
// 合併檔案
void imgraw::merge(string ori_name, string idx_name){
    imgraw ori(ImrSize(64, 64));
    ori.read(ori_name);
    imgraw idx(ImrSize(64, 64));
    idx.read(idx_name);
    // 寫入檔案
    for (int j= 0, c =0; j < 4; ++j){
        for (int i = 0; i < 16; ++i){
            this->block(j)[i] = ori.block(idx[c])[i];
            this->block(j)[i] = (imch)10;
            // 這裡有問題居然不能寫入
            cout << (int)this->block(j)[i] << " ";
            c++;
        }
    }
}


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
    // 取值(第幾個區塊)
    // auto&& B= [&](imint i){
    //     return ImrMask{this->block(i)};
    // };
    // 使用範例
    // B(0).info();
    // cout << endl;
    // 取值(第幾個區塊)
    // auto&& B2= [&](imint i){
    //     return ImrMask{sou.block(i)};
    // };
    // 使用範例
    // B2(0).info();
    //----------------------------------------------------------------
    this->comp(sou);
    

    // 返回找好的idx
    // return 
}
// 讀區塊(返回ref)
imch & imgraw::block(imint idx, imint idx_y, imint idx_x){
    imint y=((idx/64)*4);
    imint x=((idx%64)*4);

    return this->maskVal(ImrCoor(y, x), 
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