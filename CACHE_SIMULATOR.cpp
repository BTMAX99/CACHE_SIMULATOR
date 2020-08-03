#include <bits/stdc++.h>
#include <fstream>
using namespace std;
struct cacheBlock {
  int tag = -1;
  int valid_bit = -1;
  int dirty_bit = 0;
};

struct setk {
  vector<cacheBlock> *ptr;
  int fill = 0;
};
int main() {
  ifstream fin;
  ofstream fout;
  fin.open("input.txt");
  fout.open("output.txt");
  int cache_size;
  int block_size;
  int no_of_blocks;
  int associativity;
  int replacement_policy;
  int no_sets;
  int cache_access = 0;
  fin >> cache_size >> block_size >> associativity >> replacement_policy;
  int blocks_per_set;
  int compulsory_misses = 0;
  int conflict_misses = 0;
  int cache_misses = 0;
  int dirty_evicted = 0;
  int capacity_misses = 0;
  int read_misses = 0;
  int write_misses = 0;
  no_of_blocks = cache_size / block_size;
  if (associativity == 0) {
    no_sets = 1;
    blocks_per_set = no_of_blocks;
  } else {
    no_sets = no_of_blocks / associativity;
    blocks_per_set = associativity;
  }
  setk *full;
  full = new setk[no_sets];
  int off_set = log2(block_size);
  int index_bits = log2(no_sets);
  int tag_bits = 32 - off_set - index_bits - 1;
  int read_access = 0;
  int write_access = 0;
  for (int i = 0; i < no_sets; i++) {
    full[i].ptr = new vector<cacheBlock>;
  }
  string memory_trace;
  
  while (fin >> memory_trace) {
    cache_access++;
    string binary;
    binary = "";
    for (int i = 0; i < 8; i++) {

      if (memory_trace[i] == '0') {
        binary = binary + "0000";
      } else if (memory_trace[i] == '1') {
        binary = binary + "0001";
      } else if (memory_trace[i] == '2') {
        binary = binary + "0010";
      } else if (memory_trace[i] == '3') {
        binary = binary + "0011";
      } else if (memory_trace[i] == '4') {
        binary = binary + "0100";
      } else if (memory_trace[i] == '5') {
        binary = binary + "0101";
      } else if (memory_trace[i] == '6') {
        binary = binary + "0110";
      } else if (memory_trace[i] == '7') {
        binary = binary + "0111";
      } else if (memory_trace[i] == '8') {
        binary = binary + "1000";
      } else if (memory_trace[i] == '9') {
        binary = binary + "1001";
      } else if (memory_trace[i] == 'A') {
        binary = binary + "1010";
      } else if (memory_trace[i] == 'B') {
        binary = binary + "1011";
      } else if (memory_trace[i] == 'C') {
        binary = binary + "1100";
      } else if (memory_trace[i] == 'D') {
        binary = binary + "1101";
      } else if (memory_trace[i] == 'E') {
        binary = binary + "1110";
      } else if (memory_trace[i] == 'F') {
        binary = binary + "1111";
      }
    }

    if (binary[0] == '0') {
      read_access++;
    }
    if (binary[0] == '1') {
      write_access++;
    }
    int index = 0;
    for (int i = 0; i < index_bits; i++) {
      if (binary[31 - off_set - i] == '1') {
        index = index + pow(2, i);
      }
    }
    vector<cacheBlock> *temp1 = full[index].ptr;
    // cacheBlock temp=(*temp1)[1];
    int tag_calculated=0;

    for (int i = 0; i < tag_bits; i++) {
      if (binary[31 - off_set - index_bits - i] == '1') {
        tag_calculated = tag_calculated + pow(2, i);
      }
    }

    if (full[index].fill == 0) {
      cacheBlock B;
      B.tag = tag_calculated;
      B.valid_bit = 1;
      if (binary[0] == '1') {
        B.dirty_bit = 1;
      }

      temp1->push_back(B);
      full[index].fill = 1;
      compulsory_misses++;
      cache_misses++;
      if (binary[0] == '0') {
        read_misses++;
      }
      if (binary[0] == '1') {
        write_misses++;
      }

    } else {
      int j = 0;
      int k = 0;
      for (auto it = temp1->begin(); it != temp1->end(); it++) {
        j++;
        if (j <= blocks_per_set) {
          if (it->tag == tag_calculated) {
            cacheBlock B;
            B.tag = tag_calculated;
            B.valid_bit = 1;
	    int d = it->dirty_bit;
            if (binary[0] == '1') {
              B.dirty_bit = 1;
            }
            else{
              B.dirty_bit = d;
            }
            temp1->erase(it);
            temp1->insert(temp1->begin(), B);
            k = 1;
            break;
          }
        }
        if (j >= blocks_per_set + 1) {
          if (it->tag == tag_calculated) {
            cacheBlock B;
            B.tag = tag_calculated;
            B.valid_bit = 1;
            int d = it->dirty_bit;
            if (binary[0] == '1') {
              B.dirty_bit = 1;
            }
            else{
              B.dirty_bit = d;
            }
            auto et = temp1->begin();
for(int i=1;i<blocks_per_set;i++)
          {  et++;}
          
            //et++;
            if (et->dirty_bit == 1) {
              dirty_evicted++;
            }

            temp1->erase(it);
            temp1->insert(temp1->begin(), B);
            if (associativity == 0) {
              capacity_misses++;
		}
             else if (associativity != 0) {
              conflict_misses++;
            }
            cache_misses++;
            if (binary[0] == '0') {
              read_misses++;
            }
            if (binary[0] == '1') {
              write_misses++;
            }

            k = 1;
            break;
          }
        }
      }
      if (k == 0 && j < blocks_per_set) {
        cacheBlock B;
        B.tag = tag_calculated;
        B.valid_bit = 1;
        if (binary[0] == '1') {
          B.dirty_bit = 1;
        }
        temp1->insert(temp1->begin(), B);
        compulsory_misses++;
        if (binary[0] == '0') {
          read_misses++;
        }
        if (binary[0] == '1') {
          write_misses++;
        }
        cache_misses++;

      } else if (k == 0 && j >= blocks_per_set) {
        cacheBlock B;
        B.tag = tag_calculated;
        B.valid_bit = 1;
        if (binary[0] == '1') {
          B.dirty_bit = 1;
        }
	auto et = temp1->begin();
	for(int i=1;i<blocks_per_set;i++)
          {  et++;}
          
            //et++;
            if (et->dirty_bit == 1) {
              dirty_evicted++;
            }
        temp1->insert(temp1->begin(), B);
        compulsory_misses++;
        cache_misses++;
        if (binary[0] == '0') {
          read_misses++;
        }
        if (binary[0] == '1') {
          write_misses++;
        }
      }
    }

 
    //cout<<binary<<endl;
      //cout<<off_set<<endl;
     
    /*cout<<index_bits<<endl;
    cout<<tag_bits<<endl;
    cout<<tag_calculated<<endl;
    cout<<index<<endl;*/
  }
  fout << cache_size << endl
       << block_size << endl
       << associativity << endl
       << replacement_policy << endl
       << cache_access << endl
       << read_access << endl
       << write_access << endl
       << cache_misses << endl
       << compulsory_misses << endl
       << conflict_misses << endl
       << capacity_misses << endl
       << read_misses << endl
       << write_misses << endl
       << dirty_evicted<<endl;
    return 0;
}

	
	
	

