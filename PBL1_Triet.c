#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <float.h>
#include <math.h>
#include <stdarg.h>
#include <string.h>
#ifdef _WIN32
#include <direct.h>
#endif

typedef long long ll;

#define MAX_N 100

typedef struct{
    ll u,v;
    double w;
} Canh;

#define RESET   "\033[30;47m"
#define BLACK   "\033[30;47m"
#define RED     "\033[31;47m"
#define GREEN   "\033[32;47m"
#define YELLOW  "\033[33;47m"
#define BLUE    "\033[34;47m"
#define PURPLE  "\033[35;47m"
#define CYAN    "\033[36;47m"


static FILE *g_algo_out = NULL;
static int g_output_to_file = 0;
static char g_result_path[512] = {0};

int is_ansi_color_token(const char *fmt);
void algo_printf(const char *fmt, ...);
void algo_line();
void algo_print_double(double value);
void line();
void clear_screen();
void pause_screen();
void luu_duong_dan_result();
void bao_cao_da_ghi_file(void);

#define printf(...) algo_printf(__VA_ARGS__)

void header_big();
void menu_nhap();
void menu_xuat();
void menu_chuc_nang();

void clr(int n, double g[][MAX_N]);
void nhap_mt(int n, double g[][MAX_N]);
void tao_ngau_nhien(int n, double wmax, double g[][MAX_N]);
int doc_mt_tu_file(const char *ten_file, int *n, double g[][MAX_N]);
void xuat_mt(FILE *out, int n, double g[][MAX_N]);
void in_mt(int n, double g[][MAX_N]);
int lien_thong(int n, double g[][MAX_N]);
void dfs(int n, double g[][MAX_N], int u, int vis[]);
int goc(int par[], int x);
void hop(int par[], int rank[], int a, int b);
int cmp(const void *a, const void *b);

void cay_dfs(int n, double g[][MAX_N], int s);
void cay_bfs(int n, double g[][MAX_N], int s);
int la_euler(int n, double g[][MAX_N]);
void dijkstra(int n, double g[][MAX_N], int s, int t);
void prim(int n, double g[][MAX_N]);
void kruskal(int n, double g[][MAX_N]);

void thuc_hien_thuat_toan(int che_do_xuat, int n, double g[][MAX_N], int chon, int s, int t);
void chay_menu_chuc_nang(int n, double g[][MAX_N], int che_do_xuat);

int is_ansi_color_token(const char *fmt){
    return fmt == RED || fmt == GREEN || fmt == YELLOW || fmt == BLUE || fmt == PURPLE || fmt == CYAN || fmt == RESET;
}

void algo_printf(const char *fmt, ...){
    FILE *out = g_algo_out ? g_algo_out : stdout;
    if(out != stdout && is_ansi_color_token(fmt)){
        return;
    }
    va_list args;
    va_start(args, fmt);
    vfprintf(out, fmt, args);
    va_end(args);
}

void algo_line(){
    FILE *out = g_algo_out ? g_algo_out : stdout;
    fputs("============================================================\n", out);
}

void algo_print_double(double value){
    FILE *out = g_algo_out ? g_algo_out : stdout;
    if(isinf(value)){
        fputs("inf", out);
    }else{
        fprintf(out, "%.2f", value);
    }
}

void line(){
    algo_printf(CYAN);
    algo_line();
    algo_printf(RESET);
}

void clear_screen(){
    #ifdef _WIN64
        system("cls");
    #else
        system("clear");
    #endif
}

void pause_screen(){
    printf(YELLOW);
    printf("\nNhan ENTER de tiep tuc...");
    printf(RESET);
    getchar();
    getchar();
}

void luu_duong_dan_result(){
#ifdef _WIN32
    if(_getcwd(g_result_path, sizeof(g_result_path)) != NULL){
        strncat(g_result_path, "\\result.dat", sizeof(g_result_path) - strlen(g_result_path) - 1);
    }else{
        strncpy(g_result_path, "result.dat", sizeof(g_result_path) - 1);
    }
#else
    if(getcwd(g_result_path, sizeof(g_result_path)) != NULL){
        strncat(g_result_path, "/result.dat", sizeof(g_result_path) - strlen(g_result_path) - 1);
    }else{
        strncpy(g_result_path, "result.dat", sizeof(g_result_path) - 1);
    }
#endif
}

