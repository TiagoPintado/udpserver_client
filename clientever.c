#include <zmq.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define max_buffer 1024 

int main (void)
{
    void *context = zmq_ctx_new ();
    void *requester = zmq_socket (context, ZMQ_REQ);
    zmq_connect (requester, "udp://localhost:5555");
    
    char buffer[max_buffer] = {0}; //buffer=0
    int num_arrays, max_size;

    printf("Digite o numero de vetores: ");
    scanf("%d", &num_arrays);

    printf("Digite o tamanho maximo dos vetores: ");
    scanf("%d", &max_size);

    for (int i = 0; i < num_arrays; i++) {
        int vetor[max_size], num_elementos = 0, num;

        printf("Digite os numeros para preencher o vetor %d: ", i + 1);
        while (scanf("%d", &num) == 1 && num_elementos < max_size) {
            vetor[num_elementos] = num;
            num_elementos++;
        }

        char vetor_size[100];
        sprintf(vetor_size, "%d", num_elementos); //converte tamanho do vetor para string
        strcat(buffer, vetor_size);
        strcat(buffer, ","); //adiciona virgula após tamanho do vetor

        for (int j = 0; j < num_elementos; j++) { //adiciona cada valor do vetor na string
            char vetor_new[100];
            sprintf(vetor_new, "%d", vetor[j]);
            strcat(buffer, vetor_new);
            strcat(buffer, ",");
        }
    }

    zmq_send(requester, buffer, strlen(buffer), 0);

    char response[max_buffer] = {0}; //inicializa resposta com 0
    zmq_recv(requester, response, max_buffer, 0);

    char *token = strtok(response, ","); //imprimir cada valor da string recebida
    while (token != NULL) {
        printf("%s\n", token);
        token = strtok(NULL, ",");
    }

    zmq_close(requester);
    zmq_ctx_destroy(context);

    return 0;
}
