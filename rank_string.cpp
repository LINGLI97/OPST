#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>

using namespace std;

// compile g++ -O3 rank_string.cpp -o rank_string
// run ./rank_string input_file

int main(int argc, char **argv)
{
   std::ifstream is(argv[1]);


    vector<double> input_seq_vec;
    double c;
    while (is>>c)
    {
//        if(c=='\n')
//        {
//            break;
//        }
//        else
//        {
//            input_seq_vec.push_back(c);
//        }

            input_seq_vec.push_back(c);

    }
    is.close();

//    vector<char> input_seq_vec;
//    char c;
//    while (is>>c)
//    {
//            input_seq_vec.push_back(c);
//
//    }
//    is.close();

//    is.close();
  
  for(auto &it : input_seq_vec)
	cout<<it<<" ";
  cout<<endl;

  vector<double> copy_for_print(input_seq_vec.begin(),input_seq_vec.end());

  sort(input_seq_vec.begin(), input_seq_vec.end());

  unordered_map<double, int> my_map;

  int rnk=0;

  //after sorting
  for(auto &it : input_seq_vec)
  {
      cout<<it<<" ";
      unordered_map<double, int>::const_iterator it2=my_map.find(it);
      if(it2==my_map.end())
      {
	      my_map[it]=rnk;
		rnk++;
      }
  }
  cout<<endl;

  cout<<"my_map:\n";
  for(auto &it : my_map)
  {
	cout<<it.first<<" "<<it.second<<endl;
  }
  cout<<endl;

  cout<<"print string with ranks instead of letters:"<<endl;
  
  for(auto &it : copy_for_print)
        cout<<my_map[it]<<" ";
  cout<<endl;

    fstream f;

    string newName= (string)argv[1]+"_discretized";
    f.open(newName,ios::out);

    for(auto &it : copy_for_print){
        f<<my_map[it]<<endl;

    }

    f.close();

    //    cout<<endl;
}
  