void bao_cao_da_ghi_file(void){
    luu_duong_dan_result();
    printf(GREEN);
    printf(">>> Da hoan thanh. Ket qua duoc ghi tai: %s\n", g_result_path);
    printf(RESET);
}

void header_big(){
    printf(BLUE);
    line();
    printf("        TRUONG DAI HOC BACH KHOA - DAI HOC DA NANG\n");
    printf("                   KHOA CONG NGHE THONG TIN\n");
    printf ("\n");
    printf ("\n");
    printf ("\n");
    printf(YELLOW);
    printf("                 PBL1: DO AN LAP TRINH TINH TOAN\n");
    printf("        CAI DAT VA UNG DUNG CAC THUAT TOAN TREN DO THI\n");
    printf ("\n");
    printf ("\n");
    printf(GREEN);
    printf("           Nguoi huong dan : ThS. Tran Ho Thuy Tien\n");
    printf(PURPLE);
    printf("           Sinh vien thuc hien  : Tran Dinh Hong Phat\n");
    printf("                                  Tran Minh Triet\n");
    printf(CYAN);
    printf("           Lop                  : 25T_DT2\n");
    printf("\n");
    printf("                       Da Nang 06/2026\n");
    line();
    printf(RESET);
}

void menu_nhap(){
    printf("\n");
    line();
    printf(YELLOW);
    printf(" %-3s | %-45s\n","STT","PHUONG THUC NHAP DU LIEU DO THI");
    printf(GREEN);
    printf(" %-3d | %-45s\n",1,"Nhap thu cong tu ban phim");
    printf(" %-3d | %-45s\n",2,"Doc du lieu tu file (.dat)");
    printf(" %-3d | %-45s\n",3,"Tao ma tran ngau nhien");
    printf(" %-3d | %-45s\n",0,"Ket thuc chuong trinh");
    line();
    printf(CYAN);
    printf("Nhap lua chon: ");
    printf(RESET);
}

void menu_xuat(){
    printf("\n");
    line();
    printf(YELLOW);
    printf(" %-3s | %-45s\n","STT","PHUONG THUC XUAT KET QUA");
    printf(GREEN);
    printf(" %-3d | %-45s\n",1,"In truc tiep len console");
    printf(" %-3d | %-45s\n",2,"Ghi vao file result.dat");
    printf(" %-3d | %-45s\n",0,"Quay lai menu nhap");
    line();
    printf(CYAN);
    printf("Nhap lua chon: ");
    printf(RESET);
}

void menu_chuc_nang(){
    printf("\n");
    line();
    printf(YELLOW);
    printf(" %-3s | %-45s\n","STT","CHUC NANG");
    printf(GREEN);
    printf(" %-3d | %-45s\n",1,"Cay phu DFS");
    printf(" %-3d | %-45s\n",2,"Cay phu BFS");
    printf(" %-3d | %-45s\n",3,"Kiem tra do thi Euler");
    printf(" %-3d | %-45s\n",4,"Dijkstra tim duong ngan nhat");
    printf(" %-3d | %-45s\n",5,"Prim tim cay khung nho nhat");
    printf(" %-3d | %-45s\n",6,"Kruskal tim cay khung nho nhat");
    printf(" %-3d | %-45s\n",0,"Quay lai menu ban dau");
    line();
    printf(CYAN);
    printf("Nhap lua chon: ");
    printf(RESET);
}

void clr(int n, double g[][MAX_N]){
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            g[i][j]=0.0;
        }
    }
}

void nhap_mt(int n, double g[][MAX_N]){
    clr(n,g);
    line();
    printf(YELLOW);
    printf("NHAP MA TRAN DOI XUNG\n");
    printf("\n");
    printf(RESET);
    for(int i=0;i<n;i++){
        for(int j=i+1;j<n;j++){
            double w;
            int nhap_dung = 0;

            while (!nhap_dung) {
                printf("a[%d][%d]= ", i+1, j+1);
                if (scanf("%lf", &w) != 1) {
                    printf(RED);
                    printf("Loi: Du lieu khong hop le! Vui long nhap mot so.\n");
                    printf(RESET);
                    int c;
                    while ((c = getchar()) != '\n' && c != EOF);
                } else {
                    nhap_dung = 1;
                }
            }

            if(w<0.0) w=0.0;
            g[i][j]=w;
            g[j][i]=w;
        }
    }
}

