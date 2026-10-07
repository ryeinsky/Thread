#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* hitung_fungsi(void* arg)
{
    double x = 4.5;
    double hasil = (x * x) + (5 * x) + 6;

    pthread_mutex_lock(&mutex);

    printf("=========================================================\n");
    printf("[Thread 1] Berapakah hasil dari x^2 + 5x + 6 jika x = 4.5\n");
    printf("[Thread 1] Hasil f(%.1f) = %.2f\n", x, hasil);

    pthread_mutex_unlock(&mutex);

    return NULL;
}


void* tampilkan_fibonacci(void* arg)
{
    int n = 10;
    int t1 = 0;
    int t2 = 1;
    int nextTerm;

    pthread_mutex_lock(&mutex);

    printf("=========================================================\n");
    printf("[Thread 2] 10 Bilangan Pertama Fibonacci: ");

    for (int i = 1; i <= n; i++)
    {
        printf("%d ", t1);

        nextTerm = t1 + t2;
        t1 = t2;
        t2 = nextTerm;
    }

    printf("\n");
    pthread_mutex_unlock(&mutex);

    return NULL;
}


/* =========================================================
   THREAD 3
   Membaca teks dari file "catatan.txt"
   ========================================================= */
void* baca_teks(void* arg)
{
    FILE *file;
    int ch;

    pthread_mutex_lock(&mutex);

    printf("=========================================================\n");
    printf("[Thread 3] Memulai pembacaan file...\n");

    file = fopen("catatan.txt", "r");

    if (file == NULL)
    {
        printf("[Thread 3] File 'catatan.txt' tidak ditemukan.\n");
        printf("[Thread 3] Membuat file contoh...\n");
        printf("=========================================================\n");

        file = fopen("catatan.txt", "w+");

        if (file == NULL)
        {
            printf("[Thread 3] Gagal membuka atau membuat file.\n");

            pthread_mutex_unlock(&mutex);
            return NULL;
        }

        fprintf(
            file,
            "Halo! Ini adalah isi teks dari thread ketiga."
        );

        rewind(file);
    }

    printf("[Thread 3] Isi teks: ");

    while ((ch = fgetc(file)) != EOF)
    {
        putchar(ch);
    }

    printf("\n");

    fclose(file);

    pthread_mutex_unlock(&mutex);

    return NULL;
}


int main()
{
    pthread_t thread1;
    pthread_t thread2;
    pthread_t thread3;

    if (pthread_create(&thread1, NULL, hitung_fungsi, NULL) != 0)
    {
        perror("Gagal membuat Thread 1");
        pthread_mutex_destroy(&mutex);
        return 1;
    }


    if (pthread_create(&thread2, NULL, tampilkan_fibonacci, NULL) != 0)
    {
        perror("Gagal membuat Thread 2");

        pthread_join(thread1, NULL);
        pthread_mutex_destroy(&mutex);

        return 1;
    }


    if (pthread_create(&thread3, NULL, baca_teks, NULL) != 0)
    {
        perror("Gagal membuat Thread 3");

        pthread_join(thread1, NULL);
        pthread_join(thread2, NULL);

        pthread_mutex_destroy(&mutex);

        return 1;
    }

    
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);


    pthread_mutex_destroy(&mutex);

    printf("\nSemua tugas thread selesai.\n");
    printf("Program utama ditutup.\n");

    return 0;
}