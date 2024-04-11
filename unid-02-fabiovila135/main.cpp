#include <iostream>
#include <algorithm>
using namespace std;

const int MAX = 255;

enum valores{
  UNDEFINED, GRAY, ENLARGE, REDUCE, ROTATE, SHARP, BLUR 
};

struct Pixel
{
  int R, G, B;
};

struct Imagem
{
  string formato;
  int col, lin, vmax;
  Pixel pixel[MAX][MAX];
};

Imagem receber_valores(); // atribui valores à imagem original.
valores checar_operacao(string operation);
void checar_pixel(int &linha, int &coluna, int maxlin, int maxcol); // checa se o valor digitado está entre 0 e o valor máximo
void checar_valores(int &valor_novo);
void gray(Imagem img, Imagem &new_img);
void enlarge(Imagem img, Imagem &new_img);
void reduce(Imagem img, Imagem &new_img);
void rotate(Imagem img, Imagem &new_img);
void sharp(Imagem img, Imagem &new_img);
void blur(Imagem img, Imagem &new_img);
void imprimir(Imagem new_img);

void print_usage(string program_name) {
  /**
 * Imprime uma ajuda explicando como o programa deve ser usado.
 * @param program_name Nome do arquivo executável.
 */
  cerr << "Uso correto:" << endl;
  cerr << '\t' << program_name << " <operação>" << endl;
  cerr << "Em que <operação> pode ser:" << endl;
  cerr << '\t' << "gray: para criar uma versão em escala de cinza da imagem original." << endl;
  cerr << '\t' << "enlarge: para aumentar o tamanho da imagem em 2x." << endl;
  cerr << '\t' << "reduce: para diminuir o tamanho da imagem em 2x." << endl;
  cerr << '\t' << "rotate: para rotacionar a imagem em 90 graus no sentido horário." << endl;
  cerr << '\t' << "sharp: para aumentar a nitidez da imagem." << endl;
  cerr << '\t' << "blur: para aumentar o desfoque da imagem." << endl;
  cerr << endl;
  cerr << "A imagem original é lida da entrada padrão e transformada";
  cerr << "a imagem é enviada para a saída padrão." << endl;
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    print_usage(argv[0]);
  }
  else {
    string operation = argv[1];
  
    Imagem img = receber_valores(); // recebe os valores de RGB de cada pixel da imagem
    Imagem new_img; // declara a imagem nova que será preenchida por cada função

    int num = checar_operacao(operation);
    switch (num) // chama a função desejada
    {
    case 0:
      exit(1); // se a função digitada não for igual a nenhuma das outras, fecha o programa e apresenta erro 1
      break;

    case 1:
      gray(img, new_img);
      break;
    
    case 2:
      enlarge(img, new_img);
      break;
    
    case 3:
      reduce(img, new_img);
      break;  
    
    case 4:
      rotate(img, new_img);
      break;
    
    case 5:
      sharp(img, new_img);
      break;
    
    case 6: 
      blur(img, new_img);
      break;
    default:
      break;
    
    }

    imprimir(new_img); // imprime a nova imagem
    return 0;
  }
}

Imagem receber_valores() { 
  Imagem img;
  cin >> img.formato >> img.col >> img.lin >> img.vmax;

  for (int i = 0; i < img.lin; ++i) {
    for (int j = 0; j < img.col; ++j) {
      cin >> img.pixel[i][j].R;
      cin >> img.pixel[i][j].G;
      cin >> img.pixel[i][j].B;
    }
  }
  return img;
}

valores checar_operacao(string operation) {
  if (operation == "gray") {
    return GRAY;
  }
  else if (operation == "enlarge") {
    return ENLARGE;
  }
  else if (operation == "reduce") {
    return REDUCE;
  }
  else if (operation == "rotate") {
    return ROTATE;
  }
  else if (operation == "sharp") {
    return SHARP;
  }
  else if (operation == "blur") {
    return BLUR;
  }
  else {
    return UNDEFINED;
  }
}

void checar_pixel(int &linha, int &coluna, int maxlin, int maxcol){ 
  if (linha < 0) {
    linha = 0;
  }
  
  if (linha > maxlin) {
    linha = maxlin;
  }

  if (coluna < 0) {
    coluna = 0;
  }

  if (coluna > maxcol) {
    coluna = maxcol;
  }
}

void checar_valores(int &valor_novo) {
  if (valor_novo < 0) {
    valor_novo = 0;
  }

  if (valor_novo > 255) {
    valor_novo = 255;
  }
}

