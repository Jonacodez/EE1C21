#include "robot_lib.h"
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int find_route (int x, int road[40][50*3+1], int mazes[13][13], int s_m, int s_n, int m_f, int n_f)
{
    int b,q,i,k,m,n,marker,turn,markturn,p,cnt,brk_m = 0,brk_n = 0;
    int s;
    int stations[2][2];
    i = 0;
    p = 0;
    cnt = 0;
    stations[0][0] = s_m;
    stations[0][1] = s_n;
    stations[1][0] = m_f;
    stations[1][1] = n_f;
    k = 0;
        q = 1;
        while (p <= cnt)
        {
            m = stations[0][0];
            n = stations[0][1];
            turn = 0;
            markturn = 0;
            i = mazes[stations[0][0]][stations[0][1]];
            printf("\n\n%d", i);
            while (i > 0)
            {
                marker = 0;
                if (mazes[m+1][n] == i-1 && m <12)
                {
                    marker = 1;
                    if (markturn != 1)
                    {
                        ++turn;
                    }
                    markturn = 1;
                    ++k;
                }
                if (mazes[m-1][n] == i-1 && m >0)
                {
                    if (marker == 1)
                    {
                        brk_m = m + 1;
                        brk_n = n;
                    }
                    else
                    {
                        marker = 2;
                        if (markturn != 2)
                        {
                            ++turn;
                        }
                        markturn = 2;
                    }

                    ++k;
                }
                if (mazes[m][n-1] == i-1 && n > 0)
                {
                    if (marker == 1)
                    {
                        brk_m = m + 1;
                        brk_n = n;
                    }
                    else if (marker == 2)
                    {
                        brk_m = m - 1;
                        brk_n = n;
                    }
                    else
                    {
                        marker = 3;
                        if (markturn != 3)
                        {
                            ++turn;
                        }
                        markturn = 3;

                    }
                    ++k;
                }
                if (mazes[m][n+1] == i-1 && n <12)
                {
                    if (marker == 1)
                    {
                        brk_m = m + 1;
                        brk_n = n;
                    }
                    else if (marker == 2)
                    {
                        brk_m = m - 1;
                        brk_n = n;
                    }
                    else if (marker == 3)
                    {
                        brk_m = m;
                        brk_n = n - 1;
                    }
                    else
                    {
                        if (markturn != 4)
                        {
                            ++turn;
                        }
                        markturn = 4;

                    }
                    ++k;
                }
                if (k > 1)
                {
                    ++cnt;
                }
                k = 0;
                printf("(%d %d)", m, n);
                road[p][q] = m;
                road[p][q+1] = n;
                q = q+2;


                i = i - 2;
                if (marker == 1)
                {
                    m = m + 2;
                    n = n;
                }
                else if (marker == 2)
                {
                    m = m - 2;
                    n = n;
                }
                else if (marker == 3)
                {
                    m = m;
                    n = n - 2;
                }
                else
                {
                    m = m;
                    n = n + 2;
                }
            }
            road[p][0] = turn;
            mazes[brk_m][brk_n] = -1;
            x=q;
            q=1;
            ++p;

        }
        q=x;
        for (b = 0; b < p-1; b++)
        {
            for (n = 0; n < p-1-b; n++)
            {
                if (road[n][0] > road[n+1][0])
                {
                    for (i=0; i<x; i++)
                    {
                        s = road[n][i];
                        road[n][i] = road[n+1][i];
                        road[n+1][i] = s;
                    }
                }
            }
        }
            for (i=0; i<x; i++)
            {
                printf("c %d ", road[0][i]);
            }
        cnt = 0;
        p = 0;
        printf(" x = %d ", x);
    return x;
}

int phase1(int x, int maze[13][13], int c_m, int c_n, int m_f, int n_f, int road[40][50*3+1])
{
    int i = 1;
    int m = 0;
    int n = 0;
    int check = 0;
    maze[m_f][n_f] = i;
    /*write out the shortest routes*/
    while (maze[c_m][c_n] < 1)
    {
        check = 0;
        while (m < 13)
        {
            while (n < 13)
            {
                if (maze[m][n] == i)
                {
                    if (maze[m+1][n] == 0 && m < 12)
                    {
                        maze[m+1][n] = i + 1;
                        check=1;
                    }
                    if (maze[m-1][n] == 0 && m > 0)
                    {
                        maze[m-1][n] = i + 1;
                        check=1;
                    }
                    if (maze[m][n+1] == 0 && n < 12)
                    {
                        maze[m][n+1] = i + 1;
                        check=1;
                    }
                    if (maze[m][n-1] == 0 && n > 0)
                    {
                        maze[m][n-1] = i + 1;
                        check=1;
                    }
                }
                n++;
            }
            n=0;
            m++;
        }
        if (check == 0){
            printf("ERROR: No route possible");
            i = -1;
            break;
        }
        m=0;
        i++;
    }
    m=0;
    n=0;
    while(m<13){
        while(n<13){
            if(maze[m][n] > -1 && maze[m][n] < 10){
                printf(" ");
            }
            printf("%d ", maze[m][n]);
            n++;
        }
        n=0;
        printf("\n");
        m++;
    }
    m=0;
    printf("\n");
    printf("x = %d ", x);
    x = find_route(x, road, maze, c_m, c_n, m_f, n_f);
    printf("x = %d ", x);
    return x;
}


