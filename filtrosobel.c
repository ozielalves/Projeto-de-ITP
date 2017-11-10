#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
typedef struct{
    int r, g, b;
}pixel;

typedef struct{
    pixel **pixelimagem;//ponteiro que aponta para os pixels da imagem
    char codigo[2];
    int max, largura, altura;
}imagem, *pont_imagem;//ponteiro que aponta para imagem

void criarmatriz(pont_imagem Imagem){ /*Prepara os pixels da nova imagem recebendo a altura e largura da imagem original*/
    int i;
    Imagem->pixelimagem = (pixel**)malloc(Imagem->altura * sizeof(pixel*));
    for(i=0; i<Imagem->altura; i++){
        Imagem->pixelimagem[i] = (pixel*)malloc(Imagem->largura * sizeof(pixel));
    }
}
void filtrosobel(pont_imagem Imagem){
	int x=0, y=0, sobel,auxx,auxy,i, j, k, l;
	int sobely[3][3] = {{ -1, 0, 1},{ -2, 0, 2},{-1,0,1}};
	int sobelx[3][3] = {{1, 2, 1},{0, 0, 0},{-1, -2, -1}};
	for(i=1; i<Imagem->altura-1; i++){				/*Aqui deve-se retirar as bordas da imagem para nao existir indices negativos*/
		for(j=1; j<Imagem->largura-1; j++){
			x=0;
			y=0;
			for(k=0; k<3; k++){
				for(l=0; l<3; l++){	
					x += (Imagem->pixelimagem[i-1+k][j-1+l].r * sobelx[k][l]);
					y += (Imagem->pixelimagem[i-1+k][j-1+l].r * sobely[k][l]);

				}
			}
			auxx=x/4;	
			auxy=y/4;
					sobel = (sqrt(pow(auxx, 2) + pow(auxy, 2)));
					if(sobel > 255){
					sobel=255;
					}
				Imagem->pixelimagem[i][j].r =  sobel;
				Imagem->pixelimagem[i][j].g =  sobel;
				Imagem->pixelimagem[i][j].b =  sobel;
		}
	}

}
void filtrogaussiano(pont_imagem Imagem){
	int i, j, k, l;
	int aux;
	int filtro[5][5] = {{ 2,  4,  5,  4, 2 },{ 4,  9, 12,  9, 4 },{ 5, 12, 15, 12, 5 },{ 4,  9, 12,  9, 4 },{ 2,  4,  5,  4, 2 }};
	for(i = 0; i < Imagem->altura; i++){
		for(j = 0; j < Imagem->largura; j++){
			aux = 0;
			for(k = 0; k < 5; k++){
				for(l = 0; l < 5; l++){
			       if ( k+2+i < Imagem->altura && k-2+i >= 0 && j-2+l >= 0 && j+2+k < Imagem->largura ) {
						aux += Imagem->pixelimagem[i-2+k][j-2+l].r * filtro[k][l]/159;
					}
				}
			}
			Imagem->pixelimagem[i][j].r = aux;
			Imagem->pixelimagem[i][j].g = aux;
			Imagem->pixelimagem[i][j].b = aux;
		}
	}
}
void lerimagem(pont_imagem Imagem){/*Lê a imagem do usuário*/
    FILE *imagem;// ponteiro que aponta pro arquivo
    int i, j;
    char nomeimagem[250];

    printf("Digite o nome da imagem: ");
    scanf("%s", nomeimagem);
    imagem=fopen(nomeimagem, "r");// abre em modo leitura
    if(imagem == NULL){
        printf("Houve um erro ao abrir a imagem\n");
        exit(1);
    }
    fscanf(imagem,"%s",Imagem->codigo);
    if(strcmp(Imagem->codigo,"P3")!=0){ // Verifica se é PPM
        printf("Imagem nao eh PPM\n");
        exit(1);
    }
    fscanf(imagem,"%i",&Imagem->largura);// Largura da imagem
    fscanf(imagem,"%i",&Imagem->altura);// Altura da imagem
    criarmatriz(Imagem);
    fscanf(imagem,"%i",&Imagem->max);

    for(i=0; i<Imagem->altura; i++){
        for(j=0; j<Imagem->largura; j++){
            fscanf(imagem,"%i",&Imagem->pixelimagem[i][j].r);/*aqui se lê todos os pixels da imagem e armazena em r=red, g=gree,b=blue*/
            fscanf(imagem,"%i",&Imagem->pixelimagem[i][j].g);
            fscanf(imagem,"%i",&Imagem->pixelimagem[i][j].b);
        }
    }
    fclose(imagem);
} 
void novaimagem(pont_imagem Imagem){/*Função para criar a imagem em tons de cinza*/
    FILE *imagem;
    int i, j;
    char novonome[250];
    printf("Digite o novo nome para imagem em cinza: ");
    scanf("%s", novonome);
    imagem=fopen(novonome,"w");// abre em modo escrita
    fprintf(imagem,"%s\n",Imagem->codigo);/*escreve o cabeçalho da imagem*/
    fprintf(imagem,"%i ",Imagem->largura);
    fprintf(imagem,"%i\n",Imagem->altura);
    fprintf(imagem,"%i\n",Imagem->max);
    for(i=0; i<Imagem->altura; i++){
        for(j=0; j<Imagem->largura; j++){
            fprintf(imagem,"%i ",Imagem->pixelimagem[i][j].r);/*Escreve os novos pixels, ja alterados na conversão*/
            fprintf(imagem,"%i ",Imagem->pixelimagem[i][j].g);
            fprintf(imagem,"%i\n",Imagem->pixelimagem[i][j].b);
        }
    }
    fclose(imagem);
    free(Imagem->pixelimagem);//libera o espaço
} 
void transformarcinza(pont_imagem Imagem){
    int i, j;
    int combinacao;
    for(i=0; i<Imagem->altura; i++){
        for(j=0; j<Imagem->largura; j++){
            combinacao=(Imagem->pixelimagem[i][j].r*0.3 + Imagem->pixelimagem[i][j].g*0.59 + Imagem->pixelimagem[i][j].b*0.11);/*Formula para conversao de cinza dada no pdf*/
            Imagem->pixelimagem[i][j].r = combinacao;
            Imagem->pixelimagem[i][j].g = combinacao;
            Imagem->pixelimagem[i][j].b = combinacao;
        }
    }
}
int main()
{   
    imagem Imagem;
    lerimagem(&Imagem);
    transformarcinza(&Imagem);
    filtrogaussiano(&Imagem);
    filtrosobel(&Imagem);
    novaimagem(&Imagem);
  return 0;
}