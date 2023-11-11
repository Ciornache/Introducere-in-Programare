#include <iostream>
#include <fstream>
#include <cstring>

#define MAX 1000

using namespace std;

ifstream fin ("inaltime1.in");
ofstream fout ("inaltime1.out");

struct node
{
    int val;
    node * stanga = NULL, *dreapta = NULL;
};

node * root;
bool fr[MAX + 5];
int n, st[MAX + 5], dr[MAX + 5], val[MAX + 5];

void construire(node * nod)
{
    int x;
    fin >> x;
    if(x)
    {
        node * newNode = new node;
        newNode->val = x;
        nod->stanga = newNode;
        construire(newNode);
    }
    fin >> x;
    if(x)
    {
        node * newNode = new node;
        newNode->val = x;
        nod->dreapta = newNode;
        construire(newNode);
    }
}


///Inordine:St,Rad,Dr
///Preordine:Rad,St,Dr
///Postordine:St,Dr,Rad

void preordine(node * nod)
{
    if(nod == NULL)
        return;
    fout << nod->val << ' ';
    preordine(nod->stanga);
    preordine(nod->dreapta);
}

void inordine(node * nod)
{
    if(nod == NULL)
        return;
    inordine(nod->stanga);
    fout << nod->val << ' ';
    inordine(nod->dreapta);
}

void postordine(node * nod)
{
    if(nod == NULL)
        return;
    postordine(nod->stanga);
    postordine(nod->dreapta);
    fout << nod->val << ' ';
}

int inaltime(node * nod)
{
    if(nod == NULL)
        return 0;
    return 1 + max(inaltime(nod->stanga), inaltime(nod->dreapta));
}

void calculate(node * nod, int height, double * divs, int * cnt)
{
    if(nod == NULL)
        return;
    divs[height] += nod->val, cnt[height]++;
    calculate(nod->stanga, height + 1, divs, cnt);
    calculate(nod->dreapta, height + 1, divs, cnt);
}

double * medieDiv(node * root)
{
    int h = inaltime(root);
    double * div = new double[h + 1];
    int * cnt = new int[h+1];
    for(int i = 0; i < h; i++)
        div[i] = 0, cnt[i] = 0;
    calculate(root, 0, div, cnt);
    for(int i = 0; i < h; i++)
        div[i] = div[i]/(1.0*cnt[i]);
    return div;
}

void addChar(char * ch, char c, int &ind)
{
    ch[ind++]=c;
}

void build(node * nod, char * ch, int &ind)
{
    int v[10]= {0};
    int x = nod->val;
    while(x)
        v[++v[0]]=x%10,x/=10;
    for(int j = v[0]; j >= 1; --j)
        addChar(ch,v[j]+'0',ind);
    if(nod->stanga)
    {
        addChar(ch,'(',ind);
        build(nod->stanga,ch,ind);
        addChar(ch,')',ind);
    }
    if(nod->dreapta)
    {
        addChar(ch,'(',ind);
        build(nod->dreapta,ch,ind);
        addChar(ch,')',ind);
    }
}

char * tree2str(node * nod)
{
    char * ch = new char[1001];
    int ind = 0;
    build(nod, ch, ind);
    ch[ind]='\0';
    return ch;
}

void reverseBinaryTree(node * nod)
{
    if(nod == NULL)
        return;
   swap(nod->dreapta,nod->stanga);
   reverseBinaryTree(nod->dreapta);
   reverseBinaryTree(nod->stanga);
}

int main()
{
    int x;
    fin >> x;
    root = new node;
    root->val = x;
    construire(root);
    ///postordine(root);
    ///fout << inaltime(root);
//    double * rez = medieDiv(root);
//    cout << rez[1];
    reverseBinaryTree(root);
    preordine(root);
//    char * ch = tree2str(root);
//    cout << ch;
}
