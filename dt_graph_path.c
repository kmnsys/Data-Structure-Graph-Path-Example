#include <stdio.h>
#include <stdlib.h>

#define V 7
#define initial 0
#define visited 1

struct n {
    int index;
    int dist;
    struct n * next;
};

typedef struct n node;
node *heads[V];//köşeler için linked list arrayi

void addEdge(node *head, int dest, int dist) {//kenar ekleme
    node *iter = head;
    while (iter->next != NULL) {//köşenin komşu listesinde sona git
        iter = iter->next;
    }
    iter->next = (node*)malloc(sizeof(node));
    iter->next->next = NULL;
    iter->next->index = dest;//indeks atama
    iter->next->dist= dist;//uzaklık atama
}

void printEdges() {//köşe-kenar ilişkilerini yazdırma
    for (int i = 0; i <V ; i++) {
        node *iter = heads[i];
        printf("%d-->", iter->index);
        while (iter->next !=NULL) {
            printf("%d-%d,", iter->next->index, iter->next->dist);
            iter = iter->next;
        }
        printf("\n");
    }
}

void createGraph() {
    for (int i = 0; i<V ; ++i) {//heads arrayine köşeleri ekleme
        heads[i] = (node*)malloc(sizeof(node));
        heads[i]->index = i;
        heads[i]->next = NULL;
    }

    //kenar indeks ve uzaklık bilgilerini
    //heads arrayindeki linked listlere göre ekleme işlemleri
    addEdge(heads[0],1,1845);
    addEdge(heads[0],5,1264);
    addEdge(heads[1],3,7815);
    addEdge(heads[2],3,1303);
    addEdge(heads[2],5,8132);
    addEdge(heads[2],6,11550);
    addEdge(heads[3],4,5782);
    addEdge(heads[3],6,10838);
    addEdge(heads[4],2,4616);
    addEdge(heads[5],3,9566);
    addEdge(heads[6],5,5567);

    printEdges();
}

int pop(node* root) {//yığından en üstteki elemanı silme
    if(root == NULL) {
        printf("stack is empty");
        return -1;
    }

    if(root->next == NULL) {
        int rvalue = root->index;
        free(root);
        return rvalue;
    }

    node* iter = root;
    while (iter->next->next != NULL) {
        iter = iter->next;
    }

    node* temp  = iter->next;
    int rvalue = temp->index;
    iter->next = NULL;
    free(temp);
    return rvalue;
}

node* push(node *root,int a) {//yığına eleman push etme
    if(root == NULL) {//Stack boşsa
        root= (node*)malloc(sizeof(node));
        root->index = a;
        root->next = NULL;
        return root;
    }
    // stack boş değilse
    node* iter = root;
    while (iter->next !=NULL) {//stack in son elemanını bul
        iter = iter->next;
    }
    //temp değişkenine veriyi atayıp tempi stacke push et
    node* temp = (node*)malloc(sizeof(node));
    temp->index = a;
    temp->next = NULL;
    iter->next = temp;
    return root;
}

node* top(node *stack) {//yığında en üstteki elemanı bulma
    node *iter = stack;
    while (iter->next!=NULL) {
        iter = iter->next;
    }
    return iter;
}

int isAllVisited(int state[]) {//tüm köşeler ziyaret edildiyse 1 dön
    for (int i = 0; i <V ; ++i) {
        if(state[i] == initial)
            return 0;
    }
    return 1;
}

node* findPath(int src, int dest) {//Yol bulma fonksiyonu
    node *stack = NULL;
    stack = push(stack, src);//kaynağı yığına ekleme

    if (src == dest) {//kaynak ile hedef aynı ise yığını dön
        return stack;
    }

    int state[V];//Köşelerin ziyaret edilme arrayi
    for (int i = 0; i <V ; ++i) {
        state[i] = initial;
    }
    state[src] = visited;

    int tempSrc = src;//kaynağı gösteren geçici değişken
    while (!isAllVisited(state)) {//tüm köşeler ziyaret edilmediği sürece döngü çalışsın
        node *iter = heads[src];//şuanki bulunan indeksi gösteren head pointerı
        while (state[iter->next->index] == visited) {//ziyaret edilmeyen komşu köşeyi bulma
            if(iter->next->next==NULL)
                break;
            else
                iter = iter->next;
        }

        if(state[iter->next->index]==initial){//komşu köşe ziyaret edilmemişse
            src = iter->next->index; //aktif bulunulan köşeyi güncelle
            stack = push(stack, src); //bulunulan köşeyi yığına ekle
            state[src] = visited; //eski köşeyi ziyaret edilmiş işaretle

        }
        else{//komşu köşe ziyaret edilmişse yığının tepesindeki elemanı sil ve geri dön
            pop(stack);
            src=top(stack)->index;
        }
        //varış yeri bulunmuşsa ya da olası tüm ihtimaller denenmişse döngüyü sonlandır
        if (src == dest || (src == tempSrc && iter->next->next == NULL))
            return stack;
    }

    return stack;
}

int totalDist(node* path) {//Yolu gösteren yığın içinde gezerek toplam uzaklığı bulma
    node* iter = path;
    int dist = 0;
    while (iter->next!=NULL) {
        node* iter2 = heads[iter->index];
        while (iter2->next->index != iter->next->index) {
            iter2 = iter2->next;
        }
        dist = dist + iter2->next->dist;
        iter = iter->next;
    }
    return dist;
}

void printPath(node* path, int src, int dest, char cities[7][8]) {// Yolu ve uzaklığı yazdırma
    int distance = 0;
    if(path->next == NULL) {//path yığınında sadece kaynak varsa
        if(src == dest)
            printf("Source And Destination Are Same\nTotal distance is %d", distance);
        else
            printf("There Is No Path");
    }
    else {
        printf("Path Found: ");
        node *iter = path;
        while (iter!= NULL) {//yazdırma işlemleri
            for (int i = 0; i<8; ++i) {
                if (cities[iter->index][i]!='\0')
                    printf("%c", cities[iter->index][i]);
            }
            printf(", ");
            iter = iter->next;
        }
        distance = totalDist(path);//Fonksiyonu ile toplam yolu hesaplama
        printf("\nTotal distance is %d", distance);
    }
}

int main() {
    createGraph();//Graf köşeleri ve kenarları oluşturma fonksiyonu

    char cities[7][8] = {"Prague", "Helsinki", "Beijing", "Tokyo", "Jakarta", "London", "New York"};//İndeksere göre şehirler

    int src, dest;//kaynak ve hedef için değişkenler
    printf("\n\nCities of the graph:");
    printf("\nPrague = 0\nHelsinki = 1\nBeijing = 2\nTokyo = 3\nJakarta = 4\nLondon = 5\nNew York = 6\n\n");
    printf("1 - Enter the number of the first city: ");
    scanf("%d", &src);
    printf("2 - Enter the number of the destination city: ");
    scanf("%d", &dest);
    printf("\n");

    //Kullanıcıdan alınan değişkenler geçerliyse yolu bul ve yazdır
    if(src>=0 && src<=6 && dest>=0 && dest<=6) {
        node *path = findPath(src, dest);
        printPath(path,src, dest,cities);
    }
    else
        printf("Unvalid Inputs! TRY AGAIN\n");

    return 0;
}
