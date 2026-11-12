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

// Ela vai consultar o vetor Blocos, alterar a Arvore (Altera o valor e o próximo do nó)
void CalcularTorres(const vector<Bloco> &Blocos, ArvoreSegmentos &arv, vector<int> &valor, vector<int> &prox){

    // Será tratado de grupos em grupos (Grupo a = 0 ... grupo a = x)
    
    int n = (int)Blocos.size(); // Número de Blocos
    int i = 0; // Onde começa o grupo atual


    // Cada volta desse ciclo trata um grupo inteiro
    // While exterior
    while(i<n){ // Quando i chegar a N já se passaram todos os blocos

        // Encontrar o fim do grupo
        // Esse ciclo vai encontrar cada grupo de a
        int j = i;
        while(j < n && Blocos[j].a == Blocos[i].a ){
            j++;
        }
        // 1º passagem de cada grupo : Calcular o valor e prox de cada bloco do grupo (Apenas consultas)
        for(int k = i; k < j; k++){

            // Procuramos na arvore qual a melhor torre que cabe em cima do bloco k
            // Para isso precisamos saber em qual caixa da arvore o bloco está, isso está na pos_b
            pair<int,int> melhor = arv.consultar(Blocos[k].pos_b); 

            //O valor da melhor torre com este bloco na base é a altura dele + o valor da torre que fica em cima 
            // Somamos a altura do bloco + a altura da melhor torre que poderia ir em cima dele
            valor[k] = Blocos[k].h + melhor.first; // Cada <int,int> guarda o <altura da torre, indice do bloco (posição)> 

            //Agora guardamos o indice da torre que fica em cima
            prox[k] = melhor.second; // Second de melhor guarda o valor do indice do bloco
            // Se não tiver nada em cima ele vai guardar(0,-1) por tanto esse é o ultimo bloco (Nesse caso)
        }

        // 2º passagem: Inserir na árvore todos os blocos do grupo (Somente atualização)
        for(int k = i; k < j; k++){
            arv.atualizar(Blocos[k].pos_b, valor[k], k); // Na caixa do bloco k, escreve o valor[k], que veio do bloco k
        }

        i = j; // Atualizamos o i para o grupo seguinte, assim percorremos todos os blocos e conseguiremos catalogar todos os valores de a
    }


}

int EncontrarMelhor(const vector<int> &valor){

    int melhor = 0; // Indice(posição) do melhor até agora

    for(int k = 1; k < (int)valor.size(); k++){
        if(valor[k] > valor[melhor]){
            melhor = k;
        }
    }

    return melhor;
}

// Segue a cadeia de prox a partir da base e imprime a torre 
void ImprimirTorre(const vector<Bloco> &Blocos, const vector<int> &valor, const vector<int> &prox, int base){


    vector<int> torre;
    int k = base;
    while(k != -1){ // Quando prox = -1 quer dizer que não há mais um próximo bloco

        torre.push_back(k); // Adiciona o bloco de indice k na torre
        k = prox[k]; // O k salta para o bloco que está por cima
    }

    // Linha 1: Altura total da torre e número de blocos
    printf("%d %d\n", valor[base] , (int)torre.size());

    // Um bloco por linha, da base até o topo
    for(int i = 0; i < (int)torre.size(); i++){
        printf("%d %d %d\n", Blocos[torre[i]].a, Blocos[torre[i]].b, Blocos[torre[i]].h);
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

    CalcularTorres(Blocos,arv, valor, prox);
    
    int base = EncontrarMelhor(valor);

    ImprimirTorre(Blocos, valor, prox, base);

    return 0;
}