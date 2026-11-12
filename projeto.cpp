#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;

struct Bloco{

    // Dimensões dos blocos
    int a;
    int b;
    int h;
    int pos_b; // Guardará a posição do b do bloco no array todos_b

};

struct ArvoreSegmentos{

    int m; // m = número de caixinhas (as folhas); o vetor t terá 2m posições

    vector<pair<int, int>> t; // Vetor que vai armazenar em cada elemento um par, o valor e o indice dele

    ArvoreSegmentos(int tamanho_m){ // Metodo construtor

        m = tamanho_m; // Guarda em m o número de caixinhas recebido.

        t.assign(2*m,make_pair(0,-1)); // Define o tamanho do vetor t e coloca 0 em cada posição
    }

    void atualizar(int p, int valor, int indice_bloco){ // Recebe a posição para atualizar, o valor e o indice

        // Como sabemos a arvore tem 2m, metade da arvore são os nós pais, a partir da sua segunda parte temos as folhas
        // A primeira posição real da arvore que guardará a altura estará em m
        int i = m + p; // Posição da folha a ser trocada

        // O max vai comparar o que já está la no nó (t[i]-> No inicio será 0) comparado com o valor a ser inserido agora
        t[i] = max(t[i], make_pair(valor, indice_bloco)); // Isso vai escrever na folha escolhida, (Sempre colocando o maior valor)

        // Loop que vai percorrer a arvore até a raiz
        while(i > 1){

            i = i/2; // Sobe para o nó pai

            t[i] = max(t[i], make_pair(valor, indice_bloco)); // O pai guarda o máximo da sua zona então comparamos ele com o novo valor

        }

    }

    pair<int,int> consultar(int p){

        pair<int,int> resultado = make_pair(0,-1);
        int l = m; // Folha da caixa (inicio de onde temos os valores de b)
        int r = m + p; // Folha da primeira caixa que fica de fora, ou seja os maiores valores a esquerda dela (O r não entra)
        while(l < r){

            // Siginifica que o nó irmao de l fica a esquerda, que está antes da zona e se subissemos iamos incluir um número que não queriamos
            if(l % 2 ==1){ // Se l for impar 

                resultado = max(resultado, t[l]);
                l = l + 1;
            }
            // Siginifica que o nó irmao de r fica a direita, que está depois da zona e se subissemos iamos incluir um número que não queriamos
            if(r % 2 == 1){ // Se r for impar 
                r = r - 1;
                resultado = max(resultado, t[r]);
            }
            
            //Subir um nível
            l = l/2;
            r = r/2;
        }
        return resultado;
    }

};

// Lê a quantidade de blocos e as dimensões de cada um, e gera as 3 orientações de cada bloco
void lerBlocos(vector<Bloco> &Blocos){

    // Variáveis temporárias para leitura
    int x;
    int y;
    int z;

    int n; // Quantidade de blocos

    // Leitura da quantidade de Blocos
    if (scanf("%d", &n) != 1) return;

    //Ciclo de leitura de dimensões para cada n

    for(int i = 1; i <= n; i++){

        Bloco bloco;

        // Leitura das dimensões, passando os valores para as variáveis temporárias
        if (scanf("%d %d %d", &x, &y, &z) != 3) return;

        // Preenchendo o bloco para a 1 orientação (x,y,z)
        bloco.a = min(x,y); // Garante que a fique com um valor menor ou igual a b
        bloco.b = max(x,y); // Garante que b fique com um valor maior ou igual a a
        bloco.h = z;

        // Adiciona ao fim do vetor Blocos, cada bloco
        Blocos.push_back(bloco);

        // Preenchendo o bloco para a 2 orientação (x,z,y)
        bloco.a = min(x,z); // Garante que a fique com um valor menor ou igual a b
        bloco.b = max(x,z); // Garante que b fique com um valor maior ou igual a a
        bloco.h = y;

        // Adiciona ao fim do vetor Blocos, cada bloco
        Blocos.push_back(bloco);

        // Preenchendo o bloco para a 3 orientação (y,z,x)
        bloco.a = min(y,z); // Garante que a fique com um valor menor ou igual a b
        bloco.b = max(y,z); // Garante que b fique com um maior ou igual a a
        bloco.h = x;

        // Adiciona ao fim do vetor Blocos, cada bloco
        Blocos.push_back(bloco);
    }
}