void tao_ngau_nhien(int n, double wmax, double g[][MAX_N]){
    clr(n,g);
    for(int i=1;i<n;i++){
        int p=rand()%i;
        double w=((double)rand() / RAND_MAX) * wmax;
        g[i][p]= w;
        g[p][i]= w;
    }
    for(int i=0;i<n;i++){
        for(int j=i+1;j<n;j++){
            if(g[i][j]==0.0){
                double w=((double)rand() / RAND_MAX) * wmax;
                g[i][j]= w;
                g[j][i]= w;
            }
        }
    }
}

int doc_mt_tu_file(const char *ten_file, int *n, double g[][MAX_N])
{
    FILE *fp = fopen(ten_file, "r");
    if (fp == NULL)
        return 0;

    if (fscanf(fp, "%d", n) != 1)
    {
        fclose(fp);
        return 0;
    }

    if (*n < 1 || *n > MAX_N)
    {
        fclose(fp);
        return 0;
    }

    for (int i = 0; i < *n; i++)
    {
        for (int j = 0; j < *n; j++)
        {
            if (fscanf(fp, "%lf", &g[i][j]) != 1)
            {
                fclose(fp);
                return 0;
            }
        }
    }

    fclose(fp);
    return 1;
}

void xuat_mt(FILE *out, int n, double g[][MAX_N]){
    fprintf(out, "                MA TRAN TRONG SO\n");
    fprintf(out, "%5s", " ");
    for(int i=0;i<n;i++){
        fprintf(out, "%5d", i+1);
    }
    fprintf(out, "\n");
    for(int i=0;i<n;i++){
        fprintf(out, "%5d", i+1);
        for(int j=0;j<n;j++){
            fprintf(out, "%8.2f", g[i][j]);
        }
        fprintf(out, "\n");
    }
    fprintf(out, "============================================================\n");
}

void in_mt(int n, double g[][MAX_N]){
    if(g_output_to_file && g_algo_out != NULL){
        xuat_mt(g_algo_out,n,g);
        return;
    }
    line();
    printf(YELLOW);
    printf("                MA TRAN TRONG SO\n");
    printf("\n");
    printf(RESET);
    printf("%3s"," ");
    for(int i=0;i<n;i++){
        printf("%7d",i+1);
    }
    printf("\n");
   for(int i=0;i<n;i++){
       printf("%3d",i+1);
       for(int j=0;j<n;j++){
           printf("%7.2f",g[i][j]);
       }
       printf("\n");
   }
    line();
}

int lien_thong(int n, double g[][MAX_N]){
    int *vis=calloc(n,sizeof(int));
    int *q=malloc(n*sizeof(int));
    int dau=0,cuoi=0;
    int dem=0;
    q[cuoi++]=0;
    vis[0]=1;

    while(dau<cuoi){
        int u=q[dau++];
        dem++;
        for(int i=0;i<n;i++){
            if(g[u][i]>0.0 && !vis[i]){
                vis[i]=1;
                q[cuoi++]=i;
            }
        }
    }
    free(vis);
    free(q);
    return dem==n;
}

void dfs(int n, double g[][MAX_N], int u, int vis[]){
    vis[u]=1;
    printf("Tham dinh %d\n",u+1);
    for(int v=0;v<n;v++){
        if(g[u][v]>0 && !vis[v]){
            printf("(%d) ----%.5f----> (%d)\n",u+1,g[u][v],v+1);
            dfs(n,g,v,vis);
        }
    }
}

int goc(int par[], int x){
    while(x!=par[x]){
        par[x]=par[par[x]];
        x=par[x];
    }
    return x;
}

void hop(int par[], int rank[], int a, int b){
    int ga=goc(par,a);
    int gb=goc(par,b);
    if(ga==gb) return;
    if(rank[ga]<rank[gb]){
        par[ga]=gb;
    }else if(rank[ga]>rank[gb]){
        par[gb]=ga;
    }else{
        par[gb]=ga;
        rank[ga]++;
    }
}

