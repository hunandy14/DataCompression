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
    // 合併檔案還原
    imgraw img(ImrSize(Pic_y, Pic_x));
    img.merge(Origin, Idxcode);
    img.write(Pic_name_out);
    //---------------------------------------------------------
    // 訓練編碼簿
    imgraw tra(ImrSize(64, 64));
    tra.get_con(Pic_name_in, Origin, Idxcode);
    tra.write("tra1.raw");

    idx.get_idx(Pic_name_in, "tra1.raw");
    idx.write(Idxcode);
    
    imgraw img1(ImrSize(Pic_y, Pic_x));
    img1.merge("tra1.raw", Idxcode);
    img1.write("IMG_OUT1.raw");
    //---------------------------------------------------------
    imgraw tra2(ImrSize(64, 64));
    tra2.get_con(Pic_name_in, "tra1.raw", Idxcode);
    tra2.write("tra2.raw");
    imgraw img2(ImrSize(Pic_y, Pic_x));
    img2.merge("tra2.raw", Idxcode);
    img2.write("IMG_OUT2.raw");
    // 開啟檔案
    if(AutoOpen==1)
        system(Pic_name_out);
    return 0;
}
