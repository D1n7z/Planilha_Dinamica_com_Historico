/* ============================================================================
    EP1 - Planilha Esparsa com Histórico de Alterações
    TEMPLATE - preencha os TODOs abaixo. Não altere assinaturas de função,
    nomes ou ordem de campos de struct.
 
    Uso: ./ep_XXXX arquivo_entrada.txt arquivo_saida.txt
 ============================================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* ----------------------------------------------------------------------
  Estruturas de dados (não altere nomes e ordem de campos)
 ---------------------------------------------------------------------- */

typedef struct celula {
    int linha;
    int coluna;
    int valor;
    struct celula *proxima_linha;
    struct celula *proxima_coluna;
} celula_t;

typedef struct fileira {
    int indice;
    celula_t* primeiro;
    struct fileira *proximo;
} fileira_t;

typedef struct {
    bool transposicao;
    int linha;
    int coluna;
    union {
        int valor_anterior;
        int tamanho;
    };
} operacao_t;

typedef struct elo_pilha {
    operacao_t op;
    struct elo_pilha *proximo;
} elo_pilha_t;

typedef struct {
    elo_pilha_t* topo;
} pilha_t;

typedef struct {
    fileira_t* primeira_linha;
    fileira_t* primeira_coluna;
    int total_celulas;
    pilha_t historico;
} planilha_t;

/* ---------------------------------------------------------------------- *
    Funções auxiliar para leitura das strings dos comandos
 ---------------------------------------------------------------------- */

int igual(char* a, char* b) {
    int i = 0;
    while (a[i] == b[i] && a[i] != '\0') i++;

    return (a[i] == b[i]);
}

/* ---------------------------------------------------------------------- *
    Funções obrigatórias (não altere as assinaturas)
 ---------------------------------------------------------------------- */

void inicializar_planilha(planilha_t *p) {
    // TODO: inicialize primeiraLinha, primeiraColuna, total_celulas e historico.topo
    p->primeira_linha = NULL;
    p->primeira_coluna = NULL;
    p->total_celulas = 0;
    p->historico.topo = NULL;
}

celula_t* buscar_celula(planilha_t *p, int lin, int col,
                      celula_t** cel_ant_linha, celula_t** cel_ant_coluna,
                      fileira_t** fil_ant_linha, fileira_t** fil_ant_coluna) {
    /* TODO: localize a celula (lin,col), preencha os quatro antecessores 
        por referencia, retorne NULL se a celula nao existir */
    if(lin < 0 || col < 0){
        return NULL;
    }

    *fil_ant_linha = NULL;
    *fil_ant_coluna = NULL;
    *cel_ant_linha = NULL;
    *cel_ant_coluna = NULL;

    fileira_t* linha_atual = p->primeira_linha;
    fileira_t* coluna_atual = p->primeira_coluna;


    while(linha_atual != NULL && linha_atual->indice < lin){
        *fil_ant_linha = linha_atual;
        linha_atual = linha_atual->proximo;
    }

    while(coluna_atual != NULL && coluna_atual->indice < col){
        *fil_ant_coluna = coluna_atual;
        coluna_atual = coluna_atual->proximo;
    }

    celula_t* celula_linha_atual = NULL;
    if (linha_atual != NULL && linha_atual->indice == lin) {
        celula_linha_atual = linha_atual->primeiro;

         while(celula_linha_atual != NULL && celula_linha_atual->coluna < col){
            *cel_ant_linha = celula_linha_atual;
            celula_linha_atual = celula_linha_atual->proxima_linha;
        }
    } 

    celula_t* celula_coluna_atual = NULL;
    if (coluna_atual != NULL && coluna_atual->indice == col) {
        celula_coluna_atual = coluna_atual->primeiro;
        while(celula_coluna_atual != NULL && celula_coluna_atual->linha < lin){
            *cel_ant_coluna= celula_coluna_atual;
            celula_coluna_atual = celula_coluna_atual->proxima_coluna;
        }
    } 

    if(celula_linha_atual != NULL && celula_linha_atual->coluna == col){         
        return celula_linha_atual;     
    }  
    

    return NULL;
}