int cmp(const void *a, const void *b) {
    const Canh *x = (const Canh*)a;
    const Canh *y = (const Canh*)b;

    if (x->w < y->w) return -1;
    if (x->w > y->w) return 1;
    return 0;
}

void cay_dfs(int n, double g[][MAX_N], int s){
    int *vis=calloc(n,sizeof(int));
    int *cha=malloc(n*sizeof(int));
    Canh *ds=malloc((n-1)*sizeof(Canh));
    int dem=0;

    for(int i=0;i<n;i++){
        cha[i]=-1;
    }
    printf(YELLOW);
    printf("                 CAY PHU DFS\n");
    printf("\n");
    printf(RESET);
    printf("Bat dau DFS tu dinh %d\n",s+1);

    int stack[MAX_N];
    int top=0;
    stack[top++]=s;
    vis[s]=1;
    printf("Tham dinh %d\n",s+1);

    while(top>0){
        int u=stack[top-1];
        int tim_duoc=0;
        for(int v=0;v<n;v++){
            if(g[u][v]>0 && !vis[v]){
                vis[v]=1;
                cha[v]=u;
                ds[dem].u=u;
                ds[dem].v=v;
                ds[dem].w=g[u][v];
                dem++;
                printf("(%d) ----%.5f----> (%d)\n",u+1,g[u][v],v+1);
                printf("Tham dinh %d\n",v+1);
                stack[top++]=v;
                tim_duoc=1;
                break;
            }
        }
        if(!tim_duoc){
            top--;
        }
    }

    printf("\nCAC CANH TRONG CAY PHU DFS:\n");
    for(int i=0;i<dem;i++){
        printf("%lld -- %lld : %.5f\n",ds[i].u+1,ds[i].v+1,ds[i].w);
    }
    free(vis);
    free(cha);
    free(ds);
}

void cay_bfs(int n, double g[][MAX_N], int s){
    int *vis=calloc(n,sizeof(int));
    int *q=malloc(n*sizeof(int));
    int *cha=malloc(n*sizeof(int));
    Canh *ds=malloc((n-1)*sizeof(Canh));
    int dau=0,cuoi=0;
    int dem=0;

    for(int i=0;i<n;i++){
        cha[i]=-1;
    }
    vis[s]=1;
    q[cuoi++]=s;
    printf(YELLOW);
    printf("                 CAY PHU BFS\n");
    printf("\n");
    printf(RESET);
    printf("Bat dau BFS tu dinh %d\n",s+1);

    while(dau<cuoi){
        int u=q[dau++];
        printf("Lay dinh %d ra khoi hang doi\n",u+1);
        for(int i=0;i<n;i++){
            if(g[u][i]>0 && !vis[i]){
                vis[i]=1;
                cha[i]=u;
                ds[dem].u=u;
                ds[dem].v=i;
                ds[dem].w=g[u][i];
                dem++;
                printf("(%d) ----%.2f----> (%d)\n",u+1,g[u][i],i+1);
                printf("Phat hien dinh %d va dua vao hang doi\n",i+1);
                q[cuoi++]=i;
            }
        }
    }

    printf("\nCAC CANH TRONG CAY PHU BFS:\n");
    for(int i=0;i<dem;i++){
        printf("%lld -- %lld : %.2f\n",ds[i].u+1,ds[i].v+1,ds[i].w);
    }
    free(vis);
    free(q);
    free(cha);
    free(ds);
}

int la_euler(int n, double g[][MAX_N]){
    printf("Kiem tra Euler: truoc tien phai lien thong va moi dinh co bac chan\n");
    if(!lien_thong(n,g)){
        printf("Do thi khong lien thong\n");
        return 0;
    }
    for(int i=0;i<n;i++){
        int bac=0;
        for(int j=0;j<n;j++){
            if(g[i][j]>0){
                bac++;
            }
        }
        printf("Dinh %d co bac %d\n",i+1,bac);
        if(bac%2!=0){
            printf("Dinh %d co bac le nen khong phai Euler\n",i+1);
            return 0;
        }
    }
    printf("Tat ca dinh co bac chan va do thi lien thong\n");
    return 1;
}