int algortihm(int maze[13][13], int maze2[13][13], int branch){
    int save_m[100];
    int save_n[100];
    int save = 1;
    int save_i[100];
    int i = branch;
    int m = 0, m_n;
    int n = 0, n_n;
    int mark, cnt = 0, x = 0, y=0, check;
    while(y<13){
    while(x<13){
    i=branch;
    if(maze[y][x] == branch){
    m=y;
    n=x;
    save=1;
    while (1)
    {
        mark = 0;
        if(maze[m][n-1] == 0 && n >= 1){
            maze[m][n-1] = i+1;
            mark = 1;
            m_n = m;
            n_n = n-1;
        }
        if(maze[m-1][n] == 0 && m >= 1){
            if (mark == 1){
            save_m[save] = m;
            save_n[save] = n;
            save_i[save] = i;
            save++;
            }
            else
            {
                mark = 2;
                maze[m-1][n] = i+1;
                m_n = m-1;
                n_n = n;

            }
        }
        if(maze[m][n+1] == 0 && n <= 11){
            if (mark == 1 || mark == 2){
            save_m[save] = m;
            save_n[save] = n;
            save_i[save] = i;
            save++;
            }
            else
            {
                mark = 3;
                maze[m][n+1] = i+1;
                m_n = m;
                n_n = n+1;
            }
        }
        if (maze[m+1][n] == 0 && m <= 11){
            if (mark == 1 || mark == 2 || mark == 3){
            save_m[save] = m;
            save_n[save] = n;
            save_i[save] = i;
            save++;
            }
            else
            {
                mark = 4;
                maze[m+1][n] = i+1;
                m_n = m+1;
                n_n = n;
            }
        }
        if(mark == 4 || mark == 1 || mark == 2 || mark == 3){
            m = m_n;
            n = n_n;
            i++;
        }
        else {
            --save;
            if (save < 1){
            cnt++;
            }
            else{
            m = save_m[save];
            n = save_n[save];
            i = save_i[save];
            }
        }
        if (cnt > 0){
            break;
        }
    }
    save=0;
    cnt=0;
    m=0;
    n=0;

    /*retrieve biggest value*/
    while (m < 13){
        while (n < 13){
            if(maze[m][n] > check){
                m_n = m;
                n_n = n;
                check = maze[m][n];
            }
            n++;
        }
        m++;
        n=0;
    }
    m = m_n;
    n = n_n;
    maze2[m][n] = branch;
    i = maze[m_n][n_n];
    /*retrieve main route*/
    while (i>branch){
        maze[m][n] = -1;
        if(maze[m][n-1] == i-1 && n>0){
            maze2[m][n-1] = branch;
            mark = 1;
            m_n = m;
            n_n = n-1;
        }
        if(maze[m][n-1] == i+1 && n>0){
            maze2[m][n-1] = branch+1;
        }
        if(maze[m-1][n] == i-1 && m>0){
            if (mark == 1){
                maze2[m-1][n] = branch+1;
            }
            else{
                mark = 2;
                maze2[m-1][n] = branch;
                m_n = m-1;
                n_n = n;

            }
        }
        if (maze[m-1][n] == i+1 && m>0){
            maze2[m-1][n] = branch+1;
        }
        if(maze[m][n+1] == i-1 && n<12){
            if (mark == 1 || mark == 2){
                maze2[m][n+1] = branch+1;
            }
            else
            {
                mark = 3;
                maze2[m][n+1] = branch;
                m_n = m;
                n_n = n+1;
            }
        }
        if(maze[m][n+1] == i+1 && n<12){
            maze2[m][n+1] = branch+1;
        }
        if (maze[m+1][n] == i-1 && m<12){
            if (mark == 1 || mark == 2 || mark == 3){
                maze2[m+1][n] = branch+1;
            }
            else
            {
                mark = 4;
                maze2[m+1][n] = branch;
                m_n = m+1;
                n_n = n;
            }
        }
        if (maze[m+1][n] == i+1 && m<12){
            maze2[m+1][n] = branch+1;
        }
        m = m_n;
        n = n_n;
        i--;
        mark = 0;
    }

    m=0;
    n=0;
    while (m < 13)
    {
        while (n < 13)
        {
            if(maze[m][n]>branch){
                maze[m][n] = -1;
            }
            n++;
        }
        m++;
        n = 0;
    }

    }


    x++;
    }
    y++;
    x=0;
    }
    y=0;
    return 0;
}



