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
#define IMG     "IMG.raw"
#define IMG_OUT "IMG_OUT.raw"

int main(int argc, char const *argv[]) {
    // 開圖檔
    ImrSize size(256, 256), size2(64, 64);
    imgraw img(size), sou(size);
    imgraw ori(size2), tra(size2), idx(size2);
    sou.read(IMG);
    //---------------------------------------------------------
    // 建立編碼簿
    ori.get_org(sou).write("ori.raw");
    // 建立索引
    idx.get_idx(sou, ori).write("idx.raw");
    // 輸出第一次的圖檔
    img.merge(ori, idx).write(IMG_OUT);
    // 訓練編碼簿
    for (int i = 0; i < 99; ++i){
        double temp = tra.tra_code(sou, idx);
        cout << "[" << setw(2) << i << "] = " << temp << endl;
        if (temp <= 0.01)
            break;
    }tra.write("tra.raw");

    img.merge(tra, idx).write("IMG_tra.raw");
    //---------------------------------------------------------
    // 開啟檔案
    if(AutoOpen==1)
        system(IMG_OUT);
    return 0;
}
