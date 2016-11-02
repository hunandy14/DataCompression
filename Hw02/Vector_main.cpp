/**********************************************************
Name : OpenRAW_main
Date : 2016/08/03
By   : CharlotteHonG
Final: 2016/10/13
**********************************************************/
#include <iostream>
#include "OpenRAW_fun\OpenRAW"
// Visual Studio 編譯需刪除該行與該檔案
#include "Vector.cpp" // GCC單檔編譯用

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
    // 建立編碼簿
    imgraw codebook_ori(ImrSize(Pic_y/4, Pic_x/4));
    // 初始編碼簿 - 隨機取得
    codebook_ori.get_org(img);
    codebook_ori.write("origin.raw");
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