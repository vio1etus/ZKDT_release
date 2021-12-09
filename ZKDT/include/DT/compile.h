#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <map>
#include <string>
#include <iterator>

#include "DT/DT.h"

using namespace std;

std::map<std::string, int> map_of_classes; // exp: 分类名词 -> 分类 id 的映射

  /**
   * @brief read trained decision tree model and construct the tree from given preorder traversal 
   * 
   * @param filename decision tree model by  a preorder sequence
   * @return DT* pointer to decision tree 
   */
DT *_read_dt_from_file(string filename){ 
    int class_id = 0;
    bool isleft = true;
    DT *dt = new DT();

    ifstream input;
    input.open(filename.c_str()); 

    if(input.is_open()){
       std::string line;
       DTNode* current_node = nullptr;
       DTNode* tmp = nullptr;
       while(getline(input, line)){

           //if(current_node != nullptr)
           //   cout << "current_node->node_id = " << current_node->node_id << endl; //current node.
          //printf("%s\n", line.c_str());
          for(int i = 0; i < line.size(); i++){ // exp:二叉树的前序/先序构建：根左右
            if(line[i] == ':'){
            //  cout << "get in " << endl;
            //  cout << line.substr(i+2) << endl;
              string classification = line.substr(i + 2); //exp: 取冒号空格后的分类名称，并存储
              //cout << classification << endl;
              if(map_of_classes.find(classification) != map_of_classes.end()){ // exp:如果已经存在该分类，则获取该分类名的id号;否则插入(分类名，生成并递增id)到 map_of_classes
                //cout << "it is ok till now" << endl;
                class_id = map_of_classes[classification];
              }
                else
                  map_of_classes.insert(std::make_pair(classification, ++class_id));
              //  if(current_node->node_id == 0){
              //    cout << "isleft = " << isleft << " " << "node_id = " << dt->n_nodes << endl;
              //  }
            
                tmp = dt->add_leaf_node(current_node, isleft, class_id);
              //  cout << "tmpid = " << tmp->node_id << endl;
                if(isleft == false){
                  while(current_node != nullptr && current_node->r_visited) 
                  // exp:当前节点已经分类完毕，就该回溯到当前节点父节点，构建父节点的右节点了
                    current_node = current_node->parent;
                }
                //cout << "current_node->node_id = " << current_node->node_id << " " << "class_id = " << class_id << endl;
              break;
            }
            if(line[i] >= '0' && line[i] <= '9'){
              unsigned int j, variable_id_, threshold_;
              for(j = i; j < line.size(); j++){ //exp?: 这句是不是废话？直接判断它后面是空格，就说明这个是类别id
                if(line[j] < '0' || line[j] > '9'){
                  variable_id_ = stoi(line.substr(i, j)) - 1;
                  //cout << "variable_id_ = " << variable_id_ << endl;

                  break;
                }
              }
              if(line[j + 1] == '<' && line[j + 2] == '='){ //exp: 摘取比较符号后面的数字,作为阈值：可能是(information gain、gain ratio、gini index)*(read_dataset)
                threshold_ = stoi(line.substr(j + 4));
                 //cout << "threshold_ = " << threshold_ << endl;
                if(current_node == nullptr){ //exp: 如果还是空树，则创建根节点，并赋值给 current node
                  auto root = dt->add_internal_root(variable_id_, threshold_);
                  current_node = root;
                }
                else{
                  current_node = dt->add_internal_node(current_node, isleft, variable_id_, threshold_);
                }
                //if(current_node == nullptr || current_node->r_visited == false)
                  isleft = true;
                break;
              }
              if(line[j + 1] == '>'){
                current_node->r_visited = true;
                isleft = false;
                break;
              }
            }
          }
       }
       input.close();
    }
    //cout << "it is ok" << endl;
    return dt;
}

/*
int main(){
  DT * dt_model = _read_dt_from_file("/Users/cusgadmin/Desktop/ZKDT/Model/Iris_dt.txt");
  cout << "root_left_child_id = " << dt_model->root->l->node_id << endl;
  dt_model->print_dt(dt_model->root, 0);
  return 0;
}
*/


