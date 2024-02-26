/*
 Run-length encoding
 Use for encoding:
   ./rle -e input.txt output.rle

 Use for decoding:
   ./rle -d input.rle decoded
*/

#include <cstring>
#include <fstream>
#include <iostream>
void StringPush(int &quantity, int character, char *string);

void encoding(std::ifstream &input, std::ofstream &output);

void decoded(std::ifstream &input, std::ofstream &output);

void StringToOutput(int &quantity, int character, char *string,
                    std::ofstream &output);

void CharacterToOutput(int &quantity, int character, std::ofstream &output);

int main(int argc, char *argv[]) {
  std::ifstream input;
  input.open(argv[2], std::ios_base::binary);
  std::ofstream output;
  output.open(argv[3], std::ios_base::binary);
  if (strcmp(argv[1], "-d") == 0) {
    decoded(input, output);
  }
  if (strcmp(argv[1], "-e") == 0) {
    encoding(input, output);
  }
  input.close();
  output.close();
  return 0;
}

void decoded(std::ifstream &input, std::ofstream &output) {
  int quantity;
  while ((quantity = input.get()) != EOF) {
    if (quantity < 128) {
      int character = input.get();
      for (int i = 0; i <= quantity; ++i) {
        output.put(static_cast<char>(character));
      }
    } else {
      quantity -= 256;
      quantity *= -1;
      for (int i = 0; i < quantity; ++i) {
        output.put(static_cast<char>(input.get()));
      }
    }
  }
}

void encoding(std::ifstream &input, std::ofstream &output) {
  char string[128];
  int quantity = 0;
  int character = 0;
  while (input.peek() != EOF) {
    character = input.get();
    if (quantity == -128 || character == input.peek()) {
      if (quantity < 0) {
        StringToOutput(quantity, character, string, output);
      } else {
        ++quantity;
      }
    } else {
      if (quantity <= 0) {
        StringPush(quantity, character, string);
      } else {
        CharacterToOutput(quantity, character, output);
      }
    }
  }
  if (quantity < 0) {
    StringToOutput(quantity, character, string, output);
  } else if (quantity > 0) {
    --quantity;
    CharacterToOutput(quantity, character, output);
  }
}

void StringToOutput(int &quantity, int character, char *string,
                    std::ofstream &output) {
  output.put(static_cast<signed char>(quantity));
  for (int i = 0; i < -quantity; ++i) {
    output.put(static_cast<char>(string[i]));
  }
  if (quantity == -128) {
    string[0] = static_cast<char>(character);
    quantity = -1;
  } else {
    quantity = 1;
  }
}
void StringPush(int &quantity, int character, char *string) {
  string[-quantity] = static_cast<char>(character);
  --quantity;
}

void CharacterToOutput(int &quantity, int character, std::ofstream &output) {
  bool done=false;
  do {
    if (quantity >= 128) {
      output.put(static_cast<signed char>(127));
      output.put(static_cast<char>(character));
      quantity -= 128;
    } else {
      output.put(static_cast<signed char>(quantity));
      output.put(static_cast<char>(character));
      quantity = 0;
      done=true;
    }
  } while (!done);
}