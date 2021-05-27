#include <iostream>
#include <fstream>

const bool black = 1;
const bool red = 0;

using namespace std;

ifstream fin("abce.in");
ofstream fout("abce.out");

struct node{
    int key;
    node *left, *right;
	node *parent;
	bool color; // 1 - black; 0 - red;
	node( int x ) : key( x ), left( NULL ), right( NULL ), parent( NULL ), color( red ) {}
	node* grandparent(){
	    return parent -> parent;
	}
	node* sibling(){
        if( parent -> left == this )
            return parent -> right;
        return parent -> left;
	}
	bool red_child(){
        if( left && left -> color == red )
            return 1;
        if( right && right -> color == red )
            return 1;
        return 0;
	}
};

class RBTree{

    node *root;

    bool Empty(){
        return root == NULL;
    }

    node* Search_node( int val ){
        node *temp = root;

        while( temp && temp -> key != val ){
            if( val > temp -> key )
                temp = temp -> right;
            else temp = temp -> left;
        }

        return temp;
    }
    node* Insert_node( int val ){
        node *temp = root;

        while( temp ){
            if( val > temp -> key )
                if( temp -> right ) temp = temp -> right;
                else{
                    node* inserted_node = new node( val );
                    inserted_node -> parent = temp;
                    temp -> right = inserted_node;
                    temp = inserted_node;
                    break;
                }
            else
                if( temp -> left )temp = temp -> left;
                else{
                    node* inserted_node = new node( val );
                    inserted_node -> parent = temp;
                    temp -> left = inserted_node;
                    temp = inserted_node;
                    break;
                }
        }

        return temp;
    }
    void rotate_left( node* nod ){
        node* right_node = nod -> right;

        nod -> right = right_node -> left;
        if( nod -> right )
            nod -> right -> parent = nod;

        right_node -> parent = nod -> parent;

        if( right_node -> parent == NULL )
            root = right_node;
        else
            if( nod -> parent -> left == nod ){
                nod -> parent -> left = right_node;
            }
            else{
                nod -> parent -> right = right_node;
            }

        nod -> parent = right_node;

        right_node -> left = nod;
    }
    void rotate_right( node* nod ){
        node* left_node = nod -> left;

        nod -> left = left_node -> right;
        if( nod -> left )
            nod -> left -> parent = nod;

        left_node -> parent = nod -> parent;

        if( left_node -> parent == NULL )
            root = left_node;
        else
            if( nod -> parent -> left == nod ){
                nod -> parent -> left = left_node;
            }
            else{
                nod -> parent -> right = left_node;
            }

        nod -> parent = left_node;

        left_node -> right = nod;
    }
    void Fix_node( node* nod ){
        if( nod == root ){
            nod -> color = black;
            return;
        }

        node* uncle = NULL;

        if( nod -> parent -> color == red ){
            if( nod -> grandparent() -> left == nod -> parent ){
                uncle = nod -> grandparent() -> right;


                if( uncle && uncle -> color == red ){
                    uncle -> color = nod -> parent -> color = black;
                    nod -> grandparent() -> color = red;
                    nod = nod -> grandparent();
                }
                else{
                    // left left case
                    if( nod == nod -> parent -> left ){
                        rotate_right( nod -> grandparent() );
                        swap( /*old grandparent*/ nod -> parent -> right -> color, nod -> parent -> color );
                        nod = nod -> parent;
                    }
                    // left right case
                    else{
                        rotate_left( nod -> parent );
                        nod = nod -> left;

                        rotate_right( nod -> grandparent() );
                        swap( /*old grandparent*/ nod -> parent -> right -> color, nod -> parent -> color );
                        nod = nod -> parent;
                    }
                }
            }
            else{
                uncle = nod -> grandparent() -> left;

                if( uncle && uncle -> color == red ){
                    uncle -> color = nod -> parent -> color = black;
                    nod -> grandparent() -> color = red;
                    nod = nod -> grandparent();
                }
                else{
                    // right right case
                    if( nod == nod -> parent -> right ){
                        rotate_left( nod -> grandparent() );
                        swap( /*old grandparent*/ nod -> parent -> left -> color, nod -> parent -> color );
                        nod = nod -> parent;
                    }
                    // right left case
                    else{
                        rotate_right( nod -> parent );
                        nod = nod -> right;

                        rotate_left( nod -> grandparent() );

                        swap( /*old grandparent*/ nod -> parent -> left -> color, nod -> parent -> color );
                        nod = nod -> parent;
                    }
                }
            }
            Fix_node( nod );
        }
    }

    void Inorder( node* nod, node* lf, node* rg ){
        if( nod == NULL ) return;
        if( nod -> left && nod != lf ) Inorder( nod -> left, lf, rg );
        if( nod -> key >= lf -> key && nod -> key <= rg -> key ) fout << nod -> key << ' ';
        if( nod -> right && nod != rg ) Inorder( nod -> right, lf, rg );
    }

    node* Pred( int val ){
        node* temp = Search_node( val ) ;

        if( temp -> left ){

            temp = temp -> left;
            while( temp && temp -> right ){
                temp = temp -> right;
            }
            return temp;
        }

        node* p = temp -> parent;

        while( p && temp == p -> left ){
            temp = p;
            p = p -> parent;
        }
        return p;
    }

    node* Succ( int val ){
        node* temp = Search_node( val ) ;

        if( temp -> right ){
            temp = temp -> right;
            while( temp && temp -> left ){
                temp = temp -> left;
            }

            return temp;
        }

        node* p = temp -> parent;
        while( p && temp == p -> right ){
            temp = p;
            p = p -> parent;
        }
        return p;
    }

