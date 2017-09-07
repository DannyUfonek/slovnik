#include <iostream>
#include <string>

/*
* SLOVNIK
* anglický slovník implementovaný pomocí vyhledávacího stromu.
* Každý uzel obsahuje písmeno a bool, zda-li cesta stromem konèící tímto písmenem je slovo.
* Interakce s uživatelem probíhá pomocí konzole, slovník je schopný analyzovat text oddìlený mezerami a interpunkcí v souboru a najít chybnì napsaná slova.
* Slovník výslednì chybnì napsaná slova buï pouze vypíše èi rovnou opraví v souboru, slovo po slovì.
* Slovník je nutno nahrát do pamìti
* Mapa znaků:
* 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25
* a b c d e f g h i j k  l  m  n  o  p  q  r  s  t  u  v  w  x  y  z
*/

using namespace std;

struct node{
    int letter;
    bool isWord;
    node*next[26];
};

node*root = NULL;

void addNode(int k, string d)
{
    // dva ukazatele, prvnim hledam, druhym hlidam predchozi node (abych to pak mohl spojit)
    node* p = koren;
    node* predchozi = NULL;

    while (p!=NULL)
    {
        if (k < p->klic)
        {
            predchozi = p;
            p = p->mensi;
        }
        else if (k > p->klic)
        {
            predchozi = p;
            p = p->vetsi;
        }
        else
        {
            // dorazili jsme na node, ktery uz je obsazeny, co delat?
            return;
        }
    }
    // klic nebyl ve stromu

    node* u = new node;
    u->klic = k;
    u->data = d;
    u->mensi = NULL;
    u->vetsi = NULL;
    if (predchozi == NULL)
    {
        koren = u;
    }
    else if (k < predchozi->klic)
    {
        predchozi->mensi = u;
    }
    else
    {
        predchozi->vetsi = u;
    }
}

int main()
{
    cout << "Hello world!" << endl;
    return 0;
}
