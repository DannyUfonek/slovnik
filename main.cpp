#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <ctype.h>

/*
* SLOVNÍK
* anglický slovník implementovaný pomocí stromu.
* Každý uzel obsahuje písmeno a bool, zda-li cesta stromem končící tímto písmenem je slovo.
* Interakce s uživatelem probíhá pomocí konzole, slovník je schopný analyzovat text oddělený mezerami a interpunkcí v souboru a najít chybně napsaná slova.
* Slovník výsledně chybně napsaná slova vypíše buď do konzole anebo do souboru.
* Mapa znaků:
* 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25
* a b c d e f g h i j k  l  m  n  o  p  q  r  s  t  u  v  w  x  y  z
*
* zdroj slovníku: https://github.com/dwyl/english-words
*
*/

using namespace std;

const int alphabetSize = 26;

struct Node{
    bool isWord;
    Node*child[alphabetSize];
};

Node*root = NULL;


const char* DICTPATH = "dictionary.txt";
const char* OUTPATH = "output.txt";
const char* TEXTPATH = "text.txt";
Node* endOfString(string s, bool isInsert);
bool dictReady = false;

bool isWord(string s)
{
    // hledej slovo ve slovniku
    Node* current = endOfString(s, false);
    if (current == NULL)
    {
        return false;
    }
    else
    {
        return current->isWord;
    }
}

void insertWord(string s)
{
    Node* current = endOfString(s, true);
    if (current->isWord == false)
    {
        current->isWord = true;
        //cout << "slovo " << s << " pridano"  << endl;
    }
    else
    {
        //cout << "toto slovo jiz ve slovniku je" << endl;
    }
}

//vyrobi novy node na miste current
void initNode(Node* &current)
{
    current = new Node;
    current->isWord = false;
    for (int i = 0; i < alphabetSize; i++)
    {
       current->child[i] = NULL;
    }
}

void delNode(Node* parent)
{
    // rekurzivni mazani stromu
    for (int i = 0; i < alphabetSize; i++)
    {
        if (parent->child[i] != NULL)
            delNode(parent->child[i]);
    }
    delete parent;
}


//hlavni funkce, vraci ukazatel na posledni pismeno hledaneho/pridavaneho slova
//isInsert tells it whether it needs to initialize new Nodes
Node* endOfString(string s, bool isInsert)
{
    Node* current = root;
    Node* next = root;
    for (unsigned int i = 0; i < s.length(); i++)
    {
        // iteruj pres string, zkontroluj nejdriv u kazdeho znaku jestli je to pismeno
        // takhle se i zbavime carek a tecek
        if (std::isalpha(s[i]) != 0)
        {
            //cout << "->" << s[i];
            // prepocitej z velkych do malych do 0-25
            int letter = (std::tolower((int)s[i]) - 'a');

            // posun next na dalsi pismeno
            next = current->child[letter];
            if (next == NULL)
            {
                // kdyz node neexistuje
                if (isInsert == false)
                {
                    // kdyz hledame a node neni, neni to slovo
                    return NULL;
                }
                else
                {
                   //pokud pridavame, vyrob novy
                    initNode(next);
                    current->child[letter] = next;
                }
            }
            // posun i current
            current = current->child[letter];
        }
        else
        {
            cout << s[i] << " neni pismeno" << endl;
        }
    }
    //cout << endl;
    return current;
}

bool initDictionary()
{
    // priprav strom
    root = new Node;
    root->isWord = false;
    for (int i = 0; i < 26; i++)
    {
        root->child[i] = NULL;
    }

    // priprav soubor slovniku
    ifstream dictfile;
    dictfile.open(DICTPATH);

    if (dictfile.is_open())
    {
        cout << "vyrabim slovnik: " << endl;
        for (string word; dictfile >> word; )
        {
            // pridavej slova do slovniku
            insertWord(word);
        }
    }
    else
    {
        return false;
    }

    //zadny problem, muzeme zavrit
    dictfile.close();
    dictReady = true;
    return true;
}

void unloadDictionary()
{
    cout << "odstranuji slovnik" << endl;
    delNode(root);
    dictReady = false;
}

