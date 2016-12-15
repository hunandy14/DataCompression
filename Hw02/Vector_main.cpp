/**********************************************************
Name : OpenRAW_main
Date : 2016/08/03
By   : CharlotteHonG
Final: 2016/12/15
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
#define Idxcode "idxcode.raw" 
#define Pic_x 256
#define Pic_y 256

void creat_ori(imgraw & img);
int main(int argc, char const *argv[]) {
    // 創建畫布
    imgraw img(ImrSize(Pic_y, Pic_x));
    img.read(Pic_name_in);
    //---------------------------------------------------------
    // 建立編碼簿
    imgraw ori(ImrSize(64, 64));
    ori.get_org(Pic_name_in);
    ori.write(Origin);
    //---------------------------------------------------------
    // 建立索引
    imgraw idx(ImrSize(64, 64));
    idx.get_idx(Pic_name_in, Origin);
    idx.write(Idxcode);
    //---------------------------------------------------------
    // 訓練編碼簿
    imgraw tra(ImrSize(64, 64));
    // idx.get_idx(Pic_name_in, Origin);
    //---------------------------------------------------------
    // 合併檔案還原
    imgraw img2(ImrSize(Pic_y, Pic_x));
    img2.merge(Origin, Idxcode);
    img2.write(Pic_name_out);
    img2.info();
    //---------------------------------------------------------
    imgraw img3(ImrSize(Pic_y, Pic_x));
    img3.merge("origin2.raw", Idxcode);
    img3.write("IMG_OUT2.raw");
    // 開啟檔案
    if(AutoOpen==1)
        system(Pic_name_out);
    return 0;
}
