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
#define Pic_x 256
#define Pic_y 256

void creat_ori(imgraw & img);
int main(int argc, char const *argv[]) {
    // 創建畫布
    imgraw img(ImrSize(Pic_y, Pic_x));
    // 讀取檔案
    img.read(Pic_name_in);
    //---------------------------------------------------------
    creat_ori(img);
    //---------------------------------------------------------
    // 訓練編碼簿
    imgraw codebook_tra(ImrSize(Pic_y/4, Pic_x/4));
    codebook_tra.read(Origin);
    // 取值
    auto&& B = [&codebook_tra](imint i){
        return ImrMask{codebook_tra.block(i)};
    };

    for (int j = 0; j < 4; ++j){
        for (int i = 0; i < 4; ++i){
            cout << setw(4) << (int)codebook_tra.at2d(j, i);
        }cout << endl;
    }cout << endl;

    for (int i = 0; i < 16; ++i){
        cout << setw(4) << (int)B(0)[i];
    }cout << endl;
    //---------------------------------------------------------
    // 提示訊息
    img.info();
    // 輸出檔案
    img.write(Pic_name_out);
    // 開啟檔案
    if(AutoOpen==1)
        system(Pic_name_out);
    return 0;
}

void creat_ori(imgraw & img){
    // 建立編碼簿
    imgraw codebook_ori(ImrSize(Pic_y/4, Pic_x/4));
    // 初始編碼簿 - 隨機取得
    codebook_ori.get_org(img);
    codebook_ori.write(Origin);
}