// Ordena os blocos por a crescente e, quando a for igual, por b crescente
void ordenarBlocos(vector<Bloco> &Blocos){

    //Ordenação crescente a partir de a, e quando a for igual por b crescente
    // Sort ordena o vetor no próprio vetor
    sort(Blocos.begin(), Blocos.end(), [](const Bloco &p, const Bloco &q){
        if(p.a != q.a){ // Se p.a != q.a (Quer dizer que os a's dos blocos são diferentes)
            return p.a < q.a; // Se eles são diferentes retornamos o menor primeiro
        }else{ // Caso eles não sejam diferentes, ou seja, iguais vamos para o b
            return p.b < q.b; // retornamos o menor b primeiro
        }
    });
}

// Cria o vetor todos_b (ordenado e sem repetidos) e preenche o pos_b de cada bloco
void comprimirB(vector<Bloco> &Blocos, vector<int> &todos_b){

    //Criando uma lista(array) com todos os b's

    for(int i = 0; i < (int)Blocos.size(); i++){

        todos_b.push_back(Blocos[i].b); // Preenchendo o vetor todos_b com o b de cada bloco.

    }

    // Ordena o vetor todos_b de menor para maior
    sort(todos_b.begin(), todos_b.end());

    // Depois de ordenar, se houver repetidos eles ficam lado a lado, logo podemos fazer o erase
    // O unique vai juntar os elementos distintos do vetor no inicio, colocando um ponteiro para o fim dos elementos distintos
    // O resto, são os elementos repetidos que apagamos.
    todos_b.erase(unique(todos_b.begin(), todos_b.end()), todos_b.end());

    // Agora vamos preencher a posição de b em todos_b
    for(int i = 0; i < (int)Blocos.size(); i++){

        //Preenchemos o array Blocos, para cada bloco, colocamos em pos_b a posição de b em todos_b
        //Lower_bound vai fazer pesquisa binária de todos_b(cortando ela ao meio a cada busca) do inicio ao fim e vai para quando encontrar o B inserido (Blocos[i].b)
        //Devolverá um marcador para onde parar e subtraimos com o inicio para saber a posição
        Blocos[i].pos_b = lower_bound(todos_b.begin(), todos_b.end(), Blocos[i].b ) - todos_b.begin();
    }
}

// Impressões de teste (apagar ou comentar antes de submeter ao Mooshak)
void imprimirDebug(const vector<Bloco> &Blocos, const vector<int> &todos_b){

    for(int i = 0; i < (int)Blocos.size(); i++){
        printf("Bloco %d -> %d %d %d\n", i, Blocos[i].a, Blocos[i].b, Blocos[i].h);
    }

    printf("Array de todos os b's ordenado e sem repetição: ");
    for (int i = 0; i < (int)todos_b.size(); i++) {
    printf("%d", todos_b[i]);
    }
    printf("\n");

    printf("Posição de cada b no array de todos os b's: ");
    for (int i = 0; i < (int)Blocos.size(); i++) {
    printf("%d", Blocos[i].pos_b);
    }
}

int main(){

    // Vetor(Array) que conterá os Blocos
    vector<Bloco> Blocos;

    // Vetor com todos os b's (ordenado e sem repetições)
    vector<int> todos_b;

    lerBlocos(Blocos);
    ordenarBlocos(Blocos);
    comprimirB(Blocos, todos_b);
    ArvoreSegmentos arv((int)todos_b.size()); // Cria a árvore com m = número de b distintos (caixinhas); o vetor t terá 2m posições.

    
    vector<int> valor((int)Blocos.size()); // Vetor valor com o tamanho de Blocos (Melhor torre com o bloco k na base)
    vector<int> prox((int)Blocos.size(), -1); // Vetor prox com tamanho de Blocos (Bloco que fica em cima de k) Colocamos -1 para no começo = nenhum

    imprimirDebug(Blocos, todos_b);

    return 0;
}