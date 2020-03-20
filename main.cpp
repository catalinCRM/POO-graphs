#include <iostream>
#include <fstream>
using namespace std;

int viz2[1001];

class muchie
{
public:
    int i, j;
};

class Graf_neorientat
{
    muchie *M;  //lista de muchii
    int **matrice;  //matrice de adiacenta
    int n;  //nr noduri
    int m;  //nr muchii
    int *viz;  //vector vizitati
public:
    Graf_neorientat(int lista_muchii[1001][2], int nr_noduri, int nr_muchii);
    Graf_neorientat(const Graf_neorientat &g);
    ~Graf_neorientat();
    void parcurgere_adancime(int nod);
    void parcurgere_latime(int nod);
    void matricea_drumurilor();
    void componente_conexe();
    int graf_conex(Graf_neorientat g);
    friend ostream& operator<<(ostream&, Graf_neorientat);
    friend Graf_neorientat operator+(const Graf_neorientat &f, const Graf_neorientat &g);
};

Graf_neorientat::Graf_neorientat(int lista_muchii[1001][2], int nr_noduri, int nr_muchii)  //constructor
{
    n = nr_noduri;
    m = nr_muchii;
    M = new muchie[m];
    viz = new int[n+1];
    matrice = new int*[n+1];
    for(int k=1; k<=n; k++)
        viz[k] = 0;
    for(int k=1; k<=n; k++)
        matrice[k] = new int[n+1];
    for(int k=1; k<=n; k++)
        for(int p=1; p<=n; p++)
            matrice[k][p] = 0;
    for(int k=0; k<m; k++)
    {
        M[k].i = lista_muchii[k][0];
        M[k].j = lista_muchii[k][1];
        matrice[M[k].i][M[k].j] = 1;
        matrice[M[k].j][M[k].i] = 1;
    }
}

Graf_neorientat::Graf_neorientat(const Graf_neorientat &g)  //constructor de copiere
{
    n = g.n;
    m = g.m;
    M = new muchie[m];
    viz = new int[n+1];
    matrice = new int*[n+1];
    for(int k=1; k<=n; k++)
        viz[k] = 0;
    for(int k=1; k<=n; k++)
        matrice[k] = new int[n+1];
    for(int k=1; k<=n; k++)
        for(int p=1; p<=n; p++)
            matrice[k][p] = 0;
    for(int k=0; k<m; k++)
    {
        M[k].i = g.M[k].i;
        M[k].j = g.M[k].j;
        matrice[M[k].i][M[k].j] = 1;
        matrice[M[k].j][M[k].i] = 1;
    }
}

Graf_neorientat::~Graf_neorientat()  //destructorul
{
    for(int i=1; i<=n; i++)
        delete [] matrice[i];
    delete [] matrice;
    delete [] M;
    delete [] viz;
    n = 0;
    m = 0;
}

ostream& operator<<(ostream& out, Graf_neorientat g)
{
    for(int i=1; i<=g.n; i++)
    {
        for(int j=1; j<=g.n; j++)
            out<<g.matrice[i][j]<<" ";
        out<<endl;
    }
    return out;
}

void Graf_neorientat::parcurgere_adancime(int nod)
{
    //cout<<nod<<" ";
    int i;
    viz[nod]=1;
    for(i=1; i<=n; i++)
        if (matrice[nod][i]==1 && viz[i]==0)
            parcurgere_adancime(i);

}

void Graf_neorientat::parcurgere_latime(int nod)
{
    int i, k, st, dr, x[n];
    st = dr = 1;
    for(int i=1; i<=n; i++)
        x[i]=0;
    x[1] = nod;
    viz[nod] = 1;
    while(st <= dr)
    {
        k = x[st];
        for(i=1; i<=n; i++)
            if(viz[i] == 0 && matrice[k][i] == 1)
            {
                viz[i] = 1;
                x[++dr] = i;
                cout<<i<<" ";
            }
        st++;
    }
}

void Graf_neorientat::matricea_drumurilor()
{
    int a[n+1][n+1];
    for(int i=1 ; i<=n ; i++)
        for(int j=1 ; j<=n ; j++)
            a[i][j] = matrice[i][j];
    for(int k=1 ; k<=n ; k++)
        for(int i=1 ; i<=n ; i++)
            for(int j=1 ; j<=n ; j++)
                if( i!=j && a[i][j]==0)
                    a[i][j] = a[i][k] * a[k][j];
    for(int i=1 ; i<=n ; i++)
    {
        for(int j=1 ; j<=n ; j++)
            cout<<a[i][j]<<" ";
        cout<<endl;
    }
}

void dfs(int nod, int n, int **matrice, int *viz, int nrcomp)
{
    int i;
    viz[nod] = nrcomp;
    for(i=1; i<=n; i++)
        if (matrice[nod][i]==1 && viz[i]==0)
            dfs(i, n, matrice, viz, nrcomp);
}

void Graf_neorientat::componente_conexe()
{
    int i, j, nrcomp=0;
    for(i=1; i<=n; i++)
        if (viz[i]==0)
        {
            nrcomp++;
            dfs(i, n, matrice, viz, nrcomp);
        }
    for(i=1; i<=nrcomp; i++)
    {
        cout<<"Componenta "<<i<<": ";
        for(j=1; j<=n; j++)
            if (viz[j]==i)
                cout<<j<<" ";
        cout<<endl;
    }
}

int Graf_neorientat::graf_conex(Graf_neorientat g)
{
    g.parcurgere_adancime(1);
    for(int i=1; i<=n; i++)
        if(g.viz[i] == 0)
            return 0;
    return 1;
}

Graf_neorientat operator+(const Graf_neorientat &f, const Graf_neorientat &g)
{
    int noduri = f.n + g.n;
    int muchii = f.m + g.m;
    int c[2002][2], k = 0;
    for(int a=0; a<f.m; a++)
    {
        c[k][0] = f.M[a].i;
        c[k][1] = f.M[a].j;
        k++;
    }
    for(int a=0; a<g.m; a++)
    {
        int ok = 1;
        for(int b=0; b<f.m; b++)
            if(c[b][0] == g.M[a].i && c[b][1] == g.M[a].j || c[b][1] == g.M[a].i && c[b][0] == g.M[a].j)
                ok = 0;
        if(ok == 1)
        {
            c[k][0] = g.M[a].i;
            c[k][1] = g.M[a].j;
            k++;
        }
    }
    return Graf_neorientat(c, noduri, muchii);
}

int main()
{
    ifstream f("date2.in");
    int n, m;
    f>>n>>m;
    int lista[m][2];
    int x, y;
    for(int i=0; i<m; i++)
    {
        f>>x>>y;
        lista[i][0] = x;
        lista[i][1] = y;
    }
    int n2, m2;
    f>>n2>>m2;
    int lista2[m2][2];
    for(int i=0; i<m2; i++)
    {
        f>>x>>y;
        lista2[i][0] = x;
        lista2[i][1] = y;
    }
    Graf_neorientat ob1(lista, n, m);
    cout<<ob1<<endl;
    ob1.parcurgere_adancime(1);
    Graf_neorientat ob2(ob1);
    ob2.parcurgere_latime(1);
    cout<<endl;
    Graf_neorientat ob3(lista, n, m);
    ob3.matricea_drumurilor();
    cout<<endl;
    Graf_neorientat ob4(lista, n, m);
    ob4.componente_conexe();
    cout<<endl;
    cout<<ob1.graf_conex(ob1)<<endl;
    Graf_neorientat ob5(lista2, n2, m2);
    Graf_neorientat ob6=ob1+ob5;
    return 0;
}
