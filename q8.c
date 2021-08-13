#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <math.h>
#include <unistd.h> 
//#define NUM_THREAD 4

//lembrar de nao considerar 2 e 3 --> contador já começa com 2 valores preeenchidos
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;
//Qntd de primos começa em 2 já que 2 e 3 são primos;
int qntd_primos=2,num=3,meta;
int flag2=0;

//essa função seta a flag2 para 1, interrompendo o while que envia numeros para serem testados
//e da pthread_exit para liberar a thread que o encontrou
void Resultado(){
    //printf("QNTD: %d\n",qntd_primos);
    printf("Numero primo: %d\n",num);
    flag2=1;
    pthread_exit(NULL);
}

//Função que unicamente testa se os numeros são primos
//a flag marca que o numero não é primo. O mutex garante que a thread tenha exclusividade na função enquanto procura o resultado pedido
void Descobrir_primo(int tid){
//    printf("Entrei descobrir\n");
    pthread_mutex_lock(&mutex2);
    int i,flag=0;
 
    for (i = 2; i <= num / 2; i++) {
        if (num % i == 0) {
            flag=1;
            break;
        }
    }
    if(flag==0){
        qntd_primos++;
        //printf("Qntd:%d ID:%d\n",qntd_primos,tid);
    }
    if(qntd_primos==meta){
        //printf("AUAU\n");
        Resultado();
    }
    pthread_mutex_unlock(&mutex2);
}


// A função utilizando o mutex em conjunto com o sleep busca garantir que as threads so rodem enquanto o resultado nao foi encontrado
//garantindo que os numeros testados sigam a ordem e so sejam testados uma vez. 
void *Prox_numero(void *threadid){
    while(!flag2){
        //printf("Entrei prox\n");
        int tid = *((int*)threadid);
        pthread_mutex_lock(&mutex);
        num+=2;        
        //printf("Sai Prox\n");
        Descobrir_primo(tid);
        pthread_mutex_unlock(&mutex);
        sleep(0);
    }
    pthread_exit(NULL);
}


int main(){
    int NUM_THREAD;
    printf("Escreva a quantidade de Threads: ");
    scanf("%d",&NUM_THREAD);
    printf("\nEscreva o N: ");
    scanf("%d",&meta);
    pthread_t thread[NUM_THREAD];int i,j;

    //criando o threadid somente para conseguir identificar se todas estavam rodando
    int *threadid[NUM_THREAD]; 
    for(i=0;i<NUM_THREAD;i++){
        threadid[i]= (int*) malloc(sizeof(int));
        *threadid[i] = i;
        pthread_create(&thread[i],NULL,Prox_numero,(void*)threadid[i]);
    }
    for(i=0;i<NUM_THREAD;i++){
        pthread_join(thread[i],NULL);
    }
    
 
    pthread_exit(NULL);
}