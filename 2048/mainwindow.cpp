#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <cstdlib>
#include <iostream>
#include <cstdio>
#include <time.h>
#include <vector>
#include <QKeyEvent>
#include <QAction>
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QPixmap bckground(":/picture/picture/background.jpg");
    bckground = bckground.scaled(this->size());
    QPalette palette;
    palette.setBrush(QPalette::Background, bckground);
    this->setPalette(palette);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::start_game(){
    memset(map,0,sizeof(map));    //初始化map
    memset(backup,0,sizeof(map));
    srand((unsigned)time(NULL));
    score=0;
    score_count();
    for(int i=0;i<2;i++){
        int location=rand()%16;
        int index=rand()%2;
        if(map[location]==0){
            if(index==1)
                map[location]=2;
            else
                map[location]=4;
        }
        else continue;
    }
    setmap();
}

void MainWindow::random_num()
{
    srand((unsigned)time(NULL));
    o_left=false;
    o_right=false;
    o_up=false;
    o_down=false;
    int index=rand()%2;
    bool check=false;
    while(!check){
        int location=rand()%16;
        if(map[location]==0){
            if(index==0){
                check=true;
                map[location]=2;
            }
            else{
                check=true;
                map[location]=4;
            }
        }
        else continue;
    }
    setmap();
}

void MainWindow::game_over(){
    QPixmap over(":/picture/picture/gameover.jpg");
    ui->label_over->setPixmap(over);
    ui->label_over->setScaledContents(true);
}


bool MainWindow::check_left(){

    int check=0,i,j,k;       //檢查看按key後有沒有做任何動作
    for(i=0;i<16;i++)
        backup[i]=map[i];
    for(i=0;i<=3;i++){        //第一行
        for(j=i;j<=15;j+=4){  //每一排 （現在的位置）
            if(map[j]!=0){
                for(k=j+4;k<=15;k+=4){   //檢查接下來的號碼有沒有跟現在的位置一樣
                    if( (map[k]!=0) && (map[j]!=map[k]))
                        break;
                    if( (map[j]==map[k]) && (map[j]!=0) && (map[k]!=0) ){     //如果有的話，而且不是0
                        map[j]=map[j]*2;       //現在的位置等於現在的號碼*2
                        map[k]=0;              //跟現在的位置同樣號碼的位置變為0，因為合併了                        
                        check++;               //有做動作，所以check++
                        break;
                    }
                }
            }
        }
    }
    for(i=0;i<=3;i++){              //這裡是移動位置，合併後把0之後的號碼移前
        for(j=i;j<=15;j+=4){
            if(map[j]==0){      //檢查到有0後就開始檢查後面的位置有沒有不是0的，就移過來
                for(k=j+4;k<=15;k+=4){
                    if(map[k]!=0){
                        map[j]=map[k];
                        map[k]=0;
                        check++;
                        break;
                    }
                }
            }
        }
    }
    if(check!=0){    //如果有做到任何動作，表示按鍵可以執行return true
        for(i=0;i<16;i++)
            undo[i]=backup[i];
        return true;
    }
    else{
        o_left=true;    //表示此按鍵按過了
        if(o_left&&o_down&&o_right&&o_up)
            game_over();//呼叫Game Over!
        return false;
    }
}

bool MainWindow::check_right(){

    int check=0,i,j,k;       //檢查看按key後有沒有做任何動作
    for(i=0;i<16;i++)
        backup[i]=map[i];
    for(i=12;i<=15;i++){        //最後一行
        for(j=i;j>=0;j-=4){  //每一排 （現在的位置）
            if(map[j]!=0){
                for(k=j-4;k>=0;k-=4){   //檢查接下來的號碼有沒有跟現在的位置一樣
                    if( (map[k]!=0) && (map[j]!=map[k]))
                        break;
                    if( (map[j]==map[k]) && (map[j]!=0) && (map[k]!=0) ){     //如果有的話，而且不是0
                        map[j]=map[j]*2;       //現在的位置等於現在的號碼*2
                        map[k]=0;              //跟現在的位置同樣號碼的位置變為0，因為合併了
                        check++;               //有做動作，所以check++
                        break;
                    }
                }
            }
        }
    }
    for(i=12;i<=15;i++){              //這裡是移動位置，合併後把0之後的號碼移前
        for(j=i;j>=0;j-=4){
            if(map[j]==0){      //檢查到有0後就開始檢查後面的位置有沒有不是0的，就移過來
                for(k=j-4;k>=0;k-=4){
                    if(map[k]!=0){
                        map[j]=map[k];
                        map[k]=0;
                        check++;
                        break;
                    }
                }
            }
        }
    }
    if(check!=0){    //如果有做到任何動作，表示按鍵可以執行return true
        for(i=0;i<16;i++)
            undo[i]=backup[i];
        return true;
    }
    else{
        o_right=true;    //表示此按鍵按過了
        if(o_left&&o_down&&o_right&&o_up)
            game_over();//呼叫Game Over!
        return false;
    }
}

