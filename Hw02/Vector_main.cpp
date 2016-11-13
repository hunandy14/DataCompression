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
#define Pic_x 256
#define Pic_y 256

void creat_ori(imgraw & img);
int main(int argc, char const *argv[]) {
    // �Ыصe��
    imgraw img(ImrSize(Pic_y, Pic_x));
    // Ū���ɮ�
    img.read(Pic_name_in);
    //---------------------------------------------------------
    creat_ori(img);
    //---------------------------------------------------------
    // �V�m�s�Xï
    imgraw codebook_tra(ImrSize(Pic_y/4, Pic_x/4));
    codebook_tra.read(Origin);
    // ����
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
    // ���ܰT��
    img.info();
    // ��X�ɮ�
    img.write(Pic_name_out);
    // �}���ɮ�
    if(AutoOpen==1)
        system(Pic_name_out);
    return 0;
}

void creat_ori(imgraw & img){
    // �إ߽s�Xï
    imgraw codebook_ori(ImrSize(Pic_y/4, Pic_x/4));
    // ��l�s�Xï - �H�����o
    codebook_ori.get_org(img);
    codebook_ori.write(Origin);
}