void dijkstra(int n, double g[][MAX_N], int s, int t){
    double inf = INFINITY;
    double *d = malloc(n * sizeof(double));
    int *truoc = malloc(n * sizeof(int));
    int *vis = calloc(n, sizeof(int));

    for(int i=0;i<n;i++){
        d[i] = inf;
        truoc[i] = -1;
    }
    d[s] = 0.0;

    printf("Khoi tao Dijkstra: d[%d]=0, cac dinh khac = INF\n", s + 1);
    for(int i=0;i<n;i++){
        int u = -1;
        double best = inf;
        for(int j=0;j<n;j++){
            if(!vis[j] && d[j] < best){
                best = d[j];
                u = j;
            }
        }
        if(u == -1) break;
        vis[u] = 1;
        printf("Chon dinh %d co khoang cach hien tai ",u+1);
        algo_print_double(d[u]);
        printf("\n");

        for(int v=0;v<n;v++){
            if(g[u][v] > 0 && !vis[v] && d[v] > d[u] + g[u][v]){
                printf("  Cap nhat d[%d]: ",v+1);
                algo_print_double(d[v]);
                printf(" -> ");
                algo_print_double(d[u] + g[u][v]);
                printf(" qua %d\n",u+1);
                d[v] = d[u] + g[u][v];
                truoc[v] = u;
            }
        }
    }

    line();
    printf("        DUONG DI NGAN NHAT\n");
    if(t >= n || isinf(d[t])){
        printf(RED);
        printf("Khong co duong di! d[%d] = inf\n",t+1);
        printf(RESET);
    }else{
        printf(GREEN);
        printf("Do dai ngan nhat = ");
        algo_print_double(d[t]);
        printf("\n");
        int path[MAX_N];
        int k = 0;
        int x = t;
        while(x != -1){
            path[k++] = x;
            x = truoc[x];
        }
        printf("Duong di: ");
        for(int i=k-1;i>=0;i--){
            printf("%d",path[i]+1);
            if(i>0) printf(" -> ");
        }
        printf("\n");
        printf(RESET);
    }
    line();
    free(d);
    free(truoc);
    free(vis);
}

void prim(int n, double g[][MAX_N]){
    double inf = INFINITY;
    int *chon = calloc(n, sizeof(int));
    double *khoa = malloc(n * sizeof(double));
    int *truoc = malloc(n * sizeof(int));
    double tong = 0.0;

    for(int i=0;i<n;i++){
        khoa[i] = inf;
        truoc[i] = -1;
    }
    khoa[0] = 0.0;
    printf("Khoi tao Prim tai dinh 1\n");

    for(int i=0;i<n;i++){
        int u = -1;
        double best = inf;
        for(int v=0;v<n;v++){
            if(!chon[v] && khoa[v] < best){
                best = khoa[v];
                u = v;
            }
        }
        if(u == -1) break;
        chon[u] = 1;
        printf("Chon dinh %d vao cay khung\n",u+1);

        for(int v=0;v<n;v++){
            if(g[u][v] > 0 && !chon[v] && g[u][v] < khoa[v]){
                printf("  Cap nhat khoa[%d]: ",v+1);
                algo_print_double(khoa[v]);
                printf(" -> ");
                algo_print_double(g[u][v]);
                printf(" qua %d\n",u+1);
                khoa[v] = g[u][v];
                truoc[v] = u;
            }
        }
    }

    line();
    printf(YELLOW);
    printf("           CAY KHUNG NHO NHAT PRIM\n");
    printf("\n");
    printf(RESET);
    for(int i=1;i<n;i++){
        printf("%d -- %d : %.2f\n",truoc[i]+1,i+1,g[truoc[i]][i]);
        tong += g[truoc[i]][i];
    }
    printf(GREEN);
    printf("Tong trong so MST = %.2f\n",tong);
    printf(RESET);
    line();
    free(chon);
    free(khoa);
    free(truoc);
}

