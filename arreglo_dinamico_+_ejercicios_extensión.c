#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *datos;
    size_t size;
    size_t capacity;
} ArregloDinamico;

/* Redimensionar el arreglo */
int arreglo_redimensionar(ArregloDinamico *arr, size_t nueva_capacidad) {
    int *tmp = realloc(arr->datos, nueva_capacidad * sizeof(int));

    if (tmp == NULL)
        return 0;

    arr->datos = tmp;
    arr->capacity = nueva_capacidad;
    return 1;
}

/* Inicializar */
void arreglo_iniciar(ArregloDinamico *arr, size_t capacidad_inicial) {
    arr->datos = malloc(capacidad_inicial * sizeof(int));

    if (arr->datos == NULL) {
        fprintf(stderr, "Error: malloc no pudo reservar memoria.\n");
        exit(EXIT_FAILURE);
    }

    arr->size = 0;
    arr->capacity = capacidad_inicial;

    printf("[INFO] Arreglo iniciado con capacidad para %zu elementos.\n",
           capacidad_inicial);
}

/* Agregar al final */
void arreglo_agregar(ArregloDinamico *arr, int valor) {
    if (arr->size == arr->capacity) {
        size_t nueva = arr->capacity * 2;

        printf("[INFO] Capacidad agotada (%zu/%zu). Solicitando realloc a %zu elementos...\n",
               arr->size, arr->capacity, nueva);

        if (!arreglo_redimensionar(arr, nueva)) {
            fprintf(stderr, "Error: realloc no pudo ampliar la memoria.\n");
            free(arr->datos);
            exit(EXIT_FAILURE);
        }

        printf("[INFO] Realloc exitoso. Nueva capacidad: %zu elementos.\n",
               arr->capacity);
    }

    arr->datos[arr->size++] = valor;
}

/*insertar */
void arreglo_insertar(ArregloDinamico *arr, size_t indice, int valor) {
    if (indice > arr->size) {
        fprintf(stderr, "Error: índice de inserción fuera de rango.\n");
        return;
    }

    if (arr->size == arr->capacity) {
        size_t nueva = arr->capacity * 2;

        printf("[INFO] Capacidad agotada. Ampliando a %zu elementos...\n",
               nueva);

        if (!arreglo_redimensionar(arr, nueva)) {
            fprintf(stderr, "Error: realloc no pudo ampliar la memoria.\n");
            free(arr->datos);
            exit(EXIT_FAILURE);
        }

        printf("[INFO] Realloc exitoso. Nueva capacidad: %zu elementos.\n",
               arr->capacity);
    }

    for (size_t i = arr->size; i > indice; i--)
        arr->datos[i] = arr->datos[i - 1];

    arr->datos[indice] = valor;
    arr->size++;

    printf("[INFO] Elemento %d insertado en la posición %zu.\n",
           valor, indice);
}

/* eliminar + shrink-to-fit */
void arreglo_eliminar(ArregloDinamico *arr, size_t indice) {
    if (indice >= arr->size) {
        fprintf(stderr, "Error: índice de eliminación fuera de rango.\n");
        return;
    }

    int eliminado = arr->datos[indice];

    for (size_t i = indice; i < arr->size - 1; i++)
        arr->datos[i] = arr->datos[i + 1];

    arr->size--;

    printf("[INFO] Elemento %d eliminado de la posición %zu.\n",
           eliminado, indice);

    if (arr->capacity > 2 && arr->size < arr->capacity / 4) {
        size_t nueva = arr->capacity / 2;

        if (nueva < 2)
            nueva = 2;

        if (nueva < arr->size)
            nueva = arr->size;

        printf("[INFO] Poco espacio utilizado (%zu/%zu). Solicitando realloc para reducir a %zu elementos...\n",
               arr->size, arr->capacity, nueva);

        if (arreglo_redimensionar(arr, nueva))
            printf("[INFO] Reducción exitosa. Nueva capacidad: %zu elementos.\n",
                   arr->capacity);
        else
            printf("[INFO] No se pudo reducir la capacidad. Se conserva la capacidad actual: %zu elementos.\n",
                   arr->capacity);
    }
}

/*buscar */
int arreglo_buscar(const ArregloDinamico *arr, int valor) {
    for (size_t i = 0; i < arr->size; i++) {
        if (arr->datos[i] == valor)
            return (int)i;
    }

    return -1;
}

/* Mostrar */
void arreglo_mostrar(const ArregloDinamico *arr) {
    printf("[");

    for (size_t i = 0; i < arr->size; i++) {
        printf("%d", arr->datos[i]);
        if (i < arr->size - 1)
            printf(", ");
    }

    printf("]\n");
}

/* Liberar */
void arreglo_liberar(ArregloDinamico *arr) {
    free(arr->datos);
    arr->datos = NULL;
    arr->size = 0;
    arr->capacity = 0;

    printf("[INFO] Memoria liberada correctamente.\n");
}

int main(void) {
    ArregloDinamico arr;

    arreglo_iniciar(&arr, 2);

    for (int i = 1; i <= 10; i++)
        arreglo_agregar(&arr, i * 10);

    printf("\nArreglo inicial:\n");
    arreglo_mostrar(&arr);

    printf("\n--- Prueba de inserción ---\n");
    arreglo_insertar(&arr, 2, 999);

    printf("Arreglo después de insertar 999 en posición 2:\n");
    arreglo_mostrar(&arr);

    printf("\n--- Prueba de búsqueda ---\n");

    int valor = 999;
    int posicion = arreglo_buscar(&arr, valor);

    if (posicion != -1)
        printf("El valor %d se encuentra en la posición %d.\n",
               valor, posicion);
    else
        printf("El valor %d no se encuentra en el arreglo.\n", valor);

    printf("\n--- Prueba de eliminación ---\n");
    arreglo_eliminar(&arr, 2);

    printf("Arreglo después de eliminar la posición 2:\n");
    arreglo_mostrar(&arr);

    printf("\n--- Prueba de reducción de capacidad ---\n");

    for (int i = 0; i < 7; i++)
        arreglo_eliminar(&arr, 0);

    printf("Arreglo después de varias eliminaciones:\n");
    arreglo_mostrar(&arr);

    printf("Tamaño actual: %zu elementos\n", arr.size);
    printf("Capacidad actual: %zu elementos\n", arr.capacity);

    arreglo_liberar(&arr);

    return 0;
}   



