#include <arduino.h> //for debugging purposes ONLY

template<typename T> class list {
  private:
    struct Node {
      Node* last, next;
      T data;
    };
    Node* start, finish;
  public:
    T get(int index){
      if (!start){return;}
      Node* current=start;
      for (int i=0;i<index;i++){
        if (!current){return;}
        current=current->next;
      }
      return current->data;
    }
    insert(T data, int index){
      if (!start && !finish && index==0){start=new Node;finish=start;start->data=data;return;}
      if (!start && !finish && index!=0){return;}
      Node* current=start;
      for (int i=0;i<index;i++){
        if (i==index-1 && !current->next){current->next=new Node;finish=current->next;finish->data=data;return;}
        current=current->next;
      }
    }
    push_back(T data) {
      if (!start && !finish) {
        start = new Node();
        finish = start;
        start->data = data;
      } else {
        Node* temp;
        temp.last = finish;
        finish->next = temp;
        finish = temp;
      }
    }
    push_front(T data) {
      if (!start && !finish) {
        start = new Node();
        finish = start;
        start->data = data;
      } else {
        Node* temp;
        temp.next = start;
        start->last = temp;
        start = temp;
      }
    }
    struct iterator {
      private:
        Node* current;
      public:
        iterator(Node* start): current(start) {}
        T data() {
          if (current) {
            return current->data;
          } else {
            return;
          }
        }
        void operator++() {
          if (current) {
            current = current->next;
          }
        }
        void operator--() {
          if (current) {
            current = current->last;
          }
        }
    };
    iterator begin() {
      return *new iterator(start);
    }
    iterator end() {
      return *new iterator(finish);
    }
};