void kruskal(int n, double g[][MAX_N]){
    int mmax=n*(n-1)/2;
    Canh *ds=malloc(mmax*sizeof(Canh));
    int *par=malloc(n*sizeof(int));
    int *rank=calloc(n,sizeof(int));
    int m=0;
    double tong=0.0;
    int dem=0;
    for(int i=0;i<n;i++){
        par[i]=i;
    }
    for(int i=0;i<n;i++){
        for(int j=i+1;j<n;j++){
            if(g[i][j]>0){
                ds[m].u=i;
                ds[m].v=j;
                ds[m].w=g[i][j];
                m++;
            }
        }
    }
    qsort(ds, m, sizeof(Canh), cmp);
    printf(YELLOW);
    printf("         CAY KHUNG NHO NHAT KRUSKAL\n");
    printf("\n");
    printf(RESET);
    printf("Danh sach canh sau khi sap xep theo trong so:\n");
    for(int i=0;i<m;i++){
        printf("  %lld -- %lld : %.2f\n",ds[i].u+1,ds[i].v+1,ds[i].w);
    }
    printf("Xet tung canh de quyet dinh co them vao cay khung hay khong\n");
    for(int i=0;i<m;i++){
        ll u=ds[i].u;
        ll v=ds[i].v;
        if(goc(par,u)!=goc(par,v)){
            printf("%lld -- %lld : %.2f\n",u+1,v+1,ds[i].w);
            tong+=ds[i].w;
            hop(par,rank,u,v);
            printf("  -> Duoc chon\n");
            dem++;
            if(dem==n-1) break;
        }else{
            printf("%lld -- %lld : %.2f -> bo qua vi tao chu trinh\n",u+1,v+1,ds[i].w);
        }
    }
    printf(GREEN);
    printf("Tong trong so MST = %.2f\n",tong);
    printf(RESET);
    line();
    free(ds);
    free(par);
    free(rank);
}

void thuc_hien_thuat_toan(int che_do_xuat, int n, double g[][MAX_N], int chon, int s, int t){
    if(che_do_xuat==2){
        g_output_to_file = 1;
        g_algo_out = fopen("result.dat","w");
        if(g_algo_out == NULL){
            g_output_to_file = 0;
            g_algo_out = NULL;
            printf(RED);
            printf("Khong the tao file result.dat\n");
            printf(RESET);
            return;
        }
    }

    in_mt(n,g);

    if(chon==1){
        cay_dfs(n,g,s);
    }
    else if(chon==2){
        cay_bfs(n,g,s);
    }
    else if(chon==3){
        la_euler(n,g);
    }
    else if(chon==4){
        dijkstra(n,g,s,t);
    }
    else if(chon==5){
        prim(n,g);
    }
    else if(chon==6){
        kruskal(n,g);
    }

    if(che_do_xuat==2){
        fflush(g_algo_out);
        fclose(g_algo_out);
        g_algo_out = NULL;
        g_output_to_file = 0;
        bao_cao_da_ghi_file();
    }
}

void chay_menu_chuc_nang(int n, double g[][MAX_N], int che_do_xuat){
    int chon;
    do{
        menu_chuc_nang();
        scanf("%d",&chon);
        if (chon < 0 || chon > 6) {
            printf(RED);
            printf("Lua chon khong hop le! Vui long nhap lai.\n");
            printf(RESET);
            continue;
        }
        if(chon==1){
            int s;
            printf("Nhap dinh bat dau: ");
            scanf("%d",&s);
            if (s < 1 || s > n) {
                printf(RED);
                printf("Dinh bat dau khong hop le! Vui long nhap lai.\n");
                printf(RESET);
                continue;
            }
            thuc_hien_thuat_toan(che_do_xuat,n,g,1,s-1,0);
        }
        else if(chon==2){
            int s;
            printf("Nhap dinh bat dau: ");
            scanf("%d",&s);
            if (s < 1 || s > n) {
                printf(RED);
                printf("Dinh bat dau khong hop le! Vui long nhap lai.\n");
                printf(RESET);
                continue;
            }
            thuc_hien_thuat_toan(che_do_xuat,n,g,2,s-1,0);
        }
        else if(chon==3){
            thuc_hien_thuat_toan(che_do_xuat,n,g,3,0,0);
        }
        else if(chon==4){
            int s,t;
            printf("Nhap dinh dau va cuoi: ");
            scanf("%d %d",&s,&t);
            if (s < 1 || s > n || t < 1 || t > n) {
                printf(RED);
                printf("Dinh dau/cuoi khong hop le! Vui long nhap lai.\n");
                printf(RESET);
                continue;
            }
            thuc_hien_thuat_toan(che_do_xuat,n,g,4,s-1,t-1);
        }
        else if(chon==5){
            thuc_hien_thuat_toan(che_do_xuat,n,g,5,0,0);
        }
        else if(chon==6){
            thuc_hien_thuat_toan(che_do_xuat,n,g,6,0,0);
        }
        if(chon!=0){
            pause_screen();
            clear_screen();
            header_big();
            if(che_do_xuat==1){
                in_mt(n,g);
            }
        }
    }while(chon!=0);
}

