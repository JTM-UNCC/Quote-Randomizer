#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_QUOTES 1000
#define MAX_LINE_LENGTH 1024
#define MAX_QUOTE_LENGTH 128
#define FILE_NAME "Quotes for Discord  - Sheet1.csv"

// Escapes a string to make it CSV-safe (wraps in quotes, doubles inner quotes)
void csv_escape(const char *input, char *output, size_t max_len) {
  size_t j = 0;
  output[j++] = '"';
  for (size_t i = 0; input[i] != '\0' && j < max_len - 3; ++i) {
    if (input[i] == '"') {
      if (j + 2 >= max_len - 1)
        break; // prevent overflow
      output[j++] = '"';
      output[j++] = '"';
    } else {
      output[j++] = input[i];
    }
  }
  output[j++] = '"';
  output[j] = '\0';
}

void display_random_quote() {
  FILE *file = fopen(FILE_NAME, "r");
  if (!file) {
    perror("Error opening file");
    return;
  }

  char *quotes[MAX_QUOTES];
  int count = 0;
  char line[MAX_LINE_LENGTH];

  while (fgets(line, sizeof(line), file) && count < MAX_QUOTES) {
    line[strcspn(line, "\r\n")] = 0;
    quotes[count] = malloc(strlen(line) + 1);
    if (quotes[count]) {
      strcpy(quotes[count], line);
      count++;
    }
  }

  fclose(file);

  if (count == 0) {
    printf("No quotes found.\n");
    return;
  }

  srand((unsigned int)time(NULL));
  int index = rand() % count;
  printf("\nRandom Quote:\n%s\n", quotes[index]);

  for (int i = 0; i < count; i++) {
    free(quotes[i]);
  }
}

void add_quote() {
  char quote_input[MAX_QUOTE_LENGTH + 2]; // +2 for newline and null terminator
  char source_input[MAX_LINE_LENGTH];
  char escaped_quote[MAX_LINE_LENGTH];
  char escaped_source[MAX_LINE_LENGTH];

  getchar(); // Clear leftover newline

  printf("Enter the quote (max %d characters):\n> ", MAX_QUOTE_LENGTH);
  fgets(quote_input, sizeof(quote_input), stdin);
  quote_input[strcspn(quote_input, "\r\n")] = 0;

  if (strlen(quote_input) > MAX_QUOTE_LENGTH) {
    printf("Quote exceeds %d characters. Please shorten it.\n",
           MAX_QUOTE_LENGTH);
    return;
  }

  printf("Where is the quote from?\n> ");
  fgets(source_input, sizeof(source_input), stdin);
  source_input[strcspn(source_input, "\r\n")] = 0;

  csv_escape(quote_input, escaped_quote, sizeof(escaped_quote));
  csv_escape(source_input, escaped_source, sizeof(escaped_source));

  FILE *file = fopen(FILE_NAME, "a");
  if (!file) {
    perror("Error opening file for writing");
    return;
  }

  // Write 8 empty columns, then the source in the 9th column
  fprintf(file, "%s,,,,,,,,%s\n", escaped_quote, escaped_source);
  fclose(file);
  printf("Quote added successfully.\n");
}

int main() {
  int choice;

  while (0 == 0) {
    printf(" ");
    printf("Quote Manager\n");
    printf("---------------\n");
    printf("1. Display a random quote\n");
    printf("2. Add a new quote\n");
    printf("3. End program\n");
    printf("Enter your choice (1, 2, or 3): ");
    printf(" ");
    if (scanf("%d", &choice) != 1) {
      printf("Invalid input.\n");
      return 1;
    }

    if (choice == 1) {
      display_random_quote();
    } else if (choice == 2) {
      add_quote();
    } else if (choice == 3) {
      printf("Program ended.\n");
      break;
    } else {
      printf("Invalid choice.\n");
    }
  }
  return 0;
}
