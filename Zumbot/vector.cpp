//#include <arduino.h> //for debugging purposes ONLY

template<typename T> bool defaultComparator(T v1, T v2) {
  return v1 > v2;
}

template<typename T> class vector {
  private:
    T* arr;
    int len;
    int dataLen;
    void expand() {
      T* temp = new T[len * 2];
      for (int i = 0; i < dataLen; i++) {
        temp[i] = arr[i];
      }
      delete[]arr;
      arr = temp;
      len = len * 2;
    }
  public:
    vector()  {
      len = 1;
      arr = new T[len];
      dataLen = 0;
    }
    vector(int _length): len(_length) {
      arr = new T[_length];
      dataLen = 0;
    }
    vector(int _length, T _default): len(_length) {
      arr = new T[_length];
      dataLen = 0;
      for (int i = 0; i < _length; i++) {
        arr[i] = _default;
      }
    }
    ~vector() {
      delete [] arr;
    }
    T& operator[](int index) {
      if (index < len) {
        return arr[index];
      } else {
        return;
      }
    }
    void push_back(T _data) {
      if (dataLen < len) {
        arr[dataLen] = _data;
        dataLen++;
      } else {
        expand();
        push_back(_data);
      }
    }
    T pop_back() {
      if (dataLen != 0) {
        dataLen--;
        return arr[dataLen];
      }
    }
    int Size() {
      return dataLen;
    }
    T front() {
      if (dataLen > 0) {
        return arr[0];
      }
    }
    T back() {
      if (dataLen > 0) {
        return arr[dataLen - 1];
      }
    }
    void sort(bool (*biggerthan)(T, T)) {
      bool done = false;
      while (done == false) {
        done = true;
        for (int i = 0; i < dataLen - 1; i++) {
          if ((*biggerthan)(arr[i + 1], arr[i])) {
            T temp = arr[i];
            arr[i] = arr[i + 1];
            arr[i + 1] = temp;
            done = false;
          }
        }
      }
    }
    void sort() {
      sort(&defaultComparator);
    }
    void resize(int _length){
      T* temp=new T[_length];
      for (int i=0;i<min(_length,len);i++){
        temp[i]=arr[i];
      }
      delete[]arr;
      arr=temp;
      len=_length;
      dataLen=min(dataLen,_length);
    }
    T Max(){
      T best=this[0];
      for (int i=1;i<Size();i++){
        if (this[i]>best){
          best=this[i];
        }
      }
      return best;
    }
    T Min(){
      T best=this[0];
      for (int i=1;i<Size();i++){
        if (this[i]<best){
          best=this[i];
        }
      }
      return best;
    }
};
