/**********************************************************
Name : OpenRAW_main
Date : 2016/08/03
By   : CharlotteHonG
Final: 2016/10/13
**********************************************************/
#include <iostream>
#include <iomanip>
#include "OpenRAW_fun\OpenRAW"
// Visual Studio 編譯需刪除該行與該檔案
#include "Vector.cpp" // GCC單檔編譯用

using namespace std;
using namespace imr;

#define AutoOpen 0
#define Pic_name_in "IMG.raw"
#define Pic_name_out "IMG_OUT.raw"
#define Origin "origin.raw"
#define idxcode "idxcode.raw"
#define Pic_x 256
#define Pic_y 256

void creat_ori(imgraw & img);
int main(int argc, char const *argv[]) {
    // 創建畫布
    imgraw img(ImrSize(Pic_y, Pic_x));
    // 讀取檔案
    img.read(Pic_name_in);
    //---------------------------------------------------------
    // 建立編碼簿
    creat_ori(img);
    //---------------------------------------------------------
    // 建立清單
    imgraw codebook_tra(ImrSize(Pic_y/4, Pic_x/4));
    codebook_tra.training(img, Origin);
    for (int i = 0; i < 256; ++i){
        // cout << "tra=" << (int)codebook_tra[i] << endl;
    }
    codebook_tra.write(idxcode);
    //---------------------------------------------------------
    // 合併檔案還原
    imgraw img2(ImrSize(Pic_y, Pic_x));
    img2.merge(Origin, idxcode);
    //---------------------------------------------------------
    // 提示訊息
    img2.info();
    // 輸出檔案
    img2.write(Pic_name_out);
    // 開啟檔案
    if(AutoOpen==1)
        system(Pic_name_out);
    return 0;
}

void creat_ori(imgraw & img){
    // 建立編碼簿
    imgraw origin(ImrSize(Pic_y/4, Pic_x/4));
    // 初始編碼簿 - 隨機取得
    origin.get_org(img);
    origin.write(Origin);
}