int phase(int maze[13][13], int maze2[13][13], char movement[100], int startm, int startn, int mop, char direction){
    int coordinates[500];
    int traceback[200][300];
    int save_m[100];
    int save_n[100];
    int save = 1;
    int save_i[100];
    int save_p[300];
    int i = 1;
    int proof;
    int m, m_n;
    int n, n_n;
    int endt;
    int mark, cnt=0, j, k, branch = 1, checkb = 1, p, b, x, c = 0, y, cntcoo;
    int q = 0;
    int check=0;
    m = startm;
    n = startn;
    maze[m][n]=1;



    /*fill in*/
    while (1)
    {
        mark = 0;
        if(maze[m][n-1] == 0 && n >= 1){
            maze[m][n-1] = i+1;
            mark = 1;
            m_n = m;
            n_n = n-1;
        }
        if(maze[m-1][n] == 0 && m >= 1){
            if (mark == 1){
            save_m[save] = m;
            save_n[save] = n;
            save_i[save] = i;
            save++;
            }
            else
            {
                mark = 2;
                maze[m-1][n] = i+1;
                m_n = m-1;
                n_n = n;

            }
        }
        if(maze[m][n+1] == 0 && n <= 11){
            if (mark == 1 || mark == 2){
            save_m[save] = m;
            save_n[save] = n;
            save_i[save] = i;
            save++;
            }
            else
            {
                mark = 3;
                maze[m][n+1] = i+1;
                m_n = m;
                n_n = n+1;
            }
        }
        if (maze[m+1][n] == 0 && m <= 11){
            if (mark == 1 || mark == 2 || mark == 3){
            save_m[save] = m;
            save_n[save] = n;
            save_i[save] = i;
            save++;
            }
            else
            {
                mark = 4;
                maze[m+1][n] = i+1;
                m_n = m+1;
                n_n = n;
            }
        }
        if(mark == 4 || mark == 1 || mark == 2 || mark == 3){
            m = m_n;
            n = n_n;
            i++;
        }
        else {
            --save;
            if (save < 1){
                   cnt++;
            }
            else{
            m = save_m[save];
            n = save_n[save];
            i = save_i[save];
            }
        }
        if (cnt == 1){
            break;
        }
    }


    m=0;
    n=0;

    /*retrieve biggest value*/
    while (m < 13){
        while (n < 13){
            if(maze[m][n] >= check){
                m_n = m;
                n_n = n;
                check = maze[m][n];
            }
            n++;
        }
        m++;
        n=0;
    }


    m = m_n;
    n = n_n;
    maze2[m][n] = 1;
    i = maze[m_n][n_n];
    /*retrieve main route*/
    while (i>0){
        maze[m][n] = -1;
        if(maze[m][n-1] == i-1 && n>0){
            maze2[m][n-1] = 1;
            mark = 1;
            m_n = m;
            n_n = n-1;
        }
        if(maze[m][n-1] == i+1 && n>0){
            maze2[m][n-1] = 2;
        }
        if(maze[m-1][n] == i-1 && m>0){
            if (mark == 1){
                maze2[m-1][n] = 2;
            }
            else{
                mark = 2;
                maze2[m-1][n] = 1;
                m_n = m-1;
                n_n = n;

            }
        }
        if (maze[m-1][n] == i+1 && m>0){
            maze2[m-1][n] = 2;
        }
        if(maze[m][n+1] == i-1 && n<12){
            if (mark == 1 || mark == 2){
                maze2[m][n+1] = 2;
            }
            else
            {
                mark = 3;
                maze2[m][n+1] = 1;
                m_n = m;
                n_n = n+1;
            }
        }
        if(maze[m][n+1] == i+1 && n<12){
            maze2[m][n+1] = 2;
        }
        if (maze[m+1][n] == i-1 && m<12){
            if (mark == 1 || mark == 2 || mark == 3){
                maze2[m+1][n] = 2;
            }
            else
            {
                mark = 4;
                maze2[m+1][n] = 1;
                m_n = m+1;
                n_n = n;
            }
        }
        if (maze[m+1][n] == i+1 && m<12){
            maze2[m+1][n] = 2;
        }
        m = m_n;
        n = n_n;
        i--;
        mark = 0;
    }
    m = 0;
    n = 0;
    for (j = 0; j < 13; ++j)
    {
        for (k = 0; k < 13; ++k)
        {
            maze[j][k] = maze2[j][k];
        }
    }
    m=0;
    n=0;
    proof = 5;
    while(proof>0){
    proof--;
    checkb = 0;
    while (m < 13)
    {
        while (n < 13)
        {
            if(maze2[m][n] == 0){
                checkb++;
            }
            n++;
        }
        m++;
        n=0;
    }
    m=0;
    if(checkb>0){
        branch++;
        algortihm(maze, maze2, branch);
        for (j = 0; j < 13; ++j)
        {
            for (k = 0; k < 13; ++k)
            {
                maze[j][k] = maze2[j][k];
            }
        }
    }
    }
    /*print maze*/
    save = 2;
    save_m[save] = 0;
    save_n[save] = 0;
    save_i[save] = 0;
    m=0;
    n=0;
    while(m<13){
        while(n<13){
            if(maze[m][n] > 0 && maze[m][n] < 10){
                printf(" ");
            }
            printf("%d ", maze[m][n]);
            n++;
        }
        n=0;
        printf("\n");
        m++;
    }
    m=0;
    printf("\n");
    /*follow back*/
    m=startm;
    n=startn;
    p=0;
    coordinates[0] = m;
    coordinates[1] = n;
    cnt=2;
    cntcoo=0;
    while(1){
        i = maze2[m][n];
        mark = 1;
        cntcoo++;
        printf("(%d %d) ", m, n);
        if(maze2[m+1][n] == i+1 && m<12){
            coordinates[cnt] = m+1;
            coordinates[cnt+1] = n;
            save++;
            save_m[save] = m;
            save_n[save] = n;
            save_i[save] = i;
            save_p[save] = p;
            q++;
            p=0;
            traceback[q][2*p] = m;
            traceback[q][2*p+1] = n;
            m_n = m+1;
            n_n = n;
        }
        else if(maze2[m-1][n] == i+1 && m>0){
            coordinates[cnt] = m-1;
            coordinates[cnt+1] = n;
            save++;
            save_m[save] = m;
            save_n[save] = n;
            save_i[save] = i;
            save_p[save] = p;
            q++;
            p=0;
            traceback[q][2*p] = m;
            traceback[q][2*p+1] = n;
            m_n = m-1;
            n_n = n;
        }
        else if(maze2[m][n+1] == i+1 && n<12){
            coordinates[cnt] = m;
            coordinates[cnt+1] = n+1;
            save++;
            save_m[save] = m;
            save_n[save] = n;
            save_i[save] = i;
            save_p[save] = p;
            q++;
            p=0;
            traceback[q][2*p] = m;
            traceback[q][2*p+1] = n;


            m_n = m;
            n_n = n+1;
        }
        else if(maze2[m][n-1] == i+1 && n>0){
            coordinates[cnt] = m;
            coordinates[cnt+1] = n-1;
            save++;
            save_m[save] = m;
            save_n[save] = n;
            save_i[save] = i;
            save_p[save] = p;
            q++;
            p=0;
            traceback[q][2*p] = m;
            traceback[q][2*p+1] = n;


            m_n = m;
            n_n = n-1;
        }
        else if(maze2[m+1][n] == i && m <12){
            coordinates[cnt] = m+1;
            coordinates[cnt+1] = n;
            p++;
            traceback[q][2*p] = m;
            traceback[q][2*p+1] = n;
            maze2[m][n] = -1;
            m_n = m+1;
            n_n = n;
        }
        else if(maze2[m-1][n] == i && m >0){
            coordinates[cnt] = m-1;
            coordinates[cnt+1] = n;
            p++;
            traceback[q][2*p] = m;
            traceback[q][2*p+1] = n;
            maze2[m][n] = -1;
            m_n = m-1;
            n_n = n;
        }
        else if(maze2[m][n+1] == i && n<12){
            coordinates[cnt] = m;
            coordinates[cnt+1] = n+1;
            p++;
            traceback[q][2*p] = m;
            traceback[q][2*p+1] = n;
            maze2[m][n] = -1;

            m_n = m;
            n_n = n+1;
        }
        else if(maze2[m][n-1] == i && n>0){
            coordinates[cnt] = m;
            coordinates[cnt+1] = n-1;
            p++;
            traceback[q][2*p] = m;
            traceback[q][2*p+1] = n;
            maze2[m][n] = -1;
            m_n = m;
            n_n = n-1;
        }
        else{
            maze2[m][n] = -1;
            x=1;
            while(p>-1){
                coordinates[cnt] = traceback[q][p*2];
                coordinates[cnt+1] = traceback[q][p*2+1];
                cnt = cnt+2;
                --p;
            }
            --q;
            m = save_m[save];
            n = save_n[save];
            p = save_p[save];
            i = save_i[save];
            save--;
            mark = 0;
        }
        if(mark==1){
            m = m_n;
            n = n_n;
            cnt = cnt+2;
        }
    x=0;
    y=0;
    k=0;
    while (x < 13)
    {
        while (y < 13)
        {
            if(maze2[x][y] > 0){
                k++;
            }
            y++;
        }
        x++;
        y = 0;
    }
    if(k == 0){
            break;
    }

    }
    endt = cnt-2;


    c=0;
    b=0;
/*from coordinates to direction*/
    while(cntcoo > 0)
                {
                    if(coordinates[b*2] - 2 == coordinates[b*2+4] && direction == 'n')
                    {
                        movement[c] = 'f';
                        direction = 'n';
                    }
                    else if(coordinates[b*2] - 2 == coordinates[b*2+4] && direction == 'e')
                    {
                        movement[c] = 'l';
                        direction = 'n';
                    }
                    else if(coordinates[b*2] - 2 == coordinates[b*2+4] && direction == 'w')
                    {
                        movement[c] = 'r';
                        direction = 'n';
                    }
                    else if(coordinates[b*2] - 2 == coordinates[b*2+4] && direction == 's')
                    {
                        movement[c] = 'u';
                        direction = 'n';
                    }
                    else if(coordinates[b*2] + 2 == coordinates[b*2+4] && direction == 'e')
                    {
                        movement[c] = 'r';
                        direction = 's';
                    }
                    else if(coordinates[b*2] + 2 == coordinates[b*2+4] && direction == 'w')
                    {
                        movement[c] = 'l';
                        direction = 's';
                    }
                    else if(coordinates[b*2] + 2 == coordinates[b*2+4] && direction == 's')
                    {
                        movement[c] = 'f';
                        direction = 's';
                    }
                    else if(coordinates[b*2] + 2 == coordinates[b*2+4] && direction == 'n')
                    {
                        movement[c] = 'u';
                        direction = 's';
                    }
                    else if(coordinates[b*2+1] + 2 == coordinates[b*2+5] && direction == 'n')
                    {
                        movement[c] = 'r';
                        direction = 'e';
                    }
                    else if(coordinates[b*2+1] + 2 == coordinates[b*2+5] && direction == 'e')
                    {
                        movement[c] = 'f';
                        direction = 'e';
                    }
                    else if(coordinates[b*2+1] + 2 == coordinates[b*2+5] && direction == 's')
                    {
                        movement[c] = 'l';
                        direction = 'e';
                    }
                    else if(coordinates[b*2+1] + 2 == coordinates[b*2+5] && direction == 'w')
                    {
                        movement[c] = 'u';
                        direction = 'e';
                    }
                    else if(coordinates[b*2+1] - 2 == coordinates[b*2+5] && direction == 'n')
                    {
                        movement[c] = 'l';
                        direction = 'w';
                    }
                    else if(coordinates[b*2+1] - 2 == coordinates[b*2+5] && direction == 'w')
                    {
                        movement[c] = 'f';
                        direction = 'w';
                    }
                    else if(coordinates[b*2+1] - 2 == coordinates[b*2+5] && direction == 's')
                    {
                        movement[c] = 'r';
                        direction = 'w';
                    }
                    else if(coordinates[b*2+1] - 2 == coordinates[b*2+5] && direction == 'e')
                    {
                        movement[c] = 'u';
                        direction = 'w';
                    }
                    else{
                        if(coordinates[b*2] - 1 == coordinates[b*2+2] && direction == 'n')
                    {
                        movement[c] = 'f';
                        direction = 'n';
                    }
                    else if(coordinates[b*2] - 1 == coordinates[b*2+2] && direction == 'e')
                    {
                        movement[c] = 'l';
                        direction = 'n';
                    }
                    else if(coordinates[b*2] - 1 == coordinates[b*2+2] && direction == 'w')
                    {
                        movement[c] = 'r';
                        direction = 'n';
                    }
                    else if(coordinates[b*2] - 1 == coordinates[b*2+2] && direction == 's')
                    {
                        movement[c] = 'u';
                        direction = 'n';
                    }
                    else if(coordinates[b*2] + 1 == coordinates[b*2+2] && direction == 'e')
                    {
                        movement[c] = 'r';
                        direction = 's';
                    }
                    else if(coordinates[b*2] + 1 == coordinates[b*2+2] && direction == 'w')
                    {
                        movement[c] = 'l';
                        direction = 's';
                    }
                    else if(coordinates[b*2] + 1 == coordinates[b*2+2] && direction == 's')
                    {
                        movement[c] = 'f';
                        direction = 's';
                    }
                    else if(coordinates[b*2] + 1 == coordinates[b*2+2] && direction == 'n')
                    {
                        movement[c] = 'u';
                        direction = 's';
                    }
                    else if(coordinates[b*2+1] + 1 == coordinates[b*2+3] && direction == 'n')
                    {
                        movement[c] = 'r';
                        direction = 'e';
                    }
                    else if(coordinates[b*2+1] + 1 == coordinates[b*2+3] && direction == 'e')
                    {
                        movement[c] = 'f';
                        direction = 'e';
                    }
                    else if(coordinates[b*2+1] + 1 == coordinates[b*2+3] && direction == 's')
                    {
                        movement[c] = 'l';
                        direction = 'e';
                    }
                    else if(coordinates[b*2+1] + 1 == coordinates[b*2+3] && direction == 'w')
                    {
                        movement[c] = 'u';
                        direction = 'e';
                    }
                    else if(coordinates[b*2+1] - 1 == coordinates[b*2+3] && direction == 'n')
                    {
                        movement[c] = 'l';
                        direction = 'w';
                    }
                    else if(coordinates[b*2+1] - 1 == coordinates[b*2+3] && direction == 'w')
                    {
                        movement[c] = 'f';
                        direction = 'w';
                    }
                    else if(coordinates[b*2+1] - 1 == coordinates[b*2+3] && direction == 's')
                    {
                        movement[c] = 'r';
                        direction = 'w';
                    }
                    else if(coordinates[b*2+1] - 1 == coordinates[b*2+3] && direction == 'e')
                    {
                        movement[c] = 'u';
                        direction = 'w';
                    }
                    c++;
                    endt++;
                    movement[c] = 'u';
                    if(direction == 'n'){
                        direction = 's';
                    }
                    else if(direction == 's'){
                        direction = 'n';
                    }
                    else if(direction == 'e'){
                        direction = 'w';
                    }
                    else if(direction == 'w'){
                        direction = 'e';
                    }
                    }
                    c++;
                    b=b+2;
                    cntcoo = cntcoo-2;
                }
                printf("\n");
                k=c;
                while(c>0){
                    printf("%c ", movement[k-c]);
                    c--;
                }
    return 0;
}


