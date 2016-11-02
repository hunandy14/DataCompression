/**********************************************************
Name : OpenRAW_main
Date : 2016/08/03
By   : CharlotteHonG
Final: 2016/10/13
**********************************************************/
#include <iostream>
#include "OpenRAW_fun\OpenRAW"
// Visual Studio �sĶ�ݧR���Ӧ�P���ɮ�
#include "Vector.cpp" // GCC���ɽsĶ��

using namespace std;
using namespace imr;

#define AutoOpen 0
#define Pic_name_in "IMG.raw"
#define Pic_name_out "IMG_OUT.raw"
#define Pic_x 256
#define Pic_y 256

int main(int argc, char const *argv[]) {
    // �Ыصe��
    imgraw img(ImrSize(Pic_y, Pic_x));
    // Ū���ɮ�
    img.read(Pic_name_in);
    //---------------------------------------------------------
    // �إ߽s�Xï
    imgraw codebook_ori(ImrSize(Pic_y/4, Pic_x/4));
    // ��l�s�Xï - �H�����o
    codebook_ori.get_org(img);
    codebook_ori.write("origin.raw");
    //---------------------------------------------------------
    // ���ܰT��
    img.info();
    //---------------------------------------------------------
    // ��X�ɮ�
    img.write(Pic_name_out);
    // �}���ɮ�
    if(AutoOpen==1)
        system(Pic_name_out);
    return 0;
}