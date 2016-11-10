//#include <arduino.h> //for debugging purposes ONLY

class bitVector {
  private:
    unsigned char* arr;
    int len;
    bool readBit(unsigned char* target, unsigned char index) {
      //Serial.print("Reading");Serial.print(index);Serial.print(" ");Serial.print(*target,BIN);Serial.print("->");Serial.println(*target & (1 << (7 - index)),BIN);
      return (*target & ((unsigned char)1 << (7 - index))) != 0;
    }
    void writeBit(unsigned char* target, unsigned char index, bool value) {
      if (value == true) {
        *target = *target | ((unsigned char)1 << (7 - index));
      } else {
        *target = *target & ~((unsigned char)1 << (7 - index));
      }
    }
    void expand() {
      unsigned char* temp = new unsigned char[len * 2];
      for (int i = 0; i < len * 2; i++) {
        temp[i] = 0;
      }
      for (int i = 0; i < len; i++) {
        temp[i] = arr[i];
      }
      delete []arr;
      arr = temp;
      len *= 2;
    }
  public:
    bitVector() {
      arr = new unsigned char[1];
      arr[0] = 0;
      len = 1;
    }
    ~bitVector() {
      delete[]arr;
    }
    bool read(int index) {
      while (index / 8 + ((index % 8) != 0) > len) {
        expand();
      }
      return readBit(&arr[index / 8], index % 8);
    }
    void write(int index, bool value) {
      while (index / 8 + ((index % 8) != 0) > len) {
        expand();
      }
      return writeBit(&arr[index / 8], index % 8, value);
    }
};