int main(int argc, char const* argv[]){
    int stations[3], status, x=0, m_f=0, n_f=0;
    int road[40][50*3+1];
    int i=0, c_m = 12, c_n = 4, lock;
    int save_i[20];
    int first_i;
    int save = 0;
    int cnt_save[20];
    int coordinates[1000];
    int m_co[20][300];
    char end_dir[20];
    coordinates[0]=12;
    coordinates[1]=4;
    char ready;
    char direction = 'n', n_dir;
    char in, out;
    char movement[100];
    char m_movement[20][100];
    char l_movement[100];
    int maze[13][13] = {{-1, -1, -1, -1,  -1, -1,  -1, -1,  -1, -1, -1, -1, -1},
        {-1, -1, -1, -1,  -1, -1,  -1, -1,  -1, -1, -1, -1, -1},
        {-1, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0, -1, -1},
        {-1, -1,  0, -1,  0, -1,  0, -1,  0, -1,  0, -1, -1},
        {-1,  -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  -1,  -1},
        {-1, -1,  0, -1,  0, -1,  0, -1,  0, -1,  0, -1, -1},
        {-1,  -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  -1,  -1},
        {-1, -1,  0, -1,  0, -1,  0, -1,  0, -1,  0, -1, -1},
        {-1,  -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  -1,  -1},
        {-1, -1,  0, -1,  0, -1,  0, -1,  0, -1,  0, -1, -1},
        {-1, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0, -1, -1},
        {-1, -1, -1, -1,  0, -1,  -1, -1,  -1, -1, -1, -1, -1},
        {-1, -1, -1, -1,  0, -1,  -1, -1,  -1, -1, -1, -1, -1}
    };
    int maze2[2][13][13];
    int b, j, k;
    robot_lib_connect(NULL, stations, 'c', NULL);
    for (i = 0; i < 2; ++i)
    {
        for (j = 0; j < 13; ++j)
        {
            for (k = 0; k < 13; ++k)
            {
                maze2[i][j][k] = maze[j][k];
            }
        }
    }
    i=0;
    printf("Ready to go? (Y/N)\n");/*ready for action?*/
    ready = getchar();
    if(ready == 'N' || ready == 'n')
    {
        printf("\ndisconnecting:\n");
        robot_lib_disconnect();
        return 0;
    }
    while (1)
    {
        robot_lib_read_byte(&out);
        if (out == 'n' || out == 'z' || out == 'g' || out == 'x' || out == 'm' || out == 'e')
        {
            printf("\n");
            printf("\n");
            printf("read: %c", out);
            printf("\n");
        }
        if (out == 'n')
        {
            break;
        }
        if (out == 'z')
        {
            n_dir=direction;
            phase(maze2[0], maze2[1], movement, 12, 4, i, direction);
            /*pre laid path*/
            /*into coordinates*/
            direction = 'n';
            i=0;
            while (movement[i] == 'f' || movement[i] == 'r' || movement[i] == 'l' || movement[i] == 'u')
            {
                if(movement[i] == 'f' && direction == 'n'){
                coordinates[i*2+2] = coordinates[i*2]-2;
                coordinates[i*2+3] = coordinates[i*2+1];
                direction = 'n';
            }
            else if(movement[i] == 'u' && direction == 'n'){
                coordinates[i*2+2] = coordinates[i*2]+2;
                coordinates[i*2+3] = coordinates[i*2+1];
                direction = 's';
            }
            else if(movement[i] == 'r' && direction == 'n'){
                coordinates[i*2+2] = coordinates[i*2];
                coordinates[i*2+3] = coordinates[i*2+1]+2;
                direction = 'e';
            }
            else if(movement[i] == 'l' && direction == 'n'){
                coordinates[i*2+2] = coordinates[i*2];
                coordinates[i*2+3] = coordinates[i*2+1]-2;
                direction = 'w';
            }
            else if(movement[i] == 'f' && direction == 's'){
                coordinates[i*2+2] = coordinates[i*2]+2;
                coordinates[i*2+3] = coordinates[i*2+1];
                direction = 's';
            }
            else if(movement[i] == 'u' && direction == 's'){
                direction = 'n';
                coordinates[i*2+2] = coordinates[i*2]-2;
                coordinates[i*2+3] = coordinates[i*2+1];
            }
            else if(movement[i] == 'r' && direction == 's'){
                coordinates[i*2+2] = coordinates[i*2];
                coordinates[i*2+3] = coordinates[i*2+1]-2;
                direction = 'w';
            }
            else if(movement[i] == 'l' && direction == 's'){
                coordinates[i*2+2] = coordinates[i*2];
                coordinates[i*2+3] = coordinates[i*2+1]+2;
                direction = 'e';
            }
            else if(movement[i] == 'f' && direction == 'e'){
                coordinates[i*2+2] = coordinates[i*2];
                coordinates[i*2+3] = coordinates[i*2+1]+2;
                direction = 'e';
            }
            else if(movement[i] == 'u' && direction == 'e'){
                direction = 'w';
                coordinates[i*2+2] = coordinates[i*2];
                coordinates[i*2+3] = coordinates[i*2+1]-2;
            }
            else if(movement[i] == 'r' && direction == 'e'){
                coordinates[i*2+2] = coordinates[i*2]+2;
                coordinates[i*2+3] = coordinates[i*2+1];
                direction = 's';
            }
            else if(movement[i] == 'l' && direction == 'e'){
                coordinates[i*2+2] = coordinates[i*2]-2;
                coordinates[i*2+3] = coordinates[i*2+1];
                direction = 'n';
            }
            else if(movement[i] == 'f' && direction == 'w'){
                coordinates[i*2+2] = coordinates[i*2];
                coordinates[i*2+3] = coordinates[i*2+1]-2;
                direction = 'w';
            }
            else if(movement[i] == 'u' && direction == 'w'){
                direction = 'e';
                coordinates[i*2+2] = coordinates[i*2];
                coordinates[i*2+3] = coordinates[i*2+1]+2;
            }
            else if(movement[i] == 'l' && direction == 'w'){
                coordinates[i*2+2] = coordinates[i*2]+2;
                coordinates[i*2+3] = coordinates[i*2+1];
                direction = 's';
            }
            else if(movement[i] == 'r' && direction == 'w'){
                coordinates[i*2+2] = coordinates[i*2]-2;
                coordinates[i*2+3] = coordinates[i*2+1];
                direction = 'n';
            }
            printf("(%d %d) ", coordinates[i*2], coordinates[i*2+1]);
            i++;
        }
            direction='n';
            i=0;
        }






        if (out == 'm')
        {
            printf("\nsave: %d\ncnt_save: %d i: %d", save, cnt_save[save], i);
            if(save == 0){
            if (lock==1){
                i++;}
            printf("(%d %d) (%d %d) ", coordinates[(i-1)*2], coordinates[(i-1)*2+1], coordinates[(i-1)*2+2], coordinates[(i-1)*2+3]);
            if (coordinates[(i-1)*2+2] + 2 == coordinates[(i-1)*2])
            {
                maze[coordinates[(i-1)*2]-1][coordinates[(i-1)*2+1]]  = -1;
                direction = 's';
            }
            if (coordinates[(i-1)*2+2] - 2 == coordinates[(i-1)*2])
            {
                maze[coordinates[(i-1)*2]+1][coordinates[(i-1)*2+1]] = -1;
                direction = 'n';
            }
            if (coordinates[(i-1)*2+3] + 2 == coordinates[(i-1)*2+1])
            {
                maze[coordinates[(i-1)*2]][coordinates[(i-1)*2+1]-1] = -1;
                direction = 'e';
            }
            if (coordinates[(i-1)*2+3] - 2 == coordinates[(i-1)*2+1])
            {
                maze[coordinates[(i-1)*2]][coordinates[(i-1)*2+1]+1] = -1;
                direction = 'w';
            }
            }
            else{
            if (lock==1){
                    save++;
                    i++;}
            printf("(%d %d) (%d %d) ", m_co[save-1][(i-1)*2], m_co[save-1][(i-1)*2+1], m_co[save-1][(i-1)*2+2], m_co[save-1][(i-1)*2+3]);
            if (m_co[save-1][(i-1)*2+2] + 2 == m_co[save-1][(i-1)*2])
            {
                maze[m_co[save-1][(i-1)*2]-1][m_co[save-1][(i-1)*2+1]]  = -1;
                direction = 's';
            }
            if (m_co[save-1][(i-1)*2+2] - 2 == m_co[save-1][(i-1)*2])
            {
                maze[m_co[save-1][(i-1)*2]+1][m_co[save-1][(i-1)*2+1]] = -1;
                direction = 'n';
            }
            if (m_co[save-1][(i-1)*2+3] + 2 == m_co[save-1][(i-1)*2+1])
            {
                maze[m_co[save-1][(i-1)*2]][m_co[save-1][(i-1)*2+1]-1] = -1;
                direction = 'e';
            }
            if (m_co[save-1][(i-1)*2+3] - 2 == m_co[save-1][(i-1)*2+1])
            {
                maze[m_co[save-1][(i-1)*2]][m_co[save-1][(i-1)*2+1]+1] = -1;
                direction = 'w';
            }
            }
            for (b = 0; b < 2; ++b)
            {
                for (j = 0; j < 13; ++j)
                {
                    for (k = 0; k < 13; ++k)
                    {
                        maze2[b][j][k] = maze[j][k];
                    }
                }
            }
            for (j = 0; j < 13; ++j)
                {
                    for (k = 0; k < 13; ++k)
                    {
                        if (maze2[0][j][k] == 0){
                            printf(" ");
                        }
                        printf("%d ", maze2[0][j][k]);
                    }
                    printf("\n");
                }
                printf("\n");


            if (save == 0){
            if (movement[i] != 'u'){
            x = phase1(x, maze2[0], coordinates[(i-1)*2], coordinates[(i-1)*2+1], coordinates[(i-1)*2+2], coordinates[(i-1)*2+3], road);
            if(x==1){
                    i--;
            }
            else{
            printf("x=%d ", x);
            if(lock == 1){
                i--;
            }
            first_i = i;
            cnt_save[save]= 0;
            for(b=0; b < x-1; ++b){
                m_co[save][b] = road[0][b+1];
            }
            printf(" direction: %c", direction);
            printf("\n movement: %d\n", x);
            n_dir=direction;
            for (b = 0; b < x/2 - 1; ++b)
                {
                    printf("k");
                    if(road[0][b*2+1] - 2 == road[0][b*2+3] && direction == 'n')
                    {
                        m_movement[save][b] = 'f';
                        direction = 'n';
                    }
                    else if(road[0][b*2+1] - 2 == road[0][b*2+3] && direction == 'e')
                    {
                        m_movement[save][b] = 'l';
                        direction = 'n';
                    }
                    else if(road[0][b*2+1] - 2 == road[0][b*2+3] && direction == 'w')
                    {
                        m_movement[save][b] = 'r';
                        direction = 'n';
                    }
                    else if(road[0][b*2+1] + 2 == road[0][b*2+3] && direction == 'e')
                    {
                        m_movement[save][b] = 'r';
                        direction = 's';
                    }
                    else if(road[0][b*2+1] + 2 == road[0][b*2+3] && direction == 'w')
                    {
                        m_movement[save][b] = 'l';
                        direction = 's';
                    }
                    else if(road[0][b*2+1] + 2 == road[0][b*2+3] && direction == 's')
                    {
                        m_movement[save][b] = 'f';
                        direction = 's';
                    }
                    else if(road[0][b*2+2] + 2 == road[0][b*2+4] && direction == 'n')
                    {
                        m_movement[save][b] = 'r';
                        direction = 'e';
                    }
                    else if(road[0][b*2+2] + 2 == road[0][b*2+4] && direction == 'e')
                    {
                        m_movement[save][b] = 'f';
                        direction = 'e';
                    }
                    else if(road[0][b*2+2] + 2 == road[0][b*2+4] && direction == 's')
                    {
                        m_movement[save][b] = 'l';
                        direction = 'e';
                    }
                    else if(road[0][b*2+2] - 2 == road[0][b*2+4] && direction == 'n')
                    {
                        m_movement[save][b] = 'l';
                        direction = 'w';
                    }
                    else if(road[0][b*2+2] - 2 == road[0][b*2+4] && direction == 'w')
                    {
                        m_movement[save][b] = 'f';
                        direction = 'w';
                    }
                    else if(road[0][b*2+2] - 2 == road[0][b*2+4] && direction == 's')
                    {
                        m_movement[save][b] = 'r';
                        direction = 'w';
                    }
                    printf("%c ", m_movement[save][b]);
                    cnt_save[save]++;
                }
                end_dir[save]=direction;
                direction=n_dir;
                save++;
                i=-1;
            }
            }
            }
            else{

            printf("\ni=: %d",i);
            if (m_movement[save-1][i] != 'u'){
            x = phase1(x, maze2[0], m_co[save-1][(i-1)*2], m_co[save-1][(i-1)*2+1], m_co[save-1][(i-1)*2+2], m_co[save-1][(i-1)*2+3], road);
            if(x==1){
                    i--;
            }
            else{
            if(lock == 1){
                save--;
                i--;
            }
            save_i[save-1] = i;
            cnt_save[save]= 0;
            for(b=0; b < x-1; ++b){
                m_co[save][b] = road[0][b+1];
            }
            printf(" direction: %c", direction);
            printf("\n movement: %d\n", x);
            n_dir=direction;
            for (b = 0; b < x/2 - 1; ++b)
                {
                    printf("k");
                    if(road[0][b*2+1] - 2 == road[0][b*2+3] && direction == 'n')
                    {
                        m_movement[save][b] = 'f';
                        direction = 'n';
                    }
                    else if(road[0][b*2+1] - 2 == road[0][b*2+3] && direction == 'e')
                    {
                        m_movement[save][b] = 'l';
                        direction = 'n';
                    }
                    else if(road[0][b*2+1] - 2 == road[0][b*2+3] && direction == 'w')
                    {
                        m_movement[save][b] = 'r';
                        direction = 'n';
                    }
                    else if(road[0][b*2+1] + 2 == road[0][b*2+3] && direction == 'e')
                    {
                        m_movement[save][b] = 'r';
                        direction = 's';
                    }
                    else if(road[0][b*2+1] + 2 == road[0][b*2+3] && direction == 'w')
                    {
                        m_movement[save][b] = 'l';
                        direction = 's';
                    }
                    else if(road[0][b*2+1] + 2 == road[0][b*2+3] && direction == 's')
                    {
                        m_movement[save][b] = 'f';
                        direction = 's';
                    }
                    else if(road[0][b*2+2] + 2 == road[0][b*2+4] && direction == 'n')
                    {
                        m_movement[save][b] = 'r';
                        direction = 'e';
                    }
                    else if(road[0][b*2+2] + 2 == road[0][b*2+4] && direction == 'e')
                    {
                        m_movement[save][b] = 'f';
                        direction = 'e';
                    }
                    else if(road[0][b*2+2] + 2 == road[0][b*2+4] && direction == 's')
                    {
                        m_movement[save][b] = 'l';
                        direction = 'e';
                    }
                    else if(road[0][b*2+2] - 2 == road[0][b*2+4] && direction == 'n')
                    {
                        m_movement[save][b] = 'l';
                        direction = 'w';
                    }
                    else if(road[0][b*2+2] - 2 == road[0][b*2+4] && direction == 'w')
                    {
                        m_movement[save][b] = 'f';
                        direction = 'w';
                    }
                    else if(road[0][b*2+2] - 2 == road[0][b*2+4] && direction == 's')
                    {
                        m_movement[save][b] = 'r';
                        direction = 'w';
                    }
                    printf("%c ", m_movement[save][b]);
                    cnt_save[save]++;
                }
                end_dir[save]=direction;
                direction=n_dir;
                save++;
                i=-1;
            }
            }
            }
        }





        if (out == 'g'||out == 'z' || out == 'm')
        {
            if (lock==1){
                lock = 0;
            }
            if (cnt_save[save-1]>0){
                printf("\na\n");
                i++;
                printf("%d", i);
                in = m_movement[save-1][i];
                cnt_save[save-1]--;
            }
            else if (save>0){
                printf("\nb\n");
                save--;
                printf("\nsave = %d\n", save);

                printf("%d", save_i[save-1]);
                printf(" %c", end_dir[save]);
                if (save==0){
                i = first_i;
                if(end_dir[save] == 'n'){
                    if (coordinates[i*2] + 2 == coordinates[i*2+2]){
                        in = 'u';
                    }
                    if (coordinates[i*2] - 2 == coordinates[i*2+2]){
                        in = 'f';
                    }
                    if (coordinates[i*2+1] + 2 == coordinates[i*2+3]){
                        in = 'r';
                    }
                    if (coordinates[i*2+1] - 2 == coordinates[i*2+3]){
                        in = 'l';
                    }
                }
                if(end_dir[save] == 'e'){
                    if (coordinates[i*2] + 2 == coordinates[i*2+2]){
                        in = 'r';
                    }
                    if (coordinates[i*2] - 2 == coordinates[i*2+2]){
                        in = 'l';
                    }
                    if (coordinates[i*2+1] + 2 == coordinates[i*2+3]){
                        in = 'f';
                    }
                    if (coordinates[i*2+1] - 2 == coordinates[i*2+3]){
                        in = 'u';
                    }
                }
                if(end_dir[save] == 's'){
                    if (coordinates[i*2] + 2 == coordinates[i*2+2]){
                        in = 'f';
                    }
                    if (coordinates[i*2] - 2 == coordinates[i*2+2]){
                        in = 'u';
                    }
                    if (coordinates[i*2+1] + 2 == coordinates[i*2+3]){
                        in = 'l';
                    }
                    if (coordinates[i*2+1] - 2 == coordinates[i*2+3]){
                        in = 'r';
                    }
                }
                if(end_dir[save] == 'w'){
                    if (coordinates[i*2] + 2 == coordinates[i*2+2]){
                        in = 'l';
                    }
                    if (coordinates[i*2] - 2 == coordinates[i*2+2]){
                        in = 'r';
                    }
                    if (coordinates[i*2+1] + 2 == coordinates[i*2+3]){
                        in = 'u';
                    }
                    if (coordinates[i*2+1] - 2 == coordinates[i*2+3]){
                        in = 'f';
                    }
                }
                }
                else{
                i = save_i[save-1];
                printf("%d (%d %d) (%d %d)",i, m_co[save-1][i*2], m_co[save-1][i*2+1], m_co[save-1][i*2+2], m_co[save-1][i*2+3]);
                if(end_dir[save] == 'n'){
                    if (m_co[save-1][i*2] + 2 == m_co[save-1][i*2+2]){
                        in = 'u';
                    }
                    if (m_co[save-1][i*2] - 2 == m_co[save-1][i*2+2]){
                        in = 'f';
                    }
                    if (m_co[save-1][i*2+1] + 2 == m_co[save-1][i*2+3]){
                        in = 'r';
                    }
                    if (m_co[save-1][i*2+1] - 2 == m_co[save-1][i*2+3]){
                        in = 'l';
                    }
                }
                if(end_dir[save] == 's'){
                    if (m_co[save-1][i*2] + 2 == m_co[save-1][i*2+2]){
                        in = 'f';
                    }
                    if (m_co[save-1][i*2] - 2 == m_co[save-1][i*2+2]){
                        in = 'u';
                    }
                    if (m_co[save-1][i*2+1] + 2 == m_co[save-1][i*2+3]){
                        in = 'l';
                    }
                    if (m_co[save-1][i*2+1] - 2 == m_co[save-1][i*2+3]){
                        in = 'r';
                    }
                }
                if(end_dir[save] == 'e'){
                    if (m_co[save-1][i*2] + 2 == m_co[save-1][i*2+2]){
                        in = 'r';
                    }
                    if (m_co[save-1][i*2] - 2 == m_co[save-1][i*2+2]){
                        in = 'l';
                    }
                    if (m_co[save-1][i*2+1] + 2 == m_co[save-1][i*2+3]){
                        in = 'f';
                    }
                    if (m_co[save-1][i*2+1] - 2 == m_co[save-1][i*2+3]){
                        in = 'u';
                    }
                }
                if(end_dir[save] == 'w'){
                    if (m_co[save-1][i*2] + 2 == m_co[save-1][i*2+2]){
                        in = 'l';
                    }
                    if (m_co[save-1][i*2] - 2 == m_co[save-1][i*2+2]){
                        in = 'r';
                    }
                    if (m_co[save-1][i*2+1] + 2 == m_co[save-1][i*2+3]){
                        in = 'u';
                    }
                    if (m_co[save-1][i*2+1] - 2 == m_co[save-1][i*2+3]){
                        in = 'f';
                    }
                }
                }
                lock = 1;
            }
            else{
                printf("\nc\n");
                printf("%d", i);
                i++;
                in = movement[i];
            }
            if (in == 'q')
            {
                robot_lib_disconnect();
                break;
            }
            printf("\n\ninput: %c\n", in);
            robot_lib_write_byte(in);

        }
    }
    in = 'k';
    printf("\n\ninput: %c\n", in);
    robot_lib_write_byte(in);
    direction = 'n';
    i=1;
    while(1){
        robot_lib_read_byte(&out);
        if (out == 'n' || out == 'z' || out == 'g' || out == 'x' || out == 'm' || out == 'e')
        {
            printf("\n");
            printf("\n");
            printf("read: %c", out);
            printf("\n");
        }
        if (out == 'n')
        {
            break;
        }
        if (out == 'z')
        {
            printf("fuuck");
            n_dir=direction;
            phase(maze2[0], maze2[1], l_movement, 12, 4, i, direction);
        }
        if (out == 'g'||out == 'z' || out == 't' || out == 'm')
        {
            in = l_movement[i];
            i++;
            if (in == 'q')
            {
                robot_lib_disconnect();
                break;
            }
            printf("input: %c\n", in);
            robot_lib_write_byte(in);
        }
    }
    return 0;
}