bool MainWindow::check_up(){

    int check=0,i,j,k;       //檢查看按key後有沒有做任何動作
    for(i=0;i<16;i++)
        backup[i]=map[i];
    for(i=0;i<=15;i+=4){        //第一排
        for(j=i;j<=(i+3);j++){  //每一行
            if(map[j]!=0){
                for(k=(j+1);k<=(i+3);k++){   //檢查接下來的號碼有沒有跟現在的位置一樣
                    if( (map[k]!=0) && (map[j]!=map[k]) )
                        break;
                    if( (map[j]==map[k]) && (map[j]!=0) && (map[k]!=0) ){     //如果有的話，而且不是0
                        map[j]=map[j]*2;       //現在的位置等於現在的號碼*2
                        map[k]=0;              //跟現在的位置同樣號碼的位置變為0，因為合併了
                        check++;               //有做動作，所以check++
                        break;
                    }
                }
            }
        }
    }
    for(i=0;i<=15;i+=4){              //這裡是移動位置，合併後把0之後的號碼移前
        for(j=i;j<=(i+3);j++){
            if(map[j]==0){      //檢查到有0後就開始檢查後面的位置有沒有不是0的，就移過來
                for(k=(j+1);k<=(i+3);k++){
                    if(map[k]!=0){
                        map[j]=map[k];
                        map[k]=0;
                        check++;
                        break;
                    }
                }
            }
        }
    }
    if(check!=0){    //如果有做到任何動作，表示按鍵可以執行return true
        for(i=0;i<16;i++)
            undo[i]=backup[i];
        return true;
    }
    else{
        o_up=true;    //表示此按鍵按過了
        if(o_left&&o_down&&o_right&&o_up)
            game_over();//呼叫Game Over!
        return false;
    }
}

bool MainWindow::check_down(){

    int check=0,i,j,k;       //檢查看按key後有沒有做任何動作
    for(i=0;i<16;i++)
        backup[i]=map[i];
    for(i=3;i<=15;i+=4){        // 最後一排
        for(j=i;j>=(i-3);j--){  // 每一行
            if(map[j]!=0){
                for(k=(j-1);k>=(i-3);k--){   //檢查接下來的號碼有沒有跟現在的位置一樣
                    if( (map[k]!=0) && (map[j]!=map[k]))
                        break;
                    if((map[j]==map[k]) && (map[j]!=0) && (map[k]!=0) ){     //如果有的話，而且不是0
                        map[j]=map[j]*2;       //現在的位置等於現在的號碼*2
                        map[k]=0;              //跟現在的位置同樣號碼的位置變為0，因為合併了
                        check++;               //有做動作，所以check++
                        break;
                    }
                }
            }
        }
    }
    for(i=3;i<=15;i+=4){              //這裡是移動位置，合併後把0之後的號碼移前
        for(j=i;j>=(i-3);j--){
            if(map[j]==0){      //檢查到有0後就開始檢查後面的位置有沒有不是0的，就移過來
                for(k=(j-1);k>=(i-3);k--){
                    if(map[k]!=0){
                        map[j]=map[k];
                        map[k]=0;
                        check++;
                        break;
                    }
                }
            }
        }
    }
    if(check!=0){    //如果有做到任何動作，表示按鍵可以執行return true
        for(i=0;i<16;i++)
            undo[i]=backup[i];
        return true;
    }
    else{
        o_down=true;    //表示此按鍵按過了
        if(o_left&&o_down&&o_right&&o_up)
            game_over();//呼叫Game Over!
        return false;
    }
}


