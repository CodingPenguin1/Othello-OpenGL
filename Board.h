class Board {
  private:
  public:
    int size;
    int **array;

    Board(int);
    ~Board();
    int &operator[](int i);
    void print();
};