bool spellcheck(const char* filepath, bool fileOutput = false)
{
    // pocet chyb
    int counter = 0;
    ifstream textfile;
    ofstream output;
    textfile.open(filepath);
    if (fileOutput == true)
    {
        // priprav soubor pro vypis
        output.open(OUTPATH);
        output << "Spellcheck textu z " << filepath << endl << " Chybna slova:" << endl;
    }
    else
    {
        cout << "Spellcheck textu z " << filepath << endl << " Chybna slova:" << endl;
    }

    if (textfile.is_open() && (fileOutput == output.is_open())) // nerovnost znamena chybu
    {
        // cout << "oteviram text" << endl;
        for (string word; textfile >> word; )
        {
            // zkontroluj jestli slovo je ve slovniku
            Node*lastsearch = endOfString(word, false);
            if (lastsearch == NULL)
            {
                // node neexistuje
                if (fileOutput == true)
                    output << word << endl;
                else
                    cout << word << endl;
                counter++;
            }
            else
            {
                // node existuje, ale neni to slovo
                if (lastsearch->isWord == false)
                {
                    if (fileOutput == true)
                        output << word << endl;
                    else
                        cout << word << endl;
                    counter++;
                }
            }
        }
    }
    else
    {
        // chyba
        return false;
    }

    if (fileOutput == true)
    {
        // dokonci soubor pro vypis
        output << "Spellcheck ukoncen. Chybnych slov: " << counter << endl;
        output.close();
    }
    else
    {
        cout << "Spellcheck ukoncen. Chybnych slov: " << counter << endl;
    }
    return true;
}

int main ()
{
    // inicializuj
    int prikaz = -1;
    string vstup = "";

    while (prikaz < 4)
    {
        prikaz = -1;

        cout << "-------- ANGLICKY SLOVNIK ----------" << endl <<
        "Zadejte prikaz:" << endl <<
        "0 Nahrat slovnik z " << DICTPATH << endl <<
        "1 Hledat konkretni slovo ve slovniku " << endl <<
        "2 Zkontrolovat text v " << TEXTPATH << endl <<
        "3 Zkontrolovat text v " << TEXTPATH << " a vypsat chyby do " << OUTPATH << endl <<
        "4 Exit" << endl;

        while (true)
        {
            // ziskej prikaz od uzivatele bezpecne (operator << to nezvlada sam):
            getline(cin, vstup);

            // This code converts from string to number safely.
            stringstream myStream(vstup);
            if (myStream >> prikaz)
                if (0 <= prikaz && prikaz <= 4)
                    break;

            cout << "Spatne cislo, zadejte nejake znovu: " << endl;
        }

        if (prikaz == 0)
        {
            if (dictReady == false)
            {
                // nahraj slovnik - vyrob strom ze slov
                bool success = initDictionary();
                if (success == true)
                    cout << "Uspesne nacten slovnik z " << DICTPATH << endl;
                else
                    cout << "Nastal problem pri nacitani slovniku z " << DICTPATH << endl;
            }
            else
                cout << "slovnik jiz byl nacten!" << endl;
        }
        if (dictReady == true)
        {
            if (prikaz == 1)
            {
                string word;
                cout << "zadejte slovo k vyhledani: ";
                cin >> word;
                cout << endl;
                Node*last = endOfString(word, false);
                cout << endl;
                if (last == NULL)
                    cout << word << " nenalezeno" << endl;
                else
                {
                    if (last->isWord == true)
                        cout << word << " nalezeno ve slovniku" << endl;
                    else
                        cout << word << " nenalezeno" << endl;
                }

            }
            else if (prikaz == 2)
            {
                // zkontroluj text
                spellcheck(TEXTPATH, false);
            }
            else if (prikaz == 3)
            {
                spellcheck(TEXTPATH, true);
            }
            else if (prikaz == 4)
            {
                unloadDictionary();
                break;
            }
        }
        else
        {
            cout << "pred provedenim tohoto prikazu nejdrive nactete slovnik!" << endl;
        }

    }
}
