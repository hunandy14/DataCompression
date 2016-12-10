/*****************************************************************
Name : 
Date : 2016/12/08
By   : CharlotteHonG
Final: 2016/12/08
*****************************************************************/
namespace imr {
imgraw::ImrBlock::ImrBlock(imgraw& img, imint pos){
    this->pos_y=((pos/(img.high/4))*4);
    this->pos_x=((pos%(img.width/4))*4);
    this->img_p.resize(16);
    for (int j=0, c=0; j < 4; ++j){
        for (int i = 0; i < 4; ++i){
            this->img_p[c++]=&img.at2d(j+pos_y, i+pos_x);
        }
    }
}

void imgraw::ImrBlock::info(){
    for (int i = 0, c = 0; i < 4; ++i){
        for (int j = 0; j < 4; ++j){
            cout << setw(4) << (int)*(this->img_p[c++]);
        }cout << endl;
    }
}

imch& imgraw::ImrBlock::operator[](const size_t __n){
    return *(this->img_p[__n]);
}
const imch& imgraw::ImrBlock::operator[](const size_t __n) const{
    return *(this->img_p[__n]);
}
void imgraw::ImrBlock::operator=(ImrBlock b){
    for (int i = 0; i < 16; ++i){
        *(this->img_p[i])=*(b.img_p[i]);
    }
}
} // imr