    void Delete_node( node* nod ){

        if( nod == NULL ) return;
        // node to be deleted is leaf
        if( nod -> left == NULL && nod -> right == NULL ){
            if( nod == root ){
                root = NULL;
                delete nod;
                return;
            }

            if( nod -> color == black )
                Fix_double_black( nod );
            else{
                //if( nod -> sibling() )
                //   nod -> sibling() -> color = red;
            }

            if( nod -> parent -> left == nod )
                nod -> parent -> left = NULL;
            else nod -> parent -> right = NULL;

            delete nod;
            return;
        }

        // nod has 1 child
        if( nod -> left == NULL || nod -> right == NULL ){
            node* child;
            if( nod -> left ) child = nod -> left;
            else child = nod -> right;

            if( nod == root ){
                swap( root -> key, child -> key );
                delete child;
                root -> right = root -> left = NULL;
                return;
            }

            //promote the child
            if( nod -> parent -> left == nod ){
                nod -> parent -> left = child;
            }
            else nod -> parent -> right = child;
            child -> parent = nod -> parent;

            if( nod -> color == black && child -> color == black )
                Fix_double_black( child );
            else child -> color = black;

            delete nod;
            return;
        }

        // nod has 2 children
        if( nod -> left && nod -> right ){
            node* successor = Succ( nod -> key );
            swap( successor -> key, nod -> key );
            Delete_node( successor );
        }
    }

    void Fix_double_black( node* nod ){
        if( nod == root )
            return;

        node* bro = nod -> sibling();

        if( nod -> sibling() == NULL ){
            Fix_double_black( nod -> parent );
            return;
        }

        if( nod -> sibling() -> color == red ){
            nod -> parent -> color = red;
            nod -> sibling() -> color = black;

            if( nod -> sibling() -> parent -> left == nod -> sibling() ){
                //left case
                rotate_right( nod -> parent );
            }
            else {
                //right case
                rotate_left( nod -> parent );
            }

            Fix_double_black( nod );
        }
        else{

            if( nod -> sibling() -> red_child() == 1 ){
                if( nod -> sibling() -> left && nod -> sibling() -> left -> color == red ){
                    if( nod -> sibling() -> parent -> left == nod -> sibling() ){
                        // sibling is on left --> left left case
                        nod -> sibling() -> left -> color = nod -> sibling() -> color;
                        nod -> sibling() -> color = nod -> parent -> color;
                        rotate_right( nod -> parent );
                    }
                    else{
                        // sibling is on right --> right left case
                        nod -> sibling() -> left -> color = nod -> parent -> color;
                        rotate_right( nod -> sibling() );
                        rotate_left( nod -> parent );
                    }
                }
                else{
                    if( nod -> sibling() -> parent -> right == nod -> sibling() ){
                        // sibling is on right --> right right case
                        nod -> sibling() -> right -> color = nod -> sibling() -> color;
                        nod -> sibling() -> color = nod -> parent -> color;
                        rotate_left( nod -> parent );
                    }
                    else{
                        // sibling is on left --> left right case
                        nod -> sibling() -> right -> color = nod -> parent -> color;
                        rotate_left( nod -> sibling() );
                        rotate_right( nod -> parent );
                    }
                }
                nod -> parent -> color = black;
            }
            else{
                // sibling has 2 black children
                nod -> sibling() -> color = red;
                if( nod -> parent -> color == black )
                    Fix_double_black( nod -> parent );
                else nod -> parent -> color = black;
            }
        }
    }

public:
    RBTree() : root( NULL ){}
    RBTree( int _key ) {
        root = new node ( _key );
        root -> color = black;
    }

    bool Search( int val ){
        if( Search_node( val ) == NULL ) return 0;
        return 1;
    }

    void Insert( int val ){
        if( Empty() ){
            root = new node( val );
            root -> color = black;
            return;
        }

        node *inserted_node = Insert_node( val );

        Fix_node( inserted_node );
    }

    void Delete( int val ){
        node* nod = Search_node( val );
        Delete_node( nod );
    }

    int Predecessor( int val ){

        if( Search( val ) == 1 )return val;

        Insert( val );
        int pred = Pred( val ) -> key;
        Delete( val );
        return pred;
    }

    int Successor( int val ){

        if( Search( val ) == 1 )return val;

        Insert( val );
        int succ = Succ( val ) -> key;
        Delete( val );

        return succ;
    }

    void Sorted( int lf_val, int rg_val ){

        lf_val = Successor( lf_val );
        rg_val = Predecessor( rg_val );

        node* lf = Search_node( lf_val );
        node* rg = Search_node( rg_val );

        Inorder( root, lf, rg );
        fout << '\n';
    }

};

int Q;
int op, x, y;

int main()
{
    fin >> Q;

    RBTree RBT;
    for( int i = 1; i <= Q; ++i )
    {
        fin >> op >> x;

        if( op == 1 ){
            RBT.Insert( x );
        }
        if( op == 2 ){
            RBT.Delete( x );
        }
        if( op == 3 ){
            if( RBT.Search( x ) == 1 )
                fout << 1 << '\n';
            else fout << 0 << '\n';
        }
        if( op == 4 ){
            fout << RBT.Predecessor(x) << '\n';
        }
        if( op == 5 ){
            fout << RBT.Successor(x) << '\n';
        }
        if( op == 6 ){
            fin >> y;
            RBT.Sorted( x, y );
        }
    }
    return 0;
}