void MainWindow::keyPressEvent(QKeyEvent *event){
    bool check;int yo=0;
    if(event->key()==Qt::Key_Left){
        check=check_left(); //檢查看按鍵可不可以執行
        if(check){          //可以執行就set map，然後全部按鍵初始化過（表示還沒按過）
            o_left=false;
            o_right=false;
            o_up=false;
            o_down=false;
            check=false;
            back_score=score;score+=4000;
            random_num();
        }
    }
    if(event->key()==Qt::Key_Right){
        check=check_right();
        if(check){
            o_left=false;
            o_right=false;
            o_up=false;
            o_down=false;
            check=false;
            back_score=score;score+=4000;
            random_num();
        }
    }
    if(event->key()==Qt::Key_Up){
        check=check_up();
        if(check){
            o_left=false;
            o_right=false;
            o_up=false;
            o_down=false;
            check=false;
            back_score=score;score+=4000;
            random_num();
        }
    }
    if(event->key()==Qt::Key_Down){
        check=check_down();
        if(check){
            o_left=false;
            o_right=false;
            o_up=false;
            o_down=false;
            check=false;
            back_score=score;score+=4000;
            random_num();
        }
    }
    if(event->key()==Qt::Key_U){
        for(int i=0;i<16;i++){
            if(undo[i]==0)
                yo++;
        }
        if(yo!=16){
            for(int i=0;i<16;i++)
                map[i]=undo[i];
            ui->label_over->clear();
            ui->label_end->clear();
            score=back_score;
            setmap();
        }
    }
    if(event->key()==Qt::Key_R){
        ui->label_over->clear();
        ui->label_end->clear();
        start_game();
    }
}

