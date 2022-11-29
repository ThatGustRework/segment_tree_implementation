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
            cout<<root[i]<< "->";
        }
        cout<<endl;
    }

    segment_tree(vector<T> vec_original)
    {
        originalArraySize = vec_original.size();
    }
    ~segment_tree()
    {
        delete root;
        root = NULL;
        srArraySize = 0;
        originalArraySize = 0;
    }
    void Separate_memory_for_segment_t(vector<T> &vec_original)
    {
        int segment_tree_tall = ceil(log2(originalArraySize));
        int num_max_of_leaves = (2 * pow(2, segment_tree_tall)) - 1;

        srArraySize = num_max_of_leaves;
        root = new (T)[srArraySize];
    }
    void fill_up_segment_tree(vector<T> vec_original)
    {
        int index_of_root_node = 0;
        int index_of_root_node_start = 0;
        int index_of_root_node_end = originalArraySize - 1;
    }

    T fill_up_segment_tree_recursive(int index_of_root_node,
                                     int index_node_start, int index_node_end, vector<T> &vec_original)
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
        else
        {
            int index_middle = index_node_start + ((index_node_end - index_node_start) / 2);

            int index_left_child_node = 2 * index_of_root_node + 1;
            int index_left_child_node_start = index_node_start;
            int index_left_child_node_end = index_middle;

            int index_right_child_node = 2 * index_of_root_node + 2;
            int index_right_child_node_start = index_node_start;
            int index_right_child_node_end = index_middle;

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
    }
};

int main()
{
    return 0;
}