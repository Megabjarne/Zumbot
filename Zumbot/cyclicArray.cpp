//#include <arduino.h> //for debugging purposes ONLY

template<typename T> class cyclicArray
{
  private:
    T* arr;
    int len;
    int curr;
    int dataLen;
    void moveCurr(int dist) {
      curr += dist;
      while (curr < 0) {
        curr += len;
      }
      curr = curr % len;
    }
    void changeDataLen(int dist) {
      dataLen += dist;
      if (dataLen < 0) {
        dataLen = 0;
      }
      if (dataLen > len) {
        dataLen = len;
      }
    }
    T* item(int index) {
      return &arr[(curr + index + len) % len];
    }
  public:
    cyclicArray<T>(int _length): len(_length) {
      arr = new T[_length];
      curr = 0;
      dataLen = 0;
    }
    cyclicArray<T>(int _length, T _default): len(_length) {
      arr = new T[_length];
      curr = 0;
      dataLen = 0;
      for (int i = 0; i < _length; i++) {
        arr[i] = _default;
      }
    }
    ~cyclicArray() {
      delete []arr;
    }
    T operator[](int i) {
      return *item(i);
    }
    void push_front(T _data) {
      changeDataLen(1);
      moveCurr(-1);
      *item(0) = _data;
    }
    void push_back(T _data) {
      if (dataLen != len) {
        changeDataLen(1);
      } else {
        moveCurr(1);
      }
      *item(Size() - 1) = _data;
    }
    T pop_front() {
      moveCurr(1);
      changeDataLen(-1);
      return *item(-1);
    }
    T pop_back() {
      changeDataLen(-1);
      return *item(Size());
    }
    int Size() {
      return dataLen;
    }
    int totSize() {
      return len;
    }
    T front() {
      if (dataLen > 0) {
        return *item(0);
      }
    }
    T back() {
      if (dataLen > 0) {
        return *item(dataLen - 1);
      }
    }
};
