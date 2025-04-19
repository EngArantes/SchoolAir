#include "reader_file.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <dirent.h>

#define BUFFER 1024
void read_csv_file(const char *file_path) {
    FILE *file = fopen(file_path, "r");
    if (!file) {
        perror("Erro ao abrir o ficheiro CSV");
        return;
    }

    char line[BUFFER];
    printf("Conteúdo de '%s':\n", file_path);
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line); // já vem com \n embutido
    }
    printf("\n");

    fclose(file);
}


int list_csv_files(const char *dir_path, char ***file_list) {
    DIR *dir = opendir(dir_path);
    if (!dir) return -1;

    int capacity = 10;
    int count = 0;
    char **files = malloc(capacity * sizeof(char *));
    if (!files) return -1;

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {
            const char *name = entry->d_name;
            size_t name_len = strlen(name);

            if (name_len > 4 && strcmp(name + name_len - 4, ".csv") == 0) {
                if (count >= capacity) {
                    capacity *= 2;
                    char **tmp = realloc(files, capacity * sizeof(char *));
                    if (!tmp) {
                        closedir(dir);
                        return -1;
                    }
                    files = tmp;
                }

                // Monta caminho completo
                size_t path_len = strlen(dir_path) + strlen(name) + 2;
                char *full_path = malloc(path_len);
                snprintf(full_path, path_len, "%s/%s", dir_path, name);

                files[count++] = full_path;
            }
        }
    }

    closedir(dir);
    *file_list = files;
    return count;
}



int main_reader_file(const char *input_dir) {
    char **file_list = NULL;
    int file_count = list_csv_files(input_dir, &file_list);

    if (file_count < 0) {
        perror("Erro ao listar ficheiros");
        return -1;
    }

    for (int i = 0; i < file_count; i++) {
        read_csv_file(file_list[i]);  // 👈 ler conteúdo aqui
        free(file_list[i]);
    }
    free(file_list);

    return 0;
}
