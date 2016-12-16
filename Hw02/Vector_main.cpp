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
    // 開圖檔
    ImrSize size(Pic_y, Pic_x), size2(64, 64);
    imgraw img(size), sou(size);
    imgraw ori(size2), tra(size2), idx(size2);
    sou.read(Pic_name_in);
    // 建立編碼簿
    ori.get_org(sou).write(Origin);
    //---------------------------------------------------------
    // 建立索引
    idx.get_idx(sou, ori);
    idx.write(Idxcode);
    // 輸出第一次的圖檔
    img.merge(Origin, Idxcode).write(Pic_name_out);
    // 訓練編碼簿
    idx.tra_code(Pic_name_in, Origin, "tra.raw", "idx.raw");
    img.merge("tra.raw", "idx.raw").write("IMG_tra.raw");
    //---------------------------------------------------------
    // 合併檔案還原
    // img.merge(Origin, Idxcode).write(Pic_name_out);
    //---------------------------------------------------------
    // 開啟檔案
    if(AutoOpen==1)
        system(Pic_name_out);
    return 0;
}