int obter_valor(planilha_t *p, int linha, int coluna) {
    // TODO: use buscarCelula; retorne 0 se a celula nao existir

    celula_t* cel_ant_linha = NULL;
    celula_t* cel_ant_coluna = NULL;
    fileira_t* fil_ant_linha = NULL;
    fileira_t* fil_ant_coluna = NULL;

    celula_t* celula = buscar_celula(p, linha, coluna, &cel_ant_linha, &cel_ant_coluna, &fil_ant_linha, &fil_ant_coluna);
    if (celula != NULL) {
        return celula->valor;
    }

    return 0;
}

int somar_intervalo(planilha_t* p, int linha_ini, int linha_fim, int coluna_ini, int coluna_fim) {
    // TODO: some os valores das celulas nao nulas no intervalo dado

    if(linha_ini > linha_fim || coluna_ini > coluna_fim || linha_ini < 0 || coluna_ini < 0 || linha_fim < 0 || coluna_fim < 0){
        return 0;
    }

    int soma = 0;

    fileira_t* linha_atual = p->primeira_linha;
    while(linha_atual != NULL && linha_atual->indice < linha_ini){
        linha_atual = linha_atual->proximo;
    }

    celula_t* celula_atual = NULL;
    if(linha_atual != NULL){
        celula_atual = linha_atual->primeiro;
    }
    

    while((linha_atual != NULL && linha_atual->indice <= linha_fim)){
        while(celula_atual != NULL && celula_atual->coluna < coluna_ini){
            celula_atual = celula_atual->proxima_linha;
        }
        while(celula_atual != NULL && celula_atual->coluna <= coluna_fim){
            soma += celula_atual->valor;
            celula_atual = celula_atual->proxima_linha;
        }

        if(linha_atual->proximo != NULL && linha_atual->proximo->indice <= linha_fim){
            linha_atual = linha_atual->proximo;
            celula_atual = linha_atual->primeiro;
        } else {
            break;
        }
        
    }

    return soma;
}

int contar_nao_nulas(planilha_t* p) {
    // TODO: retorne a quantidade de celulas nao nulas
    return p->total_celulas;
}

bool auxiliar_definir_celula(planilha_t* p, int lin, int col, int valor, celula_t* cel_ant_linha, celula_t* cel_ant_coluna, fileira_t* linha_anterior, fileira_t* coluna_anterior, celula_t* celula_atual) {

    if(celula_atual){
        if(valor != 0){ //atualizar valor
            celula_atual->valor = valor;
            return true;
            
        } else { //remover celula
            fileira_t* linha_atual;
            fileira_t* coluna_atual;

            //identificar a linha e a coluna atual
            if(linha_anterior != NULL){
                linha_atual = linha_anterior->proximo;
            } else {
                linha_atual = p->primeira_linha;
            }

            if(coluna_anterior != NULL){
                coluna_atual = coluna_anterior->proximo;
            } else {
                coluna_atual = p->primeira_coluna;
            }

            //desconectar a celula da linha
            if(cel_ant_linha != NULL){
                cel_ant_linha->proxima_linha = celula_atual->proxima_linha;
            } else {
                linha_atual->primeiro = celula_atual->proxima_linha;
            }

            //desconectar a celula da coluna
            if(cel_ant_coluna != NULL){
                cel_ant_coluna->proxima_coluna = celula_atual->proxima_coluna;
            } else {
                coluna_atual->primeiro = celula_atual->proxima_coluna;
            }

            //verificar se a linha ficou vazia
            if(linha_atual->primeiro == NULL){
                if(linha_anterior != NULL){
                    linha_anterior->proximo = linha_atual->proximo;
                } else {
                    p->primeira_linha = linha_atual->proximo;
                }
                free(linha_atual);
            }

            //verificar se a coluna ficou vazia
            if(coluna_atual->primeiro == NULL){
                if(coluna_anterior != NULL){
                    coluna_anterior->proximo = coluna_atual->proximo;
                } else {    
                    p->primeira_coluna = coluna_atual->proximo;
                }
                free(coluna_atual);
            }

            free(celula_atual);
            p->total_celulas--;

            return true;
            
        }
            
    } else {
        if (valor != 0){ //criar celula
            celula_t* nova_celula = malloc(sizeof(celula_t));
            nova_celula->linha = lin;
            nova_celula->coluna = col;
            nova_celula->valor = valor;

            //criar e conectar a linha
            fileira_t* linha_atual;
            if(linha_anterior != NULL){
                linha_atual = linha_anterior->proximo;
            } else {
                linha_atual = p->primeira_linha;
            }

            if(linha_atual == NULL || linha_atual->indice != lin){
                fileira_t* nova_linha = malloc(sizeof(fileira_t));
                nova_linha->primeiro = NULL;
                nova_linha->indice = lin;

                //conectar a nova linha na lista de linhas
                nova_linha->proximo = linha_atual;
                if(linha_anterior != NULL){
                    linha_anterior->proximo = nova_linha;   
                } else {
                    p->primeira_linha = nova_linha;
                }

                linha_atual = nova_linha;
            }

            //criar e conectar a coluna
            fileira_t* coluna_atual;
            if(coluna_anterior != NULL){
                coluna_atual = coluna_anterior->proximo;
            } else {
                coluna_atual = p->primeira_coluna;
            } 

            if(coluna_atual == NULL || coluna_atual->indice != col){
                fileira_t* nova_coluna = malloc(sizeof(fileira_t));
                nova_coluna->primeiro = NULL;
                nova_coluna->indice = col;

                //conectar a nova coluna na lista de colunas
                nova_coluna->proximo = coluna_atual;
                if(coluna_anterior != NULL){
                    coluna_anterior->proximo = nova_coluna;   
                } else {
                    p->primeira_coluna = nova_coluna;
                }

                coluna_atual = nova_coluna;
            }

            //conectar a nova celula na linha
            if(cel_ant_linha != NULL){
                nova_celula->proxima_linha = cel_ant_linha->proxima_linha;
                cel_ant_linha->proxima_linha = nova_celula;
            } else {
                nova_celula->proxima_linha = linha_atual->primeiro;
                linha_atual->primeiro = nova_celula;
            }

            //conectar a nova celula na coluna
            if(cel_ant_coluna != NULL){
                nova_celula->proxima_coluna = cel_ant_coluna->proxima_coluna;
                cel_ant_coluna->proxima_coluna = nova_celula;
            } else {
                nova_celula->proxima_coluna = coluna_atual->primeiro;
                coluna_atual->primeiro = nova_celula;
            }

            p->total_celulas++;
            return true;

        } else { //operacao nula
            return false;
        }
    }     


    return false;
}

