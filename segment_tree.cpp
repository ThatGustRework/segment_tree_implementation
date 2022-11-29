#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <cmath>

using namespace std;

template <class T>
struct segment_tree
{
    T *root;         /*Pointer to the root*/
    int srArraySize; /*Segment tree array tall*/
    int originalArraySize;

    void print()
    {
        for (int i = 0; i < srArraySize; i++)
        {
            cout << root[i] << "->";
        }
        cout << endl;
    }

    segment_tree(vector<T> vec_original)
    {
        originalArraySize = vec_original.size();
        Separate_memory_for_segment_t(vec_original);
        fill_up_segment_tree(vec_original);
    }

    ~segment_tree()
    {
        delete root;
        root = NULL;
        srArraySize = 0;
        originalArraySize = 0;
    }

    void Separate_memory_for_segment_t(const vector<T> &vec_original)
    {
        int segment_tree_tall = ceil(log2(originalArraySize));
        int num_max_of_leaves = (2 * pow(2, segment_tree_tall)) - 1;

        srArraySize = num_max_of_leaves;
        root = new T[srArraySize];
    }

    void fill_up_segment_tree(const vector<T> &vec_original)
    {
        int index_of_root_node = 0;
        int index_of_root_node_start = 0;
        int index_of_root_node_end = originalArraySize - 1;
        fill_up_segment_tree_recursive(index_of_root_node,
                                       index_of_root_node_start,
                                       index_of_root_node_end,
                                       vec_original);
    }

    T fill_up_segment_tree_recursive(int index_of_root_node,
                                     int index_node_start,
                                     int index_node_end,
                                     const vector<T> &vec_original)
    {
        /*En este caso solo hay un posible valor minimo */
        if (index_node_start == index_node_end)
        {
            T aux_value = vec_original[index_node_start];
            root[index_of_root_node] = aux_value;
            return aux_value;
        }
        /*Debemos encontrar el nodo minimo preguntando recursivamente*/
        /*Calculamos el inicio y el final de los indices de los dos hijos*/

        int index_middle = index_node_start + ((index_node_end - index_node_start) / 2);

        int index_left_child_node = 2 * index_of_root_node + 1;
        int index_left_child_node_start = index_node_start;
        int index_left_child_node_end = index_middle;

        int index_right_child_node = 2 * index_of_root_node + 2;
        int index_right_child_node_start = index_middle + 1;
        int index_right_child_node_end = index_node_end;

        T left_child_min = fill_up_segment_tree_recursive(
            index_left_child_node,
            index_left_child_node_start,
            index_left_child_node_end,
            vec_original);

        T right_child_min = fill_up_segment_tree_recursive(
            index_right_child_node,
            index_right_child_node_start,
            index_right_child_node_end,
            vec_original);

        T aux_value = min(left_child_min, right_child_min);

        root[index_of_root_node] = aux_value;

        return aux_value;
    }
};

int main()
{
    vector<int> segment_tree_struc;
    segment_tree_struc.push_back(20);
    segment_tree_struc.push_back(-6);
    segment_tree_struc.push_back(4);
    segment_tree_struc.push_back(8);
    segment_tree_struc.push_back(74);
    segment_tree_struc.push_back(-7);
    segment_tree_struc.push_back(8);
    segment_tree_struc.push_back(12);
    for (size_t i = 0; i < segment_tree_struc.size(); i++)
    {
        cout << segment_tree_struc[i] << "->";
    }

    cout << "\n";
    segment_tree<int> tree(segment_tree_struc);
    tree.print();
    return 0;
}