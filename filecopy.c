
#include "filecopy.h"

char *buffer;
FILE *from;
FILE *to;
sem_t sem_write;
sem_t sem_read;

int main(int argc, char **argv) {
    pthread_t producer_thread;
    pthread_t consumer_thread;

    if (argc != 3) {
        printf("use %s <from> <to>\n", argv[0]);
        return 1;
    }

    if ((from = fopen(argv[1], "rb")) == NULL) {
        printf("cannot read %s\n", argv[1]);
        return -2;
    }

    if ((to = fopen(argv[2], "wb")) == NULL ) {
        printf("cannot write to %s\n", argv[2]);
        return -3;
    }

    buffer = malloc(sizeof(char) * RING_SIZE);

    sem_init(&sem_write, 0, RING_SIZE - 1);
    sem_init(&sem_read, 0, 0);

    pthread_create(&producer_thread, NULL, producer, NULL);
    pthread_create(&consumer_thread, NULL, consumer, NULL);

    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    if (fclose(from) == EOF) {
        printf("Error closing source file.\n");
        exit(1);
    }

    if (fclose(to) == EOF) {
        printf("Error closing destination file.\n");
        exit(1);
    }

    return 0;
}

void* producer(void *args) {
    int pos = 0;
    while (1) {
        sem_wait(&sem_write);
        buffer[pos] = fgetc(from);
        if (feof(from)) {
            break;
        }
        #ifdef DEBUG
        printf("put %c to %d\n", buffer[pos], pos);
        #endif
        sem_post(&sem_read);
        pos++;
        pos %= RING_SIZE;
    }
    return NULL;
}
void* consumer(void *args) {
    int pos = 0;
    while (1) {
        int value;
        sem_getvalue(&sem_read, &value);
        if (feof(from) && value == 0) {
            break;
        }
        sem_wait(&sem_read);
        #ifdef DEBUG
        printf("get %c to %d\n", buffer[pos], pos);
        #endif
        fputc(buffer[pos], to);
        sem_post(&sem_write);
        pos++;
        pos %= RING_SIZE;
    }
    return NULL;
}


