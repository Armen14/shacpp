#include <QtCore/QCoreApplication>
#include <iostream>
#include <bitset>
#include <QString>
#include <QBitArray>
#include <qdebug.h>

using namespace std;

quint32 F(quint32 X,quint32 Y,quint32 Z, int t);
quint32 K(int t);
quint32* W(quint32* w);
QString bitsToString(QBitArray& arr);
QBitArray stringToBits(const QString& str);
void sha1(QString message);
quint32* new_array_32(int size);
quint32 left(int bits, quint32 word);

typedef bitset<32> bit32;
typedef bitset<64> bit64;

int main() {

    sha1("231658132151354651321585135132153132165432156143216513215646");

    return 0;
}

void sha1(QString message) {
    message = message.toAscii();
    quint32 A = 0x67452301;
    quint32 B = 0xEFCDAB89;
    quint32 C = 0x98BADCFE;
    quint32 D = 0x10325476;
    quint32 E = 0xC3D2E1F0;

    QBitArray bit_message = stringToBits(message);
    quint64 size_message = bit_message.size();
    bit64 bit_size_message = size_message;

    bit_message.resize(bit_message.size() + 1);
    bit_message.setBit(bit_message.size()-1,true);
    while(!(bit_message.size() % 512 == 448)) {
        bit_message.resize(bit_message.size() + 1);
        bit_message.setBit(bit_message.size()-1,false);
    }
    bit_message.resize(bit_message.size() + 64);
    int counter = 0;
    for(int i=448; i<512; i++) {
        bit_message[i] = bit_size_message[counter];
        counter++;
    }

    int size512 = bit_message.size()/512;
    for(int i=0; i<size512; i++) {
        quint32* w = new_array_32(80);
        int counter = 0;
        for(int j=0; j<16; j++) {
            bit32 w_bit;
            int start = i*512 + j*32;
            int end = i*512 + (j+1)*32;
            counter = 0;
            for(int k=start; k<end; k++) {
                w_bit[counter] = bit_message[k];
                counter++;
            }
            w[j] = w_bit.to_ulong();
        }

        w = W(w);

        quint32 a = A;
        quint32 b = B;
        quint32 c = C;
        quint32 d = D;
        quint32 e = E;
        for(int j=0; j<80; j++) {
            quint32 temp = (a<<5) + F(b,c,d,j) + e + w[j]+ K(j);
            e=d;
            d=c;
            c=left(30,b);
            b=a;
            a=temp;
        }
        A = A + a;
        B = B + b;
        C = C + c;
        D = D + d;
        E = E + e;
    }
    printf ("%x ", A);
    printf ("%x ", B);
    printf ("%x ", C);
    printf ("%x ", D);
    printf ("%x ", E);
    qDebug()<<"hash = "<<A<<B<<C<<D<<E;

}

quint32 K(int t) {
    if(t>=0 && t<=19) {
        return 0x5A827999;
    }
    if(t>=20 && t<=39) {
        return 0x6ED9EBA1;
    }
    if(t>=40 && t<=59) {
        return 0x8F1BBCDC;
    }
    if(t>=60 && t<=79) {
        return 0xCA62C1D6;
    }
    return 0;
}

quint32 F(quint32 X,quint32 Y,quint32 Z, int t) {
    if(t>=0 && t<=19) {
        return (X & Y) | ((~X) & Z);
    }
    if(t>=20 && t<=39) {
        return X^Y^Z;
    }
    if(t>=40 && t<=59) {
        return (X & Y) | (X & Z) | (Y & Z);
    }
    if(t>=60 && t<=79) {
        return X^Y^Z;
    }
    return 0;
}

quint32* W(quint32* w) {
    for(int i=16; i<80; i++) {
        w[i] = left(1, w[i-3] ^ w[i-8] ^ w[i-14] ^ w[i-16]) ;
    }
    return w;
}

QString bitsToString(QBitArray& arr) {
    QString r;
    r.resize(arr.size() / 16);
    qMemCopy(r.data_ptr()->data, arr.data_ptr()->data+1, r.length()*2);
    return(r);
}

QBitArray stringToBits(const QString& str) {
    QBitArray r(str.length()*16);
    qMemCopy(r.data_ptr()->data+1, str.constData(), str.length()*2);
    return (r);
}

quint32* new_array_32(int size) {
    quint32* A = new quint32[size];
    return A;
}

quint32 left(int bits, quint32 word) {
    return ((word << bits) & 0xFFFFFFFF) | ((word & 0xFFFFFFFF) >> (32-bits));
}
