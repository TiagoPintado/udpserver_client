#include <zmq.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
//localizacao pasta:
// cd ../mnt/c/Users/tiago/OneDrive\ -\ Universidade\ do\ Porto/Ambiente\ de\ Trabalho/prog/

#define max_buffer 1024
//compilar:
//gcc udp-server.c -lzmq -o server      
// ./a.out
int main (void)
{
    void *context = zmq_ctx_new ();
    void *responder = zmq_socket (context, ZMQ_REP);
    zmq_bind (responder, "tcp://*:5555");

    char buffer[max_buffer];
    zmq_recv(responder, buffer, max_buffer, 0); //recebe mensagem do servidor

    char response[max_buffer];
    int num_vetores = 0;
    int pos = 0;

    while (buffer[pos] != '\0') {
        char vetor_size_str[10] = {0};
        int vetor_size = 0;
        int num_elementos = 0;

        // extrai o tamanho do vetor
        while (buffer[pos] != ',' && buffer[pos] != '\0') {
            vetor_size_str[num_elementos] = buffer[pos];
            num_elementos++;
            pos++;
        }

        vetor_size = atoi(vetor_size_str);

        int vetor[vetor_size];

        // extrai os elementos do vetor
        for (int i = 0; i < vetor_size; i++) {
            char vetor_element_str[10] = {0};
            int vetor_element = 0;
            num_elementos = 0;

            while (buffer[pos] != ',' && buffer[pos] != '\0') {
                vetor_element_str[num_elementos] = buffer[pos];
                num_elementos++;
                pos++;
            }

            vetor_element = atoi(vetor_element_str);
            vetor[i] = vetor_element;
            pos++;
        }

        // processa o vetor e constrói a resposta
        for (int i = 0; i < vetor_size; i++) {
            sprintf(response + strlen(response), "%d,", vetor[vetor_size - i - 1]);
        }

        num_vetores++;
    }

    zmq_send(responder, response, strlen(response), 0); //envia resposta para o servidor

    zmq_close(responder);
    zmq_ctx_destroy(context);

    return 0;
}