void gray(Imagem img, Imagem &new_img) {
  new_img.formato = img.formato;
  new_img.col = img.col;
  new_img.lin = img.lin;
  new_img.vmax = img.vmax;

  int media = 0;

  for (int i = 0; i < img.lin; ++i) {
    for (int j = 0; j < img.col; ++j) {
      media = ((img.pixel[i][j].R + img.pixel[i][j].G + img.pixel[i][j].B)/3); // faz a média aritmetica dos valores de RGB, gerando uma tonalidade cinza para todos eles
      new_img.pixel[i][j].R = media;
      new_img.pixel[i][j].G = media; 
      new_img.pixel[i][j].B = media; 
    }
  }
}

void enlarge(Imagem img, Imagem &new_img) {
  new_img.formato = img.formato;
  new_img.col = (2 * img.col - 1);
  new_img.lin = (2 * img.lin - 1);
  new_img.vmax = img.vmax;

  // primeiro coloca os valores de RGB da imagem original na imagem nova
  for (int i = 0; i < img.lin; ++i) { 
    for (int j = 0; j < img.col; ++j) {
      new_img.pixel[2*i][2*j].R = img.pixel[i][j].R;
      new_img.pixel[2*i][2*j].G = img.pixel[i][j].G;
      new_img.pixel[2*i][2*j].B = img.pixel[i][j].B;
    }
  }
  
  int mr, mg, mb;
  //depois preenche que possuem adjacentes horizontais com a média de seus valores RGB 
  for (int i = 0; i < new_img.lin; ++i) { 
    for (int j = 0; j < new_img.col; ++j) {
      if ((i % 2 == 0) && (j % 2 != 0)) {
        mr = ((new_img.pixel[i][j-1].R + new_img.pixel[i][j+1].R)/2);
        mg = ((new_img.pixel[i][j-1].G + new_img.pixel[i][j+1].G)/2);
        mb = ((new_img.pixel[i][j-1].B + new_img.pixel[i][j+1].B)/2);

        new_img.pixel[i][j].R = mr;
        new_img.pixel[i][j].G = mg;
        new_img.pixel[i][j].B = mb;
      }
    }
  }

  //por fim, preenche os pixels que possuem adjacentes verticais com a média de seus valores de RGB
  for (int i = 0; i < new_img.lin; ++i) {
    for (int j = 0; j < new_img.col; ++j) {
      if (i % 2 != 0) {
        mr = ((new_img.pixel[i-1][j].R + new_img.pixel[i+1][j].R)/2);
        mg = ((new_img.pixel[i-1][j].G + new_img.pixel[i+1][j].G)/2);
        mb = ((new_img.pixel[i-1][j].B + new_img.pixel[i+1][j].B)/2);

        new_img.pixel[i][j].R = mr;
        new_img.pixel[i][j].G = mg;
        new_img.pixel[i][j].B = mb; 
      }
    }
  }
}

void reduce(Imagem img, Imagem &new_img) {
  if (img.lin % 2 != 0) {
    img.lin = img.lin - 1; 
  }

  if (img.col % 2 != 0) {
    img.col = img.col - 1;
  }
  // talvez possa dispensar essas duas condicionais acima
  new_img.formato = img.formato; // a imagem reduzida possui mesmo formato da imagem original
  new_img.col = (img.col / 2); // a imagem reduzida possui metade da altura da imagem original
  new_img.lin = (img.lin / 2); // a imagem reduzida possui metade da largura da imagem original
  new_img.vmax = img.vmax; // a imagem reduzida possui mesmo valor máximo que a original

  int mrr = 0, mrg = 0, mrb = 0;
  for (int i = 0; i < img.lin; i = i + 2) {
    for (int j = 0; j < img.col; j = j + 2) {
      // faz a média dos valores de RGB dos pixels em bloco de 4, os quais constituirão um só pixel na imagem nova.
      mrr = ((img.pixel[i][j].R + img.pixel[i+1][j].R + img.pixel[i][j+1].R + img.pixel[i+1][j+1].R)/4);
      mrg = ((img.pixel[i][j].G + img.pixel[i+1][j].G + img.pixel[i][j+1].G + img.pixel[i+1][j+1].G)/4);
      mrb = ((img.pixel[i][j].B + img.pixel[i+1][j].B + img.pixel[i][j+1].B + img.pixel[i+1][j+1].B)/4);

      new_img.pixel[i/2][j/2].R = mrr;
      new_img.pixel[i/2][j/2].G = mrg;
      new_img.pixel[i/2][j/2].B = mrb;
    }
  }
}