bool definir_celula(planilha_t* p, int lin, int col, int valor) {
    /* TODO: implemente os 4 casos (atualizar/criar/remover/nulo),
       empilhando em p->historico quando houver alteracao efetiva.
       Retorna true se houve alteracao, false se foi operacao nula. */
        
    
    celula_t* cel_ant_linha = NULL;
    celula_t* cel_ant_coluna = NULL;
    fileira_t* linha_anterior = NULL; 
    fileira_t* coluna_anterior = NULL;

    celula_t* celula_atual = buscar_celula(p, lin, col, &cel_ant_linha, &cel_ant_coluna, &linha_anterior, &coluna_anterior);

    if (celula_atual && celula_atual->valor == valor) { //operacao nula 
        return false;
    }
    
    if(celula_atual){
        //atualizar valor ou remover celula
            elo_pilha_t *elo_pilha = malloc(sizeof(elo_pilha_t));
            elo_pilha->op.linha = lin;
            elo_pilha->op.coluna = col;
            elo_pilha->op.transposicao = false;
            elo_pilha->op.valor_anterior = celula_atual->valor;
            elo_pilha->proximo = p->historico.topo;
            p->historico.topo = elo_pilha;

            if(auxiliar_definir_celula(p, lin, col, valor, cel_ant_linha, cel_ant_coluna, linha_anterior, coluna_anterior, celula_atual)){
                return true;
            } else{
                elo_pilha_t *temp = p->historico.topo;
                p->historico.topo = p->historico.topo->proximo;
                free(temp);
                return false;
            }
            
    } else {
        if (valor != 0){ //criar celula
            elo_pilha_t *elo_pilha = malloc(sizeof(elo_pilha_t));
            elo_pilha->op.linha = lin;
            elo_pilha->op.coluna = col;
            elo_pilha->op.transposicao = false;
            elo_pilha->op.valor_anterior = 0;
            elo_pilha->proximo = p->historico.topo;
            p->historico.topo = elo_pilha;

            if(auxiliar_definir_celula(p, lin, col, valor, cel_ant_linha, cel_ant_coluna, linha_anterior, coluna_anterior, celula_atual)){
                return true;
            } else{
                elo_pilha_t *temp = p->historico.topo;
                p->historico.topo = p->historico.topo->proximo;
                free(temp);
                return false;
            }
            
        } else { //operacao nula
            return false;
        }
    }     


    return false;
}

