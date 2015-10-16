/* 
 * File:   main.c
 * Author: Antonio
 *
 * Created on 16 de octubre de 2015, 18:10
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

/*
 * 
 */
void main() {

    pid_t pid, Hijo_pid, pid2, Hijo2_pid;
    int fd1[2];
    int fd2[2];

    char saludoAbuelo[] = "Saludos del abuelo.\0";
    char saludoPadre[] = "Saludos del Padre.\0";
    char saludoHijo[] = "Saludos del hijo.\0";
    char saludoNieto[] = "Saludos del nieto.\0";

    char buffer[80] = "";

    pipe(fd1);
    pipe(fd2);

    pid = fork(); //el abuelo acaba de crear a su hijo

    if (pid == -1) {
        printf("Error al crear al hijo del abuelo.");
        exit(-1);
    }
    if (pid == 0) { //estamos en el codigo que ejecutara el hijo del abuelo
        pid2 = fork(); //el hijo del abuelo crea al nieto del abuelo
        switch (pid2) {
            case -1:
                printf("No se ha podido crear al nieto.");
                exit(-1);
                break;

            case 0: //nos encontramos en el codigo que ejecutara el proceso nieto
                //el nieto va a recibir
                close(fd2[1]); //cierra el descriptor de entrada
                read(fd2[0], buffer, sizeof (buffer)); //se lee el pipe
                printf("\t\tNieto ha recibido mensaje de su padre : %s\n", buffer);

                //el nieto va a enviar un mensaje
                printf("\t\tNieto envia un mensaje a su padre...\n");
                close(fd1[0]);
                write(fd1[1], saludoNieto, strlen(saludoNieto));
                break;

            default: //estamos en el proceso hijo del abuelo
                //recibe de su padre osea el abuelo
                close(fd1[1]);
                read(fd1[0], buffer, sizeof (buffer));
                printf("\tEl del medio recibe de su padre : %s\n", buffer);

                //envia a su hijo osea al nieto
                close(fd2[0]);
                write(fd2[1], saludoPadre, strlen(saludoPadre));
                Hijo2_pid = wait(NULL);

                //recibe de su hijo osea del nieto
                close(fd1[1]);
                read(fd1[0], buffer, sizeof (buffer));
                printf("\tEl del medio recibe un mensaje de su hijo: %s\n", buffer);

                //envia un mensaje a su padre osea el abuelo
                printf("\tEl del medio envía un mensaje a su padre\n");
                close(fd2[0]);
                write(fd2[1], saludoHijo, strlen(saludoHijo));


        }

    } else { //estamos en el proceso abuelo
        //abuelo envia un mensaje a su hijo
        printf("Abuelo envía un mensaje a su hijo...\n");
        close(fd1[0]);
        write(fd1[1], saludoAbuelo, strlen(saludoAbuelo));
        Hijo_pid = wait(NULL);

        //abuelo recibe de su hijo
        close(fd2[1]);
        read(fd2[0], buffer, sizeof (buffer));
        printf("Abuelo recime mensaje de su hijo: %s\n", buffer);

    }
    exit(0);
}