void rotate(Imagem img, Imagem &new_img) {
  new_img.formato = img.formato; // a imagem nova possui mesmo formato da imagem original
  new_img.lin = img.col; // a largura da imagem original se torna a altura da imagem nova
  new_img.col = img.lin; // a altura da imagem original se torna a largura da imagem nova
  new_img.vmax = img.vmax;  // o valor máximo da imagem nova continua o mesmo da imagem original
  for (int j = 0; j < img.col; ++j) { 
    for (int i = 0; i < img.lin; ++i) { // passando primeiro pelas colunas e depois pelas linhas, atribui os valores a imagem nova
      new_img.pixel[j][i].R = img.pixel[img.lin - i - 1][j].R;
      new_img.pixel[j][i].G = img.pixel[img.lin - i - 1][j].G;
      new_img.pixel[j][i].B = img.pixel[img.lin - i - 1][j].B;
    }
  }
}

void sharp(Imagem img, Imagem &new_img) {
  new_img.formato = img.formato;
  new_img.lin = img.lin;
  new_img.col = img.col;
  new_img.vmax = img.vmax;
  
double filtrosharp[3][3] = {
  {0, -1, 0},
  {-1, 5, -1},
  {0, -1, 0}
};

  for (int i = 0; i < new_img.lin; ++i) {
    for (int j = 0; j < new_img.col; ++j) {
      int newr = 0, newb = 0, newg = 0;
      for (int l = -1; l < 2; ++l) {
        for (int c = -1; c < 2; ++c) {
          int linha = i + l;
          int coluna = j + c;
          int maxlin = new_img.lin - 1;
          int maxcol = new_img.col - 1;

          checar_pixel(linha, coluna, maxlin, maxcol); //Se a máscara ultrapassar os limites da imagem, use o pixel da borda.
         
          newr += (img.pixel[linha][coluna].R * filtrosharp[l+1][c+1]);
          newg += (img.pixel[linha][coluna].G * filtrosharp[l+1][c+1]);
          newb += (img.pixel[linha][coluna].B * filtrosharp[l+1][c+1]);
        }
      }
      checar_valores(newr);
      checar_valores(newb);
      checar_valores(newg);
      new_img.pixel[i][j].R = newr;
      new_img.pixel[i][j].G = newg; 
      new_img.pixel[i][j].B = newb; 
    }
  }
}

void blur(Imagem img, Imagem &new_img) {
  new_img.formato = img.formato;
  new_img.lin = img.lin;
  new_img.col = img.col;
  new_img.vmax = img.vmax;
  
  double filtroblur[3][3] = {
  {1/9., 1/9., 1/9.},
  {1/9., 1/9., 1/9.},
  {1/9., 1/9., 1/9.}
};

  for (int i = 0; i < new_img.lin; ++i) {
    for (int j = 0; j < new_img.col; ++j) {
      double newr = 0, newb = 0, newg = 0;
      for (int l = -1; l < 2; ++l) {
        for (int c = -1; c < 2; ++c) {
          int linha = i + l;
          int coluna = j + c;
          int maxlin = new_img.lin - 1;
          int maxcol = new_img.col - 1;

          checar_pixel(linha, coluna, maxlin, maxcol); //Se a máscara ultrapassar os limites da imagem, use o pixel da borda.
         
          newr += (img.pixel[linha][coluna].R * filtroblur[l+1][c+1]);
          newg += (img.pixel[linha][coluna].G * filtroblur[l+1][c+1]);
          newb += (img.pixel[linha][coluna].B * filtroblur[l+1][c+1]);
        }
      }
      int nr = newr;
      int ng = newg;
      int nb = newb;
      checar_valores(nr);
      checar_valores(nb);
      checar_valores(ng);
      new_img.pixel[i][j].R = nr;
      new_img.pixel[i][j].G = ng; 
      new_img.pixel[i][j].B = nb; 
    }
  }
}

void imprimir(Imagem new_img) {
  cout << new_img.formato << endl << new_img.col << " " << new_img.lin << endl << new_img.vmax << endl;
  // a linha acima imprime o cabeçalho da imagem, a linha abaixo imprime os valores de RGB da imagem.
  for (int i = 0; i < new_img.lin; ++i) {
    for (int j = 0; j < new_img.col; ++j) {
      int r = new_img.pixel[i][j].R;
      int g = new_img.pixel[i][j].G;
      int b = new_img.pixel[i][j].B;

      cout << r << " " << g << " " << b << " "; 
    }
    cout << endl;
  }
}

struct Ponto
{
  int localx, localy;
};

struct Linha
{
  Ponto pontoum, pontodois; 
};

struct Poligono
{
  int pontos_max;
  Ponto pontos_usados[500];
};