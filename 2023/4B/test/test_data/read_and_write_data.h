#include <fstream>
#include <iostream>
template <class datatype>
void read_data(std::string path, int64_t* ndim, std::vector<int64_t>* dims,
               int64_t* dtype, int64_t* num, std::vector<datatype>* a) {
  std::ifstream infile;
  infile.open(path, std::ios::in);
  if (!infile.is_open()) {
    std::cout << "read error" << std::endl;
    exit(1);
  }
  infile >> *ndim;
  std::vector<int64_t> dim;
  for (int i = 0; i < *ndim; i++) {
    int64_t tmp;
    infile >> tmp;
    dims->push_back(tmp);
  }
  infile >> *dtype;
  infile >> *num;
  for (int i = 0; i < *num; i++) {
    datatype tmp;
    infile >> tmp;
    a->push_back(tmp);
  }
  infile.close();
}

void read_resize_hw(std::string path, int64_t* target_h, int64_t* target_w) {
  std::ifstream infile;
  infile.open(path, std::ios::in);
  if (!infile.is_open()) {
    std::cout << "read error" << std::endl;
    exit(1);
  }
  infile >> *target_h;
  infile >> *target_w;
  infile.close();
}

template <class datatype>
void write_data(std::string path, int64_t ndim, std::vector<int64_t> dims,
                int64_t dtype, int64_t num, datatype* a) {

  std::ofstream outfile;
  outfile.open(path, std::ios::out);
  if (!outfile.is_open()) {
    std::cout << "write error" << std::endl;
    exit(1);
  }
  outfile << ndim << std::endl;
  for (int i = 0; i < ndim; i++) {
    outfile << dims[i] << " ";
  }
  outfile << std::endl;
  outfile << dtype << std::endl;
  outfile << num << std::endl;
  for (int i = 0; i < num; i++) {
    outfile << a[i] << " ";
  }
  outfile.close();
}
