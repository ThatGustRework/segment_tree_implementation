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

    void dot(string filename)
    {
        ofstream mydot;
        mydot.open(filename, ios::out);

        mydot << "digraph g {\n";
        mydot << "node [shape=record, height=0.1];\n";
        for (int i = 1; i < srArraySize + 1; i++)
        {
            mydot << "node_" << root[i - 1] << "_" << i - 1 << "_"
                  << " [label = \"<l> | <m> " << root[i - 1] << " | <r>\"];\n";

            if (i * 2 <= srArraySize)
            {
                mydot << "node_" << root[i - 1] << "_" << i - 1 << "_"
                      << ":l -> node_" << root[i * 2 - 1] << "_" << i * 2 - 1 << "_"
                      << ":m;\n";
            }

            if (i * 2 + 1 <= srArraySize)
            {
                mydot << "node_" << root[i - 1] << "_" << i - 1 << "_"
                      << ":r -> node_" << root[i * 2] << "_" << i * 2 << "_"
                      << ":m;\n";
            }
        }
        mydot << "}";
        mydot.close();
    }

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
        srArraySize = (2 * pow(2, segment_tree_tall)) - 1;

        root = new T[srArraySize];
    }

    void fill_up_segment_tree(const vector<T> &vec_original)
    {
        /*int index_of_root_node = 0;
        int index_of_root_node_start = 0;
        int index_of_root_node_end = originalArraySize - 1;
        fill_up_segment_tree_recursive(0,
                                       0,
                                       originalArraySize-1,
                                       vec_original);*/
        fill_up_segment_tree_util_v2(0, 0, originalArraySize - 1, vec_original);
    }

    int get_middle(int index_node_start, int index_node_end)
    {
        return index_node_start + (index_node_end - index_node_start) / 2;
    }

    T fill_up_segment_tree_util_v2(int index_of_root_node,
                                   int index_node_start,
                                   int index_node_end,
                                   const vector<T> &vec_original)
    {
        if (index_node_start == index_node_end)
        {
            root[index_of_root_node] = vec_original[index_node_start];
            return vec_original[index_node_start];
        }

        int index_middle = get_middle(index_node_start, index_node_end);
        root[index_of_root_node] = fill_up_segment_tree_util_v2(
                                       index_of_root_node * 2 + 1,
                                       index_node_start,
                                       index_middle, vec_original) +
                                   fill_up_segment_tree_util_v2(
                                       index_of_root_node * 2 + 2,
                                       index_middle + 1,
                                       index_node_end,
                                       vec_original);

        return root[index_of_root_node];
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

        int index_middle = get_middle(index_node_start, index_node_end);

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

    /*------------------*/

    void update_recursive(int new_val_id, T new_val, int index_of_root_node, int index_node_start, int index_node_end)
    {
        /*Hay solo dos casos para la siguiente condicional*/
        if (index_node_start == index_node_end && index_node_start == new_val_id)
        {
            root[index_of_root_node] = new_val;

            return;
        }

        /*Actualizamos recursivamente el array*/
        int index_middle = index_node_start + ((index_node_end - index_node_start) / 2);

        int index_left_child_node = 2 * index_of_root_node + 1;
        int index_right_child_node = 2 * index_of_root_node + 2;

        if (index_node_start <= new_val_id && new_val_id <= index_middle)
        {
            int index_left_child_node_start = index_node_start;
            int index_left_child_node_end = index_middle;

            update_recursive(new_val_id,
                             new_val,
                             index_left_child_node,
                             index_left_child_node_start,
                             index_left_child_node_end);
        }
        else
        {
            int index_right_child_node_start = index_middle + 1;
            int index_right_child_node_end = index_node_end;

            update_recursive(new_val_id,
                             new_val,
                             index_right_child_node,
                             index_right_child_node_start,
                             index_right_child_node_end);
        }
        min(root[index_left_child_node], root[index_right_child_node]);
        T aux_value = min(root[index_left_child_node], root[index_right_child_node]);

        root[index_of_root_node] = aux_value;
    }

    void update(int new_val_id, T new_val)
    {
        cout << "Actualizando valor en el indice " << new_val_id << " a valor de " << new_val << endl;

        if (new_val_id < 0 || new_val_id > originalArraySize - 1)
        {
            throw "Indice esta fuera del rango.\n";
        }

        int rootindex_of_root_node = 0;
        int rootindex_node_start = 0;
        int rootindex_node_end = originalArraySize - 1;
        update_recursive(new_val_id, new_val, rootindex_of_root_node, rootindex_node_start, rootindex_node_end);
    }

    //////query/////
    T RecursivelySearchForMin(
        int l,int r, int node_index,
        int node_start_index,int node_end_index
    )
    {
        if (node_end_index<l||r<node_start_index)
        {
            T value = 10000000;
            return value;
        }else if (l<=node_start_index && node_end_index<=r)
        {
            T value = root[node_index];
            return value
        }else
        {
            int middle_index = node_start_index+((node_end_index-node_start_index)/2);
            int left_child_node_index = 2*node_index+1;
            int left_child_start_index = node_start_index;
            int left_child_end_index = middle_index;
            int right_child_node_index = 2*node_index+2;
            int right_child_start_index = middle_index+1;
            int right_child_end_index = node_end_index;

            T left_child_min = RecursivelySearchForMin(
                l,r,left_child_node_index,
                left_child_start_index,left_child_end_index
            );

            T right_child_min = RecursivelySearchForMin(
                l,r,right_child_node_index,
                right_child_start_index,right_child_end_index
            );

            T value = std::min(left_child_min,right_child_min);
            return value;
        }
    }

    T query(int l,int r)
    {
        if(l>r)
        {
            throw "L debe ser <= R.\n";
        }else if (l<0)
        {
            throw l+"esta fuera del rango valido\n";//
        }else if (r>originalArraySize-1)
        {
            throw "R esta fuera del rango valido.\n";
        }

        int root_node_index = ;
        int root_node_start_index = 0;
        int root_node_end_index = originalArraySize -1;
        T value = RecursivelySearchFornMin(L,R,root_node_end_index,root_node_start_index,root_node_end_index);
        return value;       
        
    }
/////////
};

int main()
{
    vector<int> v2 = {1, 9, 3, 5, 8, 9, 5, 6};

    for (size_t i = 0; i < v2.size(); i++)
    {
        cout << v2[i] << "->";
    }

    cout << "\n";
    segment_tree<int> tree = segment_tree<int>(v2);
    tree.print();
    cout << tree.originalArraySize << "\n";
    cout << tree.srArraySize << "\n";
    cout << tree.root[0] << "\n";
    tree.dot("tree.dot");
    return 0;
}