void MainWindow::setmap()
{
     QPixmap pix[12] = {QPixmap(":/picture/picture/grid.jpg"),
                        QPixmap(":/picture/picture/2.jpg"),
                        QPixmap(":/picture/picture/4.jpg"),
                        QPixmap(":/picture/picture/8.jpg"),
                        QPixmap(":/picture/picture/16.jpg"),
                        QPixmap(":/picture/picture/32.jpg"),
                        QPixmap(":/picture/picture/64.jpg"),
                        QPixmap(":/picture/picture/128.jpg"),
                        QPixmap(":/picture/picture/256.jpg"),
                        QPixmap(":/picture/picture/512.jpg"),
                        QPixmap(":/picture/picture/1024.jpg"),
                        QPixmap(":/picture/picture/2048.jpg")};
    if(mode==1){
        pix[0] = QPixmap(":/picture/picture/handsome grid.jpg");
    }

    int index,check;
    //print out the picture
    for(int i=0;i<16;i++){
        index=1;
        check=2;
        if(i==0){
            if(map[i]==0){
                ui->label_0->setPixmap(pix[0]);
                ui->label_0->setScaledContents(true);
                continue;
            }
            else{
                while(check!=map[i]){
                    check*=2;
                    index++;
                }
                ui->label_0->setPixmap(pix[index]);
                ui->label_0->setScaledContents(true);
                continue;
            }
        }
        else if(i==1){
            if(map[i]==0){
                ui->label_1->setPixmap(pix[0]);
                ui->label_1->setScaledContents(true);
                continue;
            }
            else{
                while(check!=map[i]){
                    check*=2;
                    index++;
                }
                ui->label_1->setPixmap(pix[index]);
                ui->label_1->setScaledContents(true);
                continue;
            }
        }
        else if(i==2){
            if(map[i]==0){
                ui->label_2->setPixmap(pix[0]);
                ui->label_2->setScaledContents(true);
                continue;
            }
            else{
                while(check!=map[i]){
                    check*=2;
                    index++;
                }
                ui->label_2->setPixmap(pix[index]);
                ui->label_2->setScaledContents(true);
                continue;
            }
        }
        else if(i==3){
            if(map[i]==0){
                ui->label_3->setPixmap(pix[0]);
                ui->label_3->setScaledContents(true);
                continue;
            }
            else{
                while(check!=map[i]){
                    check*=2;
                    index++;
                }
                ui->label_3->setPixmap(pix[index]);
                ui->label_3->setScaledContents(true);
                continue;
            }
        }
        else if(i==4){
            if(map[i]==0){
                ui->label_4->setPixmap(pix[0]);
                ui->label_4->setScaledContents(true);
                continue;
            }
            else{
                while(check!=map[i]){
                    check*=2;
                    index++;
                }
                ui->label_4->setPixmap(pix[index]);
                ui->label_4->setScaledContents(true);
                continue;
            }
        }
        else if(i==5){
            if(map[i]==0){
                ui->label_5->setPixmap(pix[0]);
                ui->label_5->setScaledContents(true);
                continue;
            }
            else{
                while(check!=map[i]){
                    check*=2;
                    index++;
                }
                ui->label_5->setPixmap(pix[index]);
                ui->label_5->setScaledContents(true);
                continue;
            }
        }
        else if(i==6){
            if(map[i]==0){
                ui->label_6->setPixmap(pix[0]);
                ui->label_6->setScaledContents(true);
                continue;
            }
            else{
                while(check!=map[i]){
                    check*=2;
                    index++;
                }
                ui->label_6->setPixmap(pix[index]);
                ui->label_6->setScaledContents(true);
                continue;
            }
        }
        else if(i==7){
            if(map[i]==0){
                ui->label_7->setPixmap(pix[0]);
                ui->label_7->setScaledContents(true);
                continue;
            }
            else{
                while(check!=map[i]){
                    check*=2;
                    index++;
                }
                ui->label_7->setPixmap(pix[index]);
                ui->label_7->setScaledContents(true);
                continue;
            }
        }
        else if(i==8){
            if(map[i]==0){
                ui->label_8->setPixmap(pix[0]);
                ui->label_8->setScaledContents(true);
                continue;
            }
            else{
                while(check!=map[i]){
                    check*=2;
                    index++;
                }
                ui->label_8->setPixmap(pix[index]);
                ui->label_8->setScaledContents(true);
                continue;
            }
        }
        else if(i==9){
            if(map[i]==0){
                ui->label_9->setPixmap(pix[0]);
                ui->label_9->setScaledContents(true);
                continue;
            }
            else{
                while(check!=map[i]){
                    check*=2;
                    index++;
                }
                ui->label_9->setPixmap(pix[index]);
                ui->label_9->setScaledContents(true);
                continue;
            }
        }
        else if(i==10){
            if(map[i]==0){
                ui->label_10->setPixmap(pix[0]);
                ui->label_10->setScaledContents(true);
                continue;
            }
            else{
                while(check!=map[i]){
                    check*=2;
                    index++;
                }
                ui->label_10->setPixmap(pix[index]);
                ui->label_10->setScaledContents(true);
                continue;
            }
        }
        else if(i==11){
            if(map[i]==0){
                ui->label_11->setPixmap(pix[0]);
                ui->label_11->setScaledContents(true);
                continue;
            }
            else{
                while(check!=map[i]){
                    check*=2;
                    index++;
                }
                ui->label_11->setPixmap(pix[index]);
                ui->label_11->setScaledContents(true);
                continue;
            }
        }
        else if(i==12){
            if(map[i]==0){
                ui->label_12->setPixmap(pix[0]);
                ui->label_12->setScaledContents(true);
                continue;
            }
            else{
                while(check!=map[i]){
                    check*=2;
                    index++;
                }
                ui->label_12->setPixmap(pix[index]);
                ui->label_12->setScaledContents(true);
                continue;
            }
        }
        else if(i==13){
            if(map[i]==0){
                ui->label_13->setPixmap(pix[0]);
                ui->label_13->setScaledContents(true);
                continue;
            }
            else{
                while(check!=map[i]){
                    check*=2;
                    index++;
                }
                ui->label_13->setPixmap(pix[index]);
                ui->label_13->setScaledContents(true);
                continue;
            }
        }
        else if(i==14){
            if(map[i]==0){
                ui->label_14->setPixmap(pix[0]);
                ui->label_14->setScaledContents(true);
                continue;
            }
            else{
                while(check!=map[i]){
                    check*=2;
                    index++;
                }
                ui->label_14->setPixmap(pix[index]);
                ui->label_14->setScaledContents(true);
                continue;
            }
        }
        else if(i==15){
            if(map[i]==0){
                ui->label_15->setPixmap(pix[0]);
                ui->label_15->setScaledContents(true);
                continue;
            }
            else{
                while(check!=map[i]){
                    check*=2;
                    index++;
                }
                ui->label_15->setPixmap(pix[index]);
                ui->label_15->setScaledContents(true);
                continue;
            }
        }
    }
    score_count();
    for(int k=0;k<16;k++){
        if(map[k]==2048)
            end_game();   //呼叫End Game!
    }
}

void MainWindow::end_game(){
    QPixmap end(":/picture/picture/endgame.jpg");
    ui->label_end->setPixmap(end);
    ui->label_end->setScaledContents(true);
}

void MainWindow::score_count(){
    ui->label_score->setText("Score: " + QString::number(score));
    ui->label_score->setStyleSheet("color:silver");
}

void MainWindow::on_actionNormal_Mode_triggered()
{
    mode=0;
    start_game();
}

void MainWindow::on_actionHandsome_Mode_triggered()
{
    mode=1;
    start_game();
}

void MainWindow::on_RButton_clicked()
{
    ui->label_over->clear();
    ui->label_end->clear();
    start_game();
}

void MainWindow::on_pushButton_clicked()
{
    int yo=0;
    for(int i=0;i<16;i++){
        if(undo[i]==0)
            yo++;
    }
    if(yo!=16){
        for(int i=0;i<16;i++)
            map[i]=undo[i];
        ui->label_over->clear();
        ui->label_end->clear();
        score=back_score;
        setmap();
    }
}