signed main(){
    system("color f0");
    srand(time(NULL));
    printf("\033[30;47m");
    printf("\033[2J\033[H");
    clear_screen();
    header_big();
    int chon;
    do{
        menu_nhap();
        scanf("%d",&chon);
        if (chon < 0 || chon > 3) {
            printf(RED);
            printf("Lua chon khong hop le! Vui long nhap lai.\n");
            printf(RESET);
            continue;
        }
        if(chon==1){
            int che_do_xuat;
            int n;
            printf(CYAN);
            printf("Nhap so dinh n (2..100): ");
            printf(RESET);
            scanf("%d",&n);

            double g[MAX_N][MAX_N];
            nhap_mt(n,g);
            menu_xuat();
            scanf("%d",&che_do_xuat);
            if (che_do_xuat < 0 || che_do_xuat > 2) {
                printf(RED);
                printf("Lua chon khong hop le! Vui long nhap lai.\n");
                printf(RESET);
                continue;
            }
            if(che_do_xuat==0){
                continue;
            }
            in_mt(n,g);
            chay_menu_chuc_nang(n,g,che_do_xuat);
        }
        else if(chon==2){
            int che_do_xuat;
            int n;
            char ten_file[256];
            double g[MAX_N][MAX_N];

            printf("\n");
            line();
            printf(YELLOW);
            printf(" HUONG DAN CAU TRUC FILE DO THI (.dat):\n");
            printf(RESET);
            printf(" - Dong 1          : So dinh cua do thi.\n");
            printf(" - n dong tiep theo: Ma tran trong so kich thuoc n x n.\n");
            printf(" (Luu y: Cac gia tri cach nhau boi khoang trang hoac tab)\n");
            line();

            printf(CYAN);
            printf("Nhap duong dan file .dat: ");
            printf(RESET);
            scanf("%255s",ten_file);

            if(doc_mt_tu_file(ten_file,&n,g)){
                printf(GREEN);
                printf("\n>>> Da doc du lieu tu file thanh cong!\n");
                printf(RESET);
                menu_xuat();
                scanf("%d",&che_do_xuat);
                if (che_do_xuat < 0 || che_do_xuat > 2) {
                    printf(RED);
                    printf("Lua chon khong hop le! Vui long nhap lai.\n");
                    printf(RESET);
                    continue;
                }
                if(che_do_xuat==0){
                    continue;
                }
                in_mt(n,g);
                chay_menu_chuc_nang(n,g,che_do_xuat);
            }else{
                printf(RED);
                printf("Khong the doc file .dat nay.\n");
                printf(RESET);
            }

        }
        else if(chon==3){
            int n;
            double wmax;

            printf(CYAN);
            printf("Nhap so dinh n (2..100): ");
            printf(RESET);
            scanf("%d",&n);

            printf(CYAN);
            printf("Nhap trong so lon nhat: ");
            printf(RESET);
            scanf("%lf",&wmax);

            double g[MAX_N][MAX_N];
            tao_ngau_nhien(n,wmax,g);
            int che_do_xuat;
            menu_xuat();
            scanf("%d",&che_do_xuat);
            if(che_do_xuat==0){
                continue;
            }
            in_mt(n,g);
            chay_menu_chuc_nang(n,g,che_do_xuat);
        }
    }while(chon!=0);
    printf(GREEN);
    printf("Cam on da su dung chuong trinh!\n");
    printf(RESET);
    return 0;
}