bool remover_celula(planilha_t* p, int lin, int col) {
    // TODO: remova a celula (lin,col)
    return definir_celula(p, lin, col, 0);
}

bool auxiliar_transpor(planilha_t* p, int lin, int col, int tamanho) {
    if (tamanho <= 1) {
        return false;
    }

    int *matriz = (int *) malloc(sizeof(int)*tamanho*tamanho);

    bool transposta = false;

    for(int i=0; i<tamanho; i++){
        for(int j=0; j<tamanho; j++){
            matriz[tamanho * j + i] = obter_valor(p, lin+i, col+j);
        }
    }

    for(int i=0; i<tamanho; i++){
        for(int j=0; j<tamanho; j++){
            celula_t* cel_ant_linha = NULL;
            celula_t* cel_ant_coluna = NULL;
            fileira_t* linha_anterior = NULL; 
            fileira_t* coluna_anterior = NULL;

            celula_t* celula_atual = buscar_celula(p, lin+i, col+j, &cel_ant_linha, &cel_ant_coluna, &linha_anterior, &coluna_anterior);
            if (celula_atual && celula_atual->valor != matriz[tamanho * i + j]) {
                transposta = true;
            } else if (!celula_atual && matriz[tamanho * i + j] != 0) {
                transposta = true;
            }
            auxiliar_definir_celula(p, lin+i, col+j, matriz[tamanho * i + j], cel_ant_linha, cel_ant_coluna, linha_anterior, coluna_anterior, celula_atual);
        }
    }

    free(matriz);
    
    return transposta;
}

bool transpor(planilha_t* p, int lin, int col, int tamanho) {
    /* TODO: transpoe uma matriz quadrada que está localizada entre
       as linhas [lin, lin + tamanho) e colunas [col, col + tamanho). */

    bool saida = auxiliar_transpor(p, lin, col, tamanho);

    if (!saida) {
        return false;
    }

    elo_pilha_t *elo_pilha = malloc(sizeof(elo_pilha_t));
    elo_pilha->op.linha = lin;
    elo_pilha->op.coluna = col;
    elo_pilha->op.transposicao = true;
    elo_pilha->op.tamanho = tamanho;

    elo_pilha->proximo = p->historico.topo;
    p->historico.topo = elo_pilha;

    return true;
}

bool desfazer(planilha_t* p) {
    /* TODO: desempilhe de p->historico e restaure o valor anterior.
       Retorna false se o historico estiver vazio, true caso contrario. */
    if(p->historico.topo != NULL){
        operacao_t* op_topo = &p->historico.topo->op;
        if(op_topo->transposicao){
            auxiliar_transpor(p, op_topo->linha, op_topo->coluna, op_topo->tamanho);
            elo_pilha_t* temp = p->historico.topo;
            p->historico.topo = temp->proximo;
            free(temp);
        } else{
            celula_t* cel_ant_linha = NULL;
            celula_t* cel_ant_coluna = NULL;
            fileira_t* linha_anterior = NULL; 
            fileira_t* coluna_anterior = NULL;


            celula_t* celula_atual = buscar_celula(p, op_topo->linha, op_topo->coluna, &cel_ant_linha, &cel_ant_coluna, &linha_anterior, &coluna_anterior);
            auxiliar_definir_celula(p, op_topo->linha, op_topo->coluna, op_topo->valor_anterior, cel_ant_linha, cel_ant_coluna, linha_anterior, coluna_anterior, celula_atual);
            elo_pilha_t* temp = p->historico.topo;
            p->historico.topo = temp->proximo;
            free(temp);
        }
        return true;
    }
    
    return false;
}

void exibir_planilha(planilha_t *p) {
    /* TODO: imprima "linha coluna valor" por linha, em ordem crescente
       de linha e, dentro de cada linha, de coluna. Se vazia, imprima
       "PLANILHA VAZIA" */
    fileira_t* linha_atual = p->primeira_linha;
    celula_t* celula_atual;

    while(linha_atual){
        celula_atual = linha_atual->primeiro;
        while(celula_atual){
            printf("%d %d %d\n", celula_atual->linha, celula_atual->coluna, celula_atual->valor);
            celula_atual = celula_atual->proxima_linha;
        }
        linha_atual = linha_atual->proximo;
    }
}

