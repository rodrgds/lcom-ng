#include "table.h"

person_info persons[MAX_PERSONS];
int person_count = 0;
char to_save[MAX_LENGTH];

int read_file() {
  FILE *file = fopen("/home/lcom/labs/proj/src/view/data.txt", "r");
  if (file == NULL) {
    perror("Error opening the file\n");
    return 1;
  }

  person_info p;
  while (fscanf(file, "%49[^;];%hhu/%hhu/%hhu %hhu:%hhu;%d\n", p.name,
                &p.time.ano, &p.time.mes, &p.time.dia,
                &p.time.hora, &p.time.minuta, &p.money) == 7) {
    if (person_count < MAX_PERSONS) {
      persons[person_count++] = p;
    }
    else break;
  }

  fclose(file);
  return 0;
}

int delete_game(uint8_t index) {
    if(strcmp(persons[index].name, p1.name) != 0){
        printf("its the different user\n");
        return 1;
    }

    for (int i = index; i < person_count; i++) {
        persons[i] = persons[i + 1];
    }
    person_count--;
    printf("was deleted\n");
    return 0;
}

void print_person_to_file(FILE *file, const person_info *p) {
    fprintf(file, "%s;%02hhu/%02hhu/%02hhu %02hhu:%02hhu;%d\n", p->name,
            p->time.ano, p->time.mes, p->time.dia,
            p->time.hora, p->time.minuta, p->money);
}

int save_file() {
    FILE *file = fopen(PATH_TO_DATA, "w");
    if (file == NULL) {
        perror("Error opening the file\n");
        return 1;
    }

    for (int i = 0; i < person_count; i++) {
        print_person_to_file(file, &persons[i]);
    }

    fclose(file);
    person_count = 0;
    return 0;
}

int add_game() {

    person_info result;

    int i = 0, len = 0;
    while (to_save[i] != '\0' && isl(to_save[i])) {
        result.name[len++] = to_save[i++];
    }
    result.name[len] = '\0';
    
    sscanf(to_save + i, "%2hhu%2hhu%2hhu%2hhu%2hhu",
           &result.time.ano, &result.time.mes, &result.time.dia,
           &result.time.hora, &result.time.minuta);
    i += 10;

    sscanf(to_save + i, "%d", &result.money);

    //printf("name: %s\n money: %u\n", result.name, result.money);

    if (person_count < MAX_PERSONS && !is_past(data, result.time))  {
        persons[person_count++] = result;
    }
    return 0;
}


