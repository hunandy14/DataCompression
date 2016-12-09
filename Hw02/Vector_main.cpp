/**********************************************************
Name : OpenRAW_main
Date : 2016/08/03
By   : CharlotteHonG
Final: 2016/10/13
**********************************************************/
#include <iostream>
#include <iomanip>
#include "OpenRAW_fun\OpenRAW"
// Visual Studio �sĶ�ݧR���Ӧ�P���ɮ�
#include "Vector.cpp" // GCC���ɽsĶ��

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
    // �Ыصe��
    imgraw img(ImrSize(Pic_y, Pic_x));
    img.read(Pic_name_in);
    //---------------------------------------------------------
    // �إ߽s�Xï
    imgraw origin(ImrSize(Pic_y/4, Pic_x/4));
    // ��l�s�Xï - �H�����o
    origin.get_org(img);
    origin.write(Origin);
    //---------------------------------------------------------
    // �إ߲M��
    imgraw codebook_tra(ImrSize(Pic_y/4, Pic_x/4));
    codebook_tra.training(img, Origin);
    // for (int i = 0; i < 256; ++i){
        // cout << "tra=" << (int)codebook_tra[i] << endl;
    // }
    codebook_tra.write(idxcode);
    //---------------------------------------------------------
    // �X���ɮ��٭�
    imgraw img2(ImrSize(Pic_y, Pic_x));
    img2.merge(Origin, idxcode);
    img2.info();
    img2.write(Pic_name_out);
    //---------------------------------------------------------
    // �}���ɮ�
    if(AutoOpen==1)
        system(Pic_name_out);
    return 0;
}