void exibir_historico(planilha_t* p) {
    /* TODO: imprima "linha coluna valor_anterior" por linha, do topo
       para a base. Se vazio, imprima "HISTORICO VAZIO" */
    elo_pilha_t* elo_atual = p->historico.topo;
    while(elo_atual){
        if(elo_atual->op.transposicao){
            printf("T %d %d %d\n", elo_atual->op.linha, elo_atual->op.coluna, elo_atual->op.tamanho);
        } else{
            printf("%d %d %d\n", elo_atual->op.linha, elo_atual->op.coluna, elo_atual->op.valor_anterior);
        }
        elo_atual = elo_atual->proximo;
    }   
}

void liberar_tudo(planilha_t* p) {
    // TODO: libere toda a memória alocada por fileira, celula, e pilha.
    fileira_t* linha_atual = p->primeira_linha;
    while(linha_atual){
        celula_t* celula_atual = linha_atual->primeiro;
        while(celula_atual){
            celula_t* temp_celula = celula_atual;
            celula_atual = celula_atual->proxima_linha;
            free(temp_celula);
        }
        fileira_t* temp_linha = linha_atual;
        linha_atual = linha_atual->proximo;
        free(temp_linha);
    }   

    fileira_t* coluna_atual = p->primeira_coluna;
    while(coluna_atual){
        fileira_t* temp_coluna = coluna_atual;
        coluna_atual = coluna_atual->proximo;
        free(temp_coluna);  
    }
    
    elo_pilha_t* elo_atual = p->historico.topo;
    while(elo_atual){
        elo_pilha_t* temp_elo = elo_atual;
        elo_atual = elo_atual->proximo;
        free(temp_elo);
    }

    inicializar_planilha(p);
}

/* ---------------------------------------------------------------------- *
    Main para leitura de arquivos (já pronta no caso, pode ser que na versão final deixemos sem)
 ---------------------------------------------------------------------- */

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Uso do comando eh: %s arquivo_entrada.txt arquivo_saida.txt\n", argv[0]);
        return 1;
    }
    
    FILE* entrada = fopen(argv[1], "r");
    FILE* saida = freopen(argv[2], "w", stdout);

    if (!entrada || !saida) {
        fprintf(stderr, "Erro ao tentar abrir os arquivos.\n");
        return 1;
    }

    planilha_t p;
    inicializar_planilha(&p);

    int n;
    fscanf(entrada, "%d", &n);

    char cmd[20];

    while (fscanf(entrada, "%s", cmd) != EOF) {
 
        if (igual(cmd, "DEF")) {
            int lin, col, valor;
            fscanf(entrada, "%d %d %d", &lin, &col, &valor);
            definir_celula(&p, lin, col, valor);
        } else if (igual(cmd, "REM")) {
            int lin, col;
            fscanf(entrada, "%d %d", &lin, &col);
            remover_celula(&p, lin, col);
        } else if (igual(cmd, "GET")) {
            int lin, col;
            fscanf(entrada, "%d %d", &lin, &col);
            fprintf(saida, "GET %d %d %d\n", lin, col, obter_valor(&p, lin, col));
        } else if (igual(cmd, "SOMA")) {
            int li, lf, ci, cf;
            fscanf(entrada, "%d %d %d %d", &li, &lf, &ci, &cf);
            fprintf(saida, "SOMA %d %d %d %d %d\n", li, lf, ci, cf, somar_intervalo(&p, li, lf, ci, cf));
        } else if (igual(cmd, "CONT")) {
            fprintf(saida, "CONT %d\n", contar_nao_nulas(&p));
        } else if (igual(cmd, "DESFAZER")) {
            if (!desfazer(&p)) {
                fprintf(saida, "HISTORICO VAZIO\n");
            }
        } else if (igual(cmd, "EXIBIR")) {
            if (contar_nao_nulas(&p)) {
                printf("PLANILHA\n");
                exibir_planilha(&p);
            } else {
                printf("PLANILHA VAZIA\n");
            }
        } else if (igual(cmd, "HIST")) {
            if (p.historico.topo) {
                printf("HISTORICO\n");
                exibir_historico(&p);
            }
            else {
                printf("HISTORICO VAZIO\n");
            }
        } else if (igual(cmd, "TRANS")) {
            int lin, col, tam;
            fscanf(entrada, "%d %d %d", &lin, &col, &tam);
            transpor(&p, lin, col, tam);
        }
    }

    fclose(entrada);
    fclose(saida);

    liberar_tudo(&p);
    return 0;
}
