#ifndef ReverseBits_h
#define ReverseBits_h

unsigned char ReverseBitsObvious(unsigned char v);
unsigned char ReverseBitsLookupTable(unsigned char v);
unsigned char ReverseBits3ops64bit(unsigned char v);
unsigned char ReverseBits4ops64bit(unsigned char v);
unsigned char ReverseBits7ops32bit(unsigned char v);
unsigned char ReverseBits5logNops1(unsigned char v);
unsigned char ReverseBits5logNops2(unsigned char v);
unsigned char ReverseBitsRBIT(unsigned char v);

#endif
