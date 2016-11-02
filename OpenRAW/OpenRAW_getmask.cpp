/**********************************************************
Name : OpenRAW_getmask 說明範例
Date : 2016/10/04
By   : CharlotteHonG
Final: 2016/10/13
**********************************************************/
#include <iostream>
#include "OpenRAW_fun\OpenRAW"
// Visual Studio 編譯需刪除該行與該檔案
#include "OpenRAW.cpp" // GCC單檔編譯用
using namespace std;
using namespace imr;

#define AutoOpen 0
#define Pic_name_in "IMG.raw"
#define Pic_name_out "IMG_OUT.raw"
#define Pic_x 256
#define Pic_y 256

int main(int argc, char const *argv[]) {
    // 創建畫布
    imgraw img(ImrSize(Pic_y, Pic_x));
    // 讀取檔案
    img.read(Pic_name_in);
    //---------------------------------------------------------
    // 設定遮罩
    img.setMaskSize(ImrSize(4,4));
    // 取得Mask陣列及排續 getMask(原點位置)
    ImrMask mask = img.getMask(ImrCoor(1,1));
    cout << endl<< "setMaskSize" << endl;
    mask.info();

    // 原圖比較
    cout << endl<< "Original" << endl;
    for(int j = 0; j < 4; ++j){
        for(int i = 0; i < 4; ++i) {
            cout << (int)img.at2d(j, i) << " ";
        }cout << endl;
    }

    // 排序
    mask.sort();
    cout << endl<< "sort" << endl;
    mask.info();
    // 取平均值
    cout << "avg=" << (int)mask.avg() << endl;
    //---------------------------------------------------------
    // 提示訊息
    img.info();
    //---------------------------------------------------------
    // 輸出檔案
    img.write(Pic_name_out);
    // 開啟檔案
    if(AutoOpen==1)
        system(Pic_name_out);
    return 0;
}