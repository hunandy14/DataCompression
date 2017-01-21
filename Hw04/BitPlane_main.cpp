/**********************************************************
Name : OpenRAW_main
Date : 2016/08/03
By   : CharlotteHonG
Final: 2016/10/13
**********************************************************/
#include <iostream>
#include "OpenRAW_fun\OpenRAW"
// Visual Studio 編譯需刪除該行與該檔案
#include "BitPlane.cpp" // GCC單檔編譯用

using namespace std;
using namespace imr;

#define AutoOpen 1
#define Pic_name_in "IMG.raw"
#define Pic_name_out "IMG_OUT.raw"
#define Pic_x 256
#define Pic_y 256

int main(int argc, char const *argv[]) {
    // 創建畫布
    imgraw img(ImrSize(Pic_y, Pic_x));
    imgraw img2(ImrSize(Pic_y, Pic_x));
    imgraw temp(ImrSize(Pic_y, Pic_x));
    // 讀取檔案
    img.read(Pic_name_in);
    //---------------------------------------------------------
    for(int i = 8; i > 0; --i) {
        temp.bitplane(img, i-1);
        img2 = img2+temp;
    }
    //---------------------------------------------------------
    // 輸出檔案
    img2.info();
    img2.write(Pic_name_out);
    // 開啟檔案
    if(AutoOpen==1)
        system(Pic_name_out);
    